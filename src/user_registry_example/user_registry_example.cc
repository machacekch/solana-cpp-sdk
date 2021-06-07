/**
 * Registering a username of (up to) 32 bytes length
 *
 * This example shows a way to track usernames and guarantee their uniqueness.
 * Rather than using an explicit hash table, the username serves as the seed from which a program derived address (PDA) is created.
 * The program refuses the registration unless the so-derived account is owned by the System Program (i.e. not in use yet).
 * It then checks if the account is rent-exempt and assigns it to itself, storing the caller's public key in the account.
 * Finally, a cross-program invocation (CPI) is made to the Memo Program, described at https://spl.solana.com/memo, to ensure it is valid UTF-8.
 *
 * The program will log the PDA where the signer's pubkey is stored. The latter is logged via the Memo CPI.
 * If you'd like to make sure the pubkey was stored correctly, send a getAccountInfo request for the PDA, asking for a base58 encoding:
 * https://docs.solana.com/developing/clients/jsonrpc-api#getaccountinfo
 */

#include <solana_cpp_sdk.h>

using namespace sol;

uint64_t register_with_username(
    const Pubkey& program_id,
    const uint8_t* username,
    const uint64_t username_len,
    AccountInfo (&accounts)[4]
) {
    const AccountInfo& signer = accounts[0];
    AccountInfo& new_account = accounts[1];
    const AccountInfo& system = accounts[2];
    const AccountInfo& memo = accounts[3];

    // check to see if the executables are the ones we expect
    if (system.pubkey() != native::SystemProgram::pubkey()) {
        log("ERROR: Expected System Program but got:", system.pubkey());
        sol_panic();
    }
    if (memo.pubkey() != from_base58("MemoSq4gqABAXKb96qnH8TysNcWxMyWCqXgDLGmfcHr")) {
        log("ERROR: Expected Memo Program but got:", memo.pubkey());
        sol_panic();
    }

    // make sure the user actually signed
    if (!signer.is_signer()) {
        log("ERROR: Missing signature for account:", signer.pubkey());
        sol_panic();
    }

    // this check ensures the account is unused
    if (new_account.owner() != native::SystemProgram::pubkey()) {
        log("ERROR: Account already in use:", new_account.pubkey());
        sol_panic();
    }

    // verify that the new account's address was derived with the username as seed
    FoundPDA pda = find_pda(program_id, SignerSeed(username, username_len));
    log("PDA:", pda.pubkey, "bump seed:", pda.bump_seed);
    if (new_account.pubkey() != pda.pubkey) {
        log("ERROR: Could not derive account address with the given username.");
        sol_panic();
    }

    // check to see if it's rent-exempt
    if (new_account.lamports() < sysvar::Rent().minimum_balance(sizeof(Pubkey))) {
        log("ERROR: Account is not rent-exempt.");
        sol_panic();
    }

    // allocate space to store the signer's pubkey
    AccountMeta allocate_metas[] = {{&new_account.pubkey(), true, true}};
    native::SystemProgram::AllocateParams allocate_params(sizeof(Pubkey));
    const Instruction allocate_instruction(system.pubkey(), allocate_metas, allocate_params);

    if (invoke_signed(allocate_instruction, accounts, {
        {{SignerSeed(username, username_len), SignerSeed(pda.bump_seed)}}
    }) != SUCCESS) {
        log("ERROR: Could not allocate space.");
        sol_panic();
    }

    // assign the account to the program itself
    AccountMeta assign_metas[] = {{&new_account.pubkey(), true, true}};
    native::SystemProgram::AssignParams assign_params(program_id);
    const Instruction assign_instruction(system.pubkey(), assign_metas, assign_params);

    if (invoke_signed(assign_instruction, accounts, {
        {{SignerSeed(username, username_len), SignerSeed(pda.bump_seed)}}
    }) != SUCCESS) {
        log("ERROR: Could not take ownership of the account.");
        sol_panic();
    }

    // copy the signer's pubkey to the account
    memcpy(new_account.data(), &signer.pubkey(), sizeof(Pubkey));

    // let's make sure the username is valid UTF-8
    // since the C++ SDK doesn't provide any helpers for constructing Memo Program parameters, we have to do so manually
    // see https://docs.rs/spl-memo/3.0.1/src/spl_memo/lib.rs.html#30-39
    AccountMeta memo_metas[] = {{&signer.pubkey(), false, true}};
    const Instruction memo_instruction(memo.pubkey(), memo_metas, username, username_len);

    if (invoke(memo_instruction, accounts) != SUCCESS) {
        log("ERROR: Username rejected. Is it valid UTF-8?");
        sol_panic();
    }

    return SUCCESS;
}

extern "C" uint64_t entrypoint(const uint8_t* input) {
    log("C++ program entrypoint");

    // 4 accounts are required:
    // 0. [SIGNER] The user who's registering
    // 1. [WRITE] New account to store the user's public key in. Must already be funded.
    // 2. [] System program to call allocate() and assign()
    // 3. [] Memo program for UTF-8 validation
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

    const uint64_t ACCOUNTS_AVAILABLE = accounts_given < MAX_ACCOUNTS ? accounts_given : MAX_ACCOUNTS;
    if (ACCOUNTS_AVAILABLE != 4) {
        return error::NOT_ENOUGH_ACCOUNT_KEYS;
    }

    return register_with_username(
        *program_id,
        instruction_data,
        instruction_data_len,
        accounts
    );
}
