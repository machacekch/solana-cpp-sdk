#pragma once
/**
 * A structure for passing bytes to the sol_keccak256 and sol_sha256 hash functions (see sol_syscalls.h)
 */

#include <sol_int.h>

namespace sol {

struct Bytes {
    const uint8_t* addr;
    uint64_t len;
};

} // namespace sol
