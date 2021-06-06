#pragma once
/**
 * Constants such as SUCCESS for returning the program status
 *
 * Builtin program status values occupy the upper 32 bits of the program return value.
 * Programs may define their own error values but they must be confined to the lower 32 bits.
 */

#include <sol_int.h>

namespace sol {

constexpr uint64_t SUCCESS = 0;

namespace error {

template<uint64_t low>
constexpr uint64_t to_builtin() {
    static_assert((low & 0xffffffff00000000) == 0);
    return low << 32;
}

constexpr uint64_t CUSTOM_ZERO = to_builtin<1>();
constexpr uint64_t INVALID_ARGUMENT = to_builtin<2>();
constexpr uint64_t INVALID_INSTRUCTION_DATA = to_builtin<3>();
constexpr uint64_t INVALID_ACCOUNT_DATA = to_builtin<4>();
constexpr uint64_t ACCOUNT_DATA_TOO_SMALL = to_builtin<5>();
constexpr uint64_t INSUFFICIENT_FUNDS = to_builtin<6>();
constexpr uint64_t INCORRECT_PROGRAM_ID = to_builtin<7>();
constexpr uint64_t MISSING_REQUIRED_SIGNATURES = to_builtin<8>();
constexpr uint64_t ACCOUNT_ALREADY_INITIALIZED = to_builtin<9>();
constexpr uint64_t UNINITIALIZED_ACCOUNT = to_builtin<10>();
constexpr uint64_t NOT_ENOUGH_ACCOUNT_KEYS = to_builtin<11>();
constexpr uint64_t ACCOUNT_BORROW_FAILED = to_builtin<12>();
constexpr uint64_t MAX_SEED_LENGTH_EXCEEDED = to_builtin<13>();
constexpr uint64_t INVALID_SEEDS = to_builtin<14>();

} // namespace error
} // namespace sol
