#pragma once

#include <cstdint>

template <typename T>
class mmio_ptr {
    private:
        const uintptr_t p;

    protected:
        T* ptr() const {
            return reinterpret_cast<T*>(p);
        }

    public:
        constexpr mmio_ptr(uintptr_t p) : p(p) {}

        T* operator->() const {
            return ptr();
        }

        T& operator*() const {
            return *ptr();
        }

        T& operator[](std::size_t idx) const {
            return ptr()[idx];
        }
};
