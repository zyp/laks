#include "interrupt.h"
#include <os/thread.h>
#include <os/time.h>

inline void __attribute__((naked)) switch_context() {
	asm volatile ("cpsid i");
	
	// Save unsaved registers.
	asm volatile ("push {r4, r5, r6, r7, r8, r9, r10, r11, lr}" ::: "memory");
	
	// Store stack pointer for old thread.
	asm volatile ("str sp, [%0]" :: "r" (&Thread::active_thread->sp));
	
	// Update running thread.
	Thread::active_thread = Thread::active_thread->next;
	
	// Fetch stack pointer for new thread.
	asm volatile ("ldr sp, [%0]" :: "r" (&Thread::active_thread->sp));
	
	asm volatile ("cpsie i");
	
	// Load registers and return.
	asm volatile ("pop {r4, r5, r6, r7, r8, r9, r10, r11, pc}" ::: "memory");
}

template<>
void interrupt<Interrupt::SVCall>() {
	switch_context();
}

template<>
void interrupt<Interrupt::SysTick>() {
	Time::tick();
}

template<> void interrupt<Interrupt::NMI>()         { while(1); }
template<> void interrupt<Interrupt::HardFault>()   { while(1); }
template<> void interrupt<Interrupt::MemManage>()   { while(1); }
template<> void interrupt<Interrupt::BusFault>()    { while(1); }
template<> void interrupt<Interrupt::UsageFault>()  { while(1); }
template<> void interrupt<Interrupt::PendSV>()      { while(1); }
