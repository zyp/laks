#pragma once

template <typename T, std::size_t N>
struct queue {
    std::array<T, N> data;
    volatile std::size_t put_idx;
    volatile std::size_t get_idx;

    async_flag put_flag;
    async_flag get_flag;

    constexpr std::size_t _inc(std::size_t n) {
        return n + 1 < N ? n + 1 : 0;
    }

    bool empty() {
        return put_idx == get_idx;
    }

    bool full() {
        return _inc(put_idx) == get_idx;
    }

    bool put(T v) {
        if(full()) {
            return false;
        }

        data[put_idx] = v;
        put_idx = _inc(put_idx);
        put_flag.set();
        return true;
    }

    bool get(T& v) {
        if(empty()) {
            return false;
        }

        v = data[get_idx];
        get_idx = _inc(get_idx);
        get_flag.set();
        return true;
    }

    async<> async_put(T v) {
        while(!put(v)) {
            co_await get_flag;
        }
    }

    async<T> async_get() {
        T v;
        while(!get(v)) {
            co_await put_flag;
        }
        co_return v;
    }
};
