#ifndef MUTEX_H
#define MUTEX_H

class Mutex {
	private:
		uint8_t locked;
	public:
		Mutex() : locked(0) {}
		Mutex(uint8_t l) : locked(l) {}
		
		bool trylock() {
			uint8_t val;
			
			// Check if mutex is locked.
			asm volatile ("ldrexb %0, [%1]" : "=r" (val) : "r" (&locked));
			if(val) {
				return false;
			}
			
			// Try taking the lock.
			asm volatile ("strexb %0, %1, [%2]" : "=r" (val) : "r" (1), "r" (&locked));
			if(val) {
				return false;
			}
			
			asm volatile("dmb");
			return true;
		}
		
		void lock() {
			while(!trylock()) {
				Thread::yield();
			}
		}
		
		void unlock() {
			asm volatile("dmb");
			locked = 0;
		}
};

#endif
