#pragma once

#include <chrono>

#include "scheduler.h"

template <typename Duration = std::chrono::milliseconds>
struct time_scheduler_t {
    struct wakeup_future : public schedulable {
        time_scheduler_t& tsched;
        Duration time;
        wakeup_future(time_scheduler_t& tsched, Duration time) : tsched(tsched), time(time) {}

        bool await_ready() {
            return tsched.now >= time;
        }

        bool await_suspend(std::coroutine_handle<> h) {
            if(tsched.now >= time) {
                return false;
            } else {
                awaiter = h;
                tsched.push_wakeup(this);
                return true;
            }
        }

        void await_resume() {}

        void resume() {
            scheduler.schedule(*this);
        }
    };

    Duration now;

    wakeup_future* wakeup_queue;

    void push_wakeup(wakeup_future* fut) {
        wakeup_future** p = &wakeup_queue;

        while((*p) && (*p)->time <= fut->time) {
            p = reinterpret_cast<wakeup_future**>(&(*p)->next);
        }

        fut->next = *p;
        *p = fut;
    }

    async_flag tick_flag;

    void tick() {
        tick_flag.set();
    }

    task wakeup_task() {
        while(1) {
            co_await tick_flag;
            now++;

            while(wakeup_queue && wakeup_queue->time <= now) {
                auto fut = wakeup_queue;
                wakeup_queue = static_cast<wakeup_future*>(fut->next);
                fut->resume();
            }
        }
    }

    wakeup_future sleep_until(Duration time) {
        return {*this, time};
    }

    wakeup_future sleep(Duration duration) {
        return sleep_until(now + duration);
    }
};
