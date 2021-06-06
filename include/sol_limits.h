#pragma once
/**
 * Limits imposed by the Solana runtime, as well as numeric limits of integral types
 */

#include <sol_int.h>

namespace sol {

// see solana/sdk/program/src/pubkey.rs
constexpr uint64_t MAX_SEED_LEN = 32;
constexpr uint64_t MAX_SEEDS = 16;

/**
 * Maximum number of bytes a program may add to an account during a single realloc
 */
constexpr uint64_t MAX_PERMITTED_DATA_INCREASE = 1024 * 10;

/**
 * Minimum of signed integral types
 */
constexpr int8_t INT8_MIN = -128;
constexpr int16_t INT16_MIN = -32767-1;
constexpr int32_t INT32_MIN = -2147483647-1;
constexpr int64_t INT64_MIN = -9223372036854775807L-1;

/**
 * Maximum of signed integral types
 */
constexpr int8_t INT8_MAX = 127;
constexpr int16_t INT16_MAX = 32767;
constexpr int32_t INT32_MAX = 2147483647;
constexpr int64_t INT64_MAX = 9223372036854775807L;

/**
 * Maximum of unsigned integral types
 */
constexpr uint8_t UINT8_MAX = 255;
constexpr uint16_t UINT16_MAX = 65535;
constexpr uint32_t UINT32_MAX = 4294967295U;
constexpr uint64_t UINT64_MAX = 18446744073709551615UL;

} // namespace sol
