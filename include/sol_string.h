#pragma once
/**
 * String utilities
 */

#include <sol_int.h>
#include <sol_syscalls.h>

namespace sol {

inline uint64_t strlen(const char* string) {
    uint64_t len = 0;
    while (*string) {
        ++len;
        ++string;
    }
    return len;
}

inline void log(const char* string) {
    syscall::sol_log_(string, strlen(string));
}

} // namespace sol
