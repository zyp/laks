#ifndef THREAD_H
#define THREAD_H

#include <stdint.h>

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
		
		Thread* next;
		
		static Thread* active_thread;
		static Thread main_thread;
		
		Thread() : next(this) {}
	
	public:
		Thread(void* stack, uint32_t stack_size, void (*func)()) {
			sp = (int_frame_t*)((uint8_t*)stack + stack_size - sizeof(int_frame_t));
			
			sp->lr_ex = 0xfffffff9;
			
			// frame->lr = thread exit handler
			sp->pc = (uint32_t)func;
			sp->psr = 0x01000000;
		}
		
		void start() {
			next = active_thread->next;
			active_thread->next = this;
		}
		
		static inline void yield() {
			asm volatile("svc 0");
		}
};

#endif
