/**
 * Deserializes a program's input and logs the AccountInfos
 *
 * This example demonstrates the minimal plumbing necessary for a C++ program.
 */

#include <solana_cpp_sdk.h>

using namespace sol;

// be sure to declare the entrypoint as extern "C"
extern "C" uint64_t entrypoint(const uint8_t* input) {
    log("C++ program entrypoint");

    // set MAX_ACCOUNTS to the maximum number of accounts your program should handle
    constexpr uint64_t MAX_ACCOUNTS = 4;

    // these variables will be set by the call to deserialize() below
    AccountInfo accounts[MAX_ACCOUNTS];
    uint64_t accounts_given = 0;
    const uint8_t* instruction_data = nullptr;
    uint64_t instruction_data_len = 0;
    const Pubkey* program_id = nullptr;

    // all parameters except for the first are out-parameters
    if (!deserialize(input, accounts, accounts_given, instruction_data, instruction_data_len, program_id)) {
        return error::INVALID_ARGUMENT;
    }

    // ACCOUNTS_AVAILABLE can be smaller than MAX_ACCOUNTS if the caller passed fewer accounts
    const uint64_t ACCOUNTS_AVAILABLE = accounts_given < MAX_ACCOUNTS ? accounts_given : MAX_ACCOUNTS;

    log_array(accounts, ACCOUNTS_AVAILABLE);

    // add custom program code here

    return SUCCESS;
}
