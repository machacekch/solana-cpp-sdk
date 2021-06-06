#pragma once
/**
 * Instructions to be executed in cross-program invocations (see sol_syscall_wrappers.h)
 *
 * An Instruction can be created from the underlying byte representation of an object (see sol_native.h), or from a byte array.
 *
 * Importantly, an Instruction does NOT copy, but only references the bytes.
 * It is the user's responsibility to ensure the validity of the referenced memory for as long as the Instruction is used.
 */

#include <sol_int.h>

namespace sol {

class AccountMeta;
class Pubkey;

class Instruction {
    const Pubkey* _program_id;     //TODO: Should this be const in the C SDK too? What about Pubkey* in other structs?
    const AccountMeta* _accounts;  /** Array of accounts to be passed to the called program */
    uint64_t _accounts_len;        /** Number of accounts */
    const uint8_t* _data;          /** Serialized parameters to be passed to the called program */
    uint64_t _data_len;            /** Number of bytes in data */

public:
    template<class T, uint64_t ACCOUNTS>
    Instruction(const Pubkey& program_id, const AccountMeta (&accounts)[ACCOUNTS], const T& data) :
        _program_id(&program_id),
        _accounts(accounts),
        _accounts_len(ACCOUNTS),
        _data(reinterpret_cast<const uint8_t*>(&data)),
        _data_len(sizeof(data)) {
    }

    template<uint64_t ACCOUNTS>
    Instruction(const Pubkey& program_id, const AccountMeta (&accounts)[ACCOUNTS], const void* data, uint64_t bytes) :
        _program_id(&program_id),
        _accounts(accounts),
        _accounts_len(ACCOUNTS),
        _data(reinterpret_cast<const uint8_t*>(data)),
        _data_len(bytes) {
    }
};

} // namespace sol
