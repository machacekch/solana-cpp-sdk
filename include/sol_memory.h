#pragma once
/**
 * Low-level operations on memory and addresses
 */

namespace sol {

// returns the nearest address divisible by BYTES, that's larger than or equal to the argument
template<uint64_t BYTES>
constexpr uint64_t aligned(uint64_t address) {
    static_assert(BYTES && !(BYTES & (BYTES - 1)), "BYTES must be a power of two");
    return (address + BYTES - 1) & ~(BYTES - 1);
}

// Example: ptr = aligned<8>(ptr);
template<uint64_t BYTES, class T>
constexpr T* aligned(const T* ptr) {
    return reinterpret_cast<T*>(aligned<BYTES>(reinterpret_cast<uint64_t>(ptr)));
}

inline void memcpy(void* dst, const void* src, int len) {
    for (int i = 0; i < len; ++i) {
        *((uint8_t*)dst + i) = *((const uint8_t*)src + i);
    }
}

} // namespace sol
