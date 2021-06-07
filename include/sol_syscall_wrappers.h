#pragma once
/**
 * Convenient wrappers around some syscalls, with better type-safety
 *
 * find_pda is a variadic function that takes any number of SignerSeed objects to return both a program derived address and a bump seed.
 * (In a cross-program invocation, the bump seed must be appended to the other seeds.)
 *
 * Example:
 * FoundPDA pda = find_pda(program_id, "user account prefix", SignerSeed(username));
 * log("pubkey:", pda.pubkey, "bump seed:", pda.bump_seed);
 *
 * Similarly, create_pda takes any number of seeds and returns in is_valid whether the derived key can be signed for in a cross-program invocation.
 *
 * invoke_signed does the cross-program invocation.
 *
 * Example:
 * invoke_signed(create_account_instruction, account_infos, {
 *     {{"bank", SignerSeed(payer_pda.bump_seed)}},  // sign for payment
 *     {{"1234", SignerSeed(created_pda.bump_seed)}} // sign for the new account
 * });
 */

#include <sol_int.h>
#include <sol_status_codes.h>
#include <sol_syscalls.h>

#define sol_panic() ::sol::syscall::sol_panic_(__FILE__, sizeof(__FILE__), __LINE__, 0)

namespace sol {

struct FoundPDA {
    Pubkey pubkey;
    uint8_t bump_seed;
};

template<typename... Ts>
FoundPDA find_pda(const Pubkey& program_id, const Ts&... seeds) {
    SignerSeed seed_array[sizeof...(Ts)] = { seeds... };
    FoundPDA result;
    if (syscall::sol_try_find_program_address(seed_array, sizeof...(Ts), &program_id, &result.pubkey, &result.bump_seed) != SUCCESS) {
        log("ERROR: Could not create a program derived address.");
        sol_panic();
    }
    return result;
}

struct CreatedPDA {
    Pubkey pubkey;
    bool is_valid;
};

template<typename... Ts>
CreatedPDA create_pda(const Pubkey& program_id, const Ts&... seeds) {
    SignerSeed seed_array[sizeof...(Ts)] = { seeds... };
    CreatedPDA result;
    result.is_valid = syscall::sol_create_program_address(seed_array, sizeof...(Ts), &program_id, &result.pubkey) == SUCCESS;
    return result;
}

template<uint64_t ACCOUNTS, uint64_t SIGNERS>
uint64_t invoke_signed(
  const Instruction& instruction,
  const AccountInfo (&account_info_array)[ACCOUNTS],
  const SignerSeeds (&signer_seeds_array)[SIGNERS]
) {
    return syscall::sol_invoke_signed_c(&instruction, account_info_array, ACCOUNTS, signer_seeds_array, SIGNERS);
}

template<uint64_t ACCOUNTS>
uint64_t invoke(
  const Instruction& instruction,
  const AccountInfo (&account_info_array)[ACCOUNTS]
) {
    return syscall::sol_invoke_signed_c(&instruction, account_info_array, ACCOUNTS, nullptr, 0);
}

} // namespace sol
