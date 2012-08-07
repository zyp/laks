#include "thread.h"

Thread Thread::main_thread __attribute__ ((init_priority (1000)));
Thread* Thread::active_thread = &Thread::main_thread;
