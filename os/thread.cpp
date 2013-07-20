#include "thread.h"

List<Thread> Thread::ready_queue __attribute__ ((init_priority (1000)));
List<Thread> Thread::sleep_queue;
Thread Thread::main_thread __attribute__ ((init_priority (1001)));
Thread* Thread::active_thread = &Thread::main_thread;
