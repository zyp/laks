#pragma once

#include <coroutine>

struct schedulable {
    schedulable* next = nullptr;

    std::coroutine_handle<> awaiter;

    schedulable() {}
    schedulable(std::coroutine_handle<> h) : awaiter(h) {}
};

struct scheduler_t {
    schedulable* first = nullptr;
    schedulable** last_next_p = &first;

    void run() {
        while(first) {
            auto sch = pop_next();
            sch.awaiter.resume();
        }
    }

    schedulable& pop_next() {
        critical_section lock;

        auto sch = first;
        first = first->next;

        if(!first) {
            last_next_p = &first;
        }

        return *sch;
    }

    void schedule(schedulable& sch) {
        critical_section lock;

        sch.next = nullptr;
        *last_next_p = &sch;
        last_next_p = &sch.next;
    }
};

scheduler_t scheduler;

struct yield : public schedulable {
    bool await_ready() {
        return false;
    }

    bool await_suspend(std::coroutine_handle<> h) {
        awaiter = h;
        scheduler.schedule(*this);
        return true;
    }

    void await_resume() {}
};

template <typename T = void>
struct future : public schedulable {
    std::optional<T> value;

    bool await_ready() {
        return bool(value);
    }

    bool await_suspend(std::coroutine_handle<> h) {
        if(value) {
            return false;
        } else {
            awaiter = h;
            return true;
        }
    }

    T await_resume() {
        return *value; // TODO: move?
    }

    bool done() {
        return bool(value);
    }

    void set(T v) {
        if(value) {
            return;
        }

        value = v;

        if(awaiter) {
            scheduler.schedule(*this);
        }
    }

    void reset() {
        value.reset();
        awaiter = nullptr;
    }
};

template <>
struct future<void> : public schedulable {
    bool ready;

    future() : ready(false) {}

    bool await_ready() {
        return ready;
    }

    bool await_suspend(std::coroutine_handle<> h) {
        if(ready) {
            return false;
        } else {
            awaiter = h;
            return true;
        }
    }

    void await_resume() {
        
    }

    bool done() {
        return ready;
    }

    void set() {
        if(ready) {
            return;
        }

        ready = true;

        if(awaiter) {
            scheduler.schedule(*this);
        }
    }

    void reset() {
        awaiter = nullptr;
        ready = false;
    }
};

struct task : public schedulable {
    struct promise_type {
        task get_return_object() {
            auto handle = std::coroutine_handle<promise_type>::from_promise(*this);
            return {handle};
        }
        std::suspend_always initial_suspend() { return {}; }
        std::suspend_never final_suspend() { return {}; }
        void return_void() {}
    };

    task(std::coroutine_handle<promise_type> h) : schedulable(h) {}
};