#pragma once
/**
 * Deserializing a program's input
 *
 * The call to deserialize() will look roughly the same in every program.
 * All parameters except for the first are out-parameters.
 *
 * Example:
 * #include <solana_cpp_sdk.h>
 * using namespace sol;
 * extern "C" uint64_t entrypoint(const uint8_t* input) {
 *     constexpr uint64_t MAX_ACCOUNTS = 4;
 *     AccountInfo accounts[MAX_ACCOUNTS];
 *     uint64_t accounts_given = 0;
 *     const uint8_t* instruction_data = nullptr;
 *     uint64_t instruction_data_len = 0;
 *     const Pubkey* program_id = nullptr;
 *
 *     if (!deserialize(input, accounts, accounts_given, instruction_data, instruction_data_len, program_id)) {
 *         return error::INVALID_ARGUMENT;
 *     }
 *
 *     const uint64_t ACCOUNTS_AVAILABLE = accounts_given < MAX_ACCOUNTS ? accounts_given : MAX_ACCOUNTS;
 *     log_array(accounts, ACCOUNTS_AVAILABLE);
 *     // ...
 *     return SUCCESS;
 * }
 */

#include <sol_limits.h>
#include <sol_memory.h>

namespace sol {

template<uint64_t MAX_ACCOUNTS_EXPECTED>
bool deserialize(
    const uint8_t* input,
    AccountInfo (&account_infos)[MAX_ACCOUNTS_EXPECTED],
    uint64_t& accounts_given,
    const uint8_t* (&instruction_data),
    uint64_t& instruction_data_len,
    const Pubkey* (&program_id)
) {
    if (input == nullptr) {
        return false;
    }
    accounts_given = *(uint64_t*)input;
    input += sizeof(uint64_t);

    for (int i = 0; i < accounts_given; ++i) {
        uint8_t dup_info = input[0];
        input += sizeof(uint8_t);

        if (i >= MAX_ACCOUNTS_EXPECTED) {
            if (dup_info == UINT8_MAX) {
                input += sizeof(uint8_t);
                input += sizeof(uint8_t);
                input += sizeof(uint8_t);
                input += 4; // padding
                input += sizeof(Pubkey);
                input += sizeof(Pubkey);
                input += sizeof(Lamports);
                uint64_t data_len = *(uint64_t*) input;
                input += sizeof(uint64_t);
                input += data_len;
                input += MAX_PERMITTED_DATA_INCREASE;
                input = aligned<8>(input);
                input += sizeof(Epoch);
            } else {
                input += 7; // padding
            }
            continue;
        }
        if (dup_info == UINT8_MAX) {
            account_infos[i]._is_signer = *(uint8_t*) input != 0;
            input += sizeof(uint8_t);

            account_infos[i]._is_writable = *(uint8_t*) input != 0;
            input += sizeof(uint8_t);

            account_infos[i]._executable = *(uint8_t*) input;
            input += sizeof(uint8_t);

            input += 4; // padding

            account_infos[i]._pubkey = (Pubkey*) input;
            input += sizeof(Pubkey);

            account_infos[i]._owner = (Pubkey*) input;
            input += sizeof(Pubkey);

            account_infos[i]._lamports = (Lamports*) input;
            input += sizeof(Lamports);

            // account data
            account_infos[i]._data_len = *(uint64_t*) input;
            input += sizeof(uint64_t);
            account_infos[i]._data = (uint8_t*) input;
            input += account_infos[i]._data_len;
            input += MAX_PERMITTED_DATA_INCREASE;
            input = aligned<8>(input);

            account_infos[i]._rent_epoch = *(Epoch*) input;
            input += sizeof(Epoch);
        } else {
            account_infos[i]._is_signer = account_infos[dup_info]._is_signer;
            account_infos[i]._is_writable = account_infos[dup_info]._is_writable;
            account_infos[i]._executable = account_infos[dup_info]._executable;
            account_infos[i]._pubkey = account_infos[dup_info]._pubkey;
            account_infos[i]._owner = account_infos[dup_info]._owner;
            account_infos[i]._lamports = account_infos[dup_info]._lamports;
            account_infos[i]._data_len = account_infos[dup_info]._data_len;
            account_infos[i]._data = account_infos[dup_info]._data;
            account_infos[i]._rent_epoch = account_infos[dup_info]._rent_epoch;
            input += 7; // padding
        }
    }

    instruction_data_len = *(uint64_t*) input;
    input += sizeof(uint64_t);
    instruction_data = input;
    input += instruction_data_len;

    program_id = (Pubkey*) input;
    input += sizeof(Pubkey);

    return true;
}

} // namespace sol
