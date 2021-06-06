#pragma once
/**
 * 32-byte public keys
 *
 * Operators for comparison and logging are provided.
 * Pubkeys can be created from their Base58 representation. (see sol_base58.h)
 *
 * Example:
 * Pubkey clock_key = from_base58("SysvarC1ock11111111111111111111111111111111");
 */

#include <sol_syscalls.h>
#include <sol_int.h>

namespace sol {

class Pubkey {
    friend bool operator==(const Pubkey& left, const Pubkey& right);
    friend bool operator<(const Pubkey& left, const Pubkey& right);
    friend constexpr Pubkey from_base58(const char* encoded);

    uint8_t bytes[32];

public:
    constexpr Pubkey() : bytes{} {
    }
};

bool operator==(const Pubkey& left, const Pubkey& right) {
    for (int i = 0; i != sizeof(Pubkey::bytes); ++i) {
        if (left.bytes[i] != right.bytes[i]) {
            return false;
        }
    }
    return true;
}

bool operator!=(const Pubkey& left, const Pubkey& right) {
    return !(left == right);
}

bool operator<(const Pubkey& left, const Pubkey& right) {
    for (int i = 0; i != sizeof(Pubkey::bytes); ++i) {
        if (left.bytes[i] != right.bytes[i]) {
            return left.bytes[i] < right.bytes[i];
        }
    }
    return false;
}

void log(const Pubkey& pubkey) {
    syscall::sol_log_pubkey(&pubkey);
}

} // namespace sol
