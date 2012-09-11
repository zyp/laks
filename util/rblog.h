#ifndef RBLOG_H
#define RBLOG_H

#include <stdint.h>
#include <os/time.h>

template <uint32_t E, uint32_t A = 0>
class RBLog {
	private:
		static const uint32_t num_entries = E;
		static const uint32_t num_arguments = A;
		
		struct entry_t {
			uint32_t timestamp;
			const char* string;
			uint32_t arguments[num_arguments];
		};
		
		entry_t entries[num_entries];
		uint32_t index;
	
	public:
		RBLog() : index(0) {
			for(entry_t& entry : entries) {
				entry = {0, 0};
			}
		}
		
		template <typename... Arguments>
		void log(const char* s, Arguments... a) {
			entries[index] = {Time::time(), s, {a...}};
			
			if(++index >= num_entries) {
				index = 0;
			}
		}
};

#endif
