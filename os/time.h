#ifndef TIME_H
#define TIME_H

#include "thread.h"

struct STK_t {
	volatile uint32_t CTRL;
	volatile uint32_t LOAD;
	volatile uint32_t VAL;
	volatile uint32_t CALIB;
};

static STK_t& STK = *(STK_t*)0xe000e010;

class Time {
	private:
		static volatile uint32_t systime;
	
	public:
		inline static void tick() {
			systime++;
		}
		
		inline static uint32_t time() {
			return systime;
		}
		
		inline static void sleep(uint32_t ms) {
			ms += systime;
			while(systime < ms) {
				Thread::yield();
			}
		}
};

#endif
