#pragma once

#include <coroutine>

template <typename T>
class promise_return {
    private:
        T val;

    public:
        void return_value(T v) {
            val = v;
        }

        T value() {
            return val;
        }
};

template <>
class promise_return<void> {
    public:
        void return_void() {}
        void value() {}
};

/// Coroutine future for asynchronous function. Implements \term{awaitable}, \term{co_await}, \term{co_return}.
///
/// \tparam T Return type.
///
/// Manages coroutine state.
/// Awaiting an empty object or destroying the coroutine state while it's being awaited results in undefined behavior.
///
template <typename T = void, typename initial_awaitable = std::suspend_always>
class async {
    public:
        class promise_type : public promise_return<T> {
            public:
                using handle_type = std::coroutine_handle<promise_type>;

                async get_return_object() {
                    return {handle_type::from_promise(*this)};
                }

                std::coroutine_handle<> parent;

                class final_awaitable {
                    public:
                        bool await_ready() { return false; }

                        std::coroutine_handle<> await_suspend(handle_type h) {
                            return h.promise().parent;
                        }

                        void await_resume() {}
                };

                initial_awaitable initial_suspend() { return {}; }
                final_awaitable final_suspend() { return {}; }
        };

        bool await_ready() {
            return handle.done();
        }

        auto await_suspend(std::coroutine_handle<> h) {
            handle.promise().parent = h;

            return handle;
        }

        T await_resume() {
            return handle.promise().value();
        }

        /// Construct empty object.
        async() : handle(nullptr) {}

        /// Transfer ownership of coroutine state, leaving *other* empty.
        async(async&& other) {
            handle = other.handle;
            other.handle = nullptr;
        }

        /// Transfer ownership of coroutine state, leaving *other* empty.
        async& operator=(async&& other) {
            handle = other.handle;
            other.handle = nullptr;

            return *this;
        }

        /// Destroy coroutine state if not empty.
        ~async() {
            if(handle) {
                handle.destroy();
            }
        }

    private:
        promise_type::handle_type handle;

        async(promise_type::handle_type h) : handle(h) {}

        async(const async&) = delete;
        async& operator=(const async&) = delete;
};
