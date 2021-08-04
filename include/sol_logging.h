#pragma once
/**
 * Run-time logging
 *
 * Examples:
 * log("The account", account_info.pubkey(), "has", account_info.data_len(), "bytes of data.");
 * log("Clock:", sysvar::Clock());
 *
 * The log function is overloaded for a number of types such as Pubkey or AccountInfo.
 * Users may define additional overloads for their custom types.
 * Note that there is a variadic template that calls the single-parameter version of log on each argument.
 *
 * TODO: Consider renaming log() to print(), to avoid confusion with the logarithm function std::log.
 */

#include <sol_int.h>
#include <sol_internal.h>
#include <sol_string.h>
#include <sol_syscalls.h>

namespace sol {

// log a statically allocated array, the entries of which must all be valid
template<class T, uint64_t ARRAY_SIZE>
void log_array(const T (&a)[ARRAY_SIZE]) {
    for (uint64_t i = 0; i != ARRAY_SIZE; ++i) {
        log(a[i]);
    }
}

// log each element of an array
template<class T>
void log_array(const T* a, const uint64_t ARRAY_SIZE) {
    for (uint64_t i = 0; i != ARRAY_SIZE; ++i) {
        log(a[i]);
    }
}

inline void log(uint64_t number) {
    syscall::sol_log_64_(0, 0, 0, 0, number);
}

// when called with 2 or more arguments, sequentially call the single-argument version of log() on each
template<typename... Ts, typename internal::enable_if<(sizeof...(Ts) > 1), bool>::type = true>
void log(const Ts&... args) {
    static_assert(sizeof...(Ts) > 1);
    (log(args), ...);
}

} // namespace sol
