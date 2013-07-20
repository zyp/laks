#ifndef THREAD_H
#define THREAD_H

#include <stdint.h>
#include <util/intrusive_list.h>

#include "time.h"

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
		
		enum State {
			Running,
			Sleeping,
		};
		
		State state;
		uint32_t sleep_deadline;
		
		ListHandle<Thread> queue_handle;
		
		static List<Thread> ready_queue;
		static List<Thread> sleep_queue;
		
		static Thread* active_thread;
		static Thread main_thread;
		
		static bool reschedule() {
			uint32_t now = Time::time();
			
			// Go through sleep queue.
			for(auto h : sleep_queue) {
				// Move to ready queue if deadline has expired.
				if(h->p->sleep_deadline <= now) {
					h->p->state = Running;
					ready_queue.append(*h);
					
					// Moving one element makes the iterator unusable, so break here.
					break;
				}
			}
			
			// Check whether active thread still is ready.
			if(active_thread->state == Running) {
				// Move thread to end of ready queue.
				ready_queue.append(active_thread->queue_handle);
			}
			
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
		
		void set_sleeping(uint32_t until) {
			sleep_deadline = until;
			state = Sleeping;
			
			// Search for thread with later deadline.
			for(auto h : sleep_queue) {
				// Insert before if found.
				if(h->p->sleep_deadline > sleep_deadline) {
					queue_handle.insert_before(*h);
					return;
				}
			}
			
			// Otherwise append to end of queue.
			sleep_queue.append(queue_handle);
		}
		
		static void sleep(uint32_t delay) {
			active_thread->set_sleeping(Time::time() + delay);
			
			yield();
		}
		
		static inline void yield() {
			asm volatile("svc 0" ::: "memory");
		}
};

#endif
