#pragma once
/**
 * Compile-time conversion of Base58-encoded public keys to their 32-byte representation
 *
 * Example:
 * constexpr Pubkey clock_key = from_base58("SysvarC1ock11111111111111111111111111111111");
 *
 * It is the caller's responsibility to ensure the validity of the Base58 string.
 */

#include <sol_pubkey.h>

namespace sol {

//constexpr const char* map_to_base58 = "123456789ABCDEFGHJKLMNPQRSTUVWXYZabcdefghijkmnopqrstuvwxyz";
constexpr int8_t map_from_base58[256] = {
    -1,-1,-1,-1,-1,-1,-1,-1, -1,-1,-1,-1,-1,-1,-1,-1,
    -1,-1,-1,-1,-1,-1,-1,-1, -1,-1,-1,-1,-1,-1,-1,-1,
    -1,-1,-1,-1,-1,-1,-1,-1, -1,-1,-1,-1,-1,-1,-1,-1,
    -1, 0, 1, 2, 3, 4, 5, 6,  7, 8,-1,-1,-1,-1,-1,-1,
    -1, 9,10,11,12,13,14,15, 16,-1,17,18,19,20,21,-1,
    22,23,24,25,26,27,28,29, 30,31,32,-1,-1,-1,-1,-1,
    -1,33,34,35,36,37,38,39, 40,41,42,43,-1,44,45,46,
    47,48,49,50,51,52,53,54, 55,56,57,-1,-1,-1,-1,-1,
    -1,-1,-1,-1,-1,-1,-1,-1, -1,-1,-1,-1,-1,-1,-1,-1,
    -1,-1,-1,-1,-1,-1,-1,-1, -1,-1,-1,-1,-1,-1,-1,-1,
    -1,-1,-1,-1,-1,-1,-1,-1, -1,-1,-1,-1,-1,-1,-1,-1,
    -1,-1,-1,-1,-1,-1,-1,-1, -1,-1,-1,-1,-1,-1,-1,-1,
    -1,-1,-1,-1,-1,-1,-1,-1, -1,-1,-1,-1,-1,-1,-1,-1,
    -1,-1,-1,-1,-1,-1,-1,-1, -1,-1,-1,-1,-1,-1,-1,-1,
    -1,-1,-1,-1,-1,-1,-1,-1, -1,-1,-1,-1,-1,-1,-1,-1,
    -1,-1,-1,-1,-1,-1,-1,-1, -1,-1,-1,-1,-1,-1,-1,-1,
};

constexpr Pubkey from_base58(const char* encoded) {
    static_assert(sizeof(map_from_base58) == 256, "map_from_base58 must have 256 entries");

    Pubkey key;

    int input_index = 0;

    int length = 0;
    while (encoded[input_index]) {
        int carry = map_from_base58[(uint8_t)encoded[input_index]];
        //TODO: assert(carry != -1, "invalid base58 character");
        int i = 0;
        for (; (carry != 0 || i < length) && (i != 32); ++i) {
            carry += 58 * key.bytes[31 - i];
            key.bytes[31 - i] = carry % 256;
            carry /= 256;
        }
        //TODO: assert(carry == 0, "result does not fit into 32 bytes");
        length = i;
        ++input_index;
    }

    return key;
}

} // namespace sol
