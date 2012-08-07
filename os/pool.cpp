#include "pool.h"

void* operator new(unsigned int, char* buf) {
	return (void*)buf;
}
