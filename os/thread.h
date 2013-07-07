#ifndef THREAD_H
#define THREAD_H

#include <stdint.h>
#include <util/intrusive_list.h>

class Thread {
	friend void switch_context();
	
	private:
		struct int_frame_t {
			// Software saved.
			uint32_t r4;
			uint32_t r5;
			uint32_t r6;
			uint32_t r7;
			uint32_t r8;
			uint32_t r9;
			uint32_t r10;
			uint32_t r11;
			uint32_t lr_ex;
			
			// Hardware saved.
			uint32_t r0;
			uint32_t r1;
			uint32_t r2;
			uint32_t r3;
			uint32_t r12;
			uint32_t lr;
			uint32_t pc;
			uint32_t psr;
		};
		
		int_frame_t* sp;
		
		ListHandle<Thread> queue_handle;
		
		static List<Thread> ready_queue;
		
		static Thread* active_thread;
		static Thread main_thread;
		
		static bool reschedule() {
			//active_thread = active_thread->next;
			
			// TODO: Check whether active thread still is ready.
			
			// Move thread to end of ready queue.
			ready_queue.append(active_thread->queue_handle);
			
			// Check whether any threads are ready to run.
			if(ready_queue.empty()) {
				return false;
			}
			
			// Set front of ready queue as active.
			active_thread = ready_queue.begin()->p;
			
			return true;
		}
		
		Thread() : queue_handle(this) {
			ready_queue.append(queue_handle);
		}
	
	public:
		Thread(void* stack, uint32_t stack_size, void (*func)()) : queue_handle(this) {
			sp = (int_frame_t*)((uint8_t*)stack + stack_size - sizeof(int_frame_t));
			
			sp->lr_ex = 0xfffffff9;
			
			// frame->lr = thread exit handler
			sp->pc = (uint32_t)func & ~1;
			sp->psr = 0x01000000;
		}
		
		void start() {
			ready_queue.append(queue_handle);
		}
		
		static inline void yield() {
			asm volatile("svc 0" ::: "memory");
		}
};

#endif
