#pragma once

#include <concepts>
#include <cstdint>
#include <span>

template <typename T, typename U = void>
concept async_awaitable = requires(T a)
{
    // TODO: await_ready and await_suspend
    { a.await_resume() } -> std::same_as<U>;
};

template <typename T, typename U>
concept async_source = requires(T a)
{
    { a.async_get() } -> async_awaitable<U>;
};

template <typename T, typename U>
concept async_sink = requires(T a, U v)
{
    { a.async_put(v) } -> async_awaitable<>;
};

template <typename T>
concept async_byte_source = async_source<T, uint8_t>;

template <typename T>
concept async_byte_sink = async_sink<T, uint8_t>;

template <typename T>
concept async_packet_source = requires(T a, std::span<uint8_t> buf)
{
    { a.async_read(buf) } -> async_awaitable<std::span<uint8_t>>;
};

template <typename T>
concept async_packet_sink = requires(T a, std::span<uint8_t> buf)
{
    { a.async_write(buf) } -> async_awaitable<>;
};
