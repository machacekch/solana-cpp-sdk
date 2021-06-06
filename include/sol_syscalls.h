#pragma once
/**
 * Declarations for Solana syscalls
 *
 * Calling these directly should rarely be necessary; the SDK provides higher-level abstractions with better type-safety for most.
 */

#include <sol_int.h>

namespace sol {

class AccountInfo;
class Bytes;
class Instruction;
class Pubkey;
class SignerSeed;
class SignerSeeds;

namespace sysvar {
class Clock;
class EpochSchedule;
class Rent;
} // namespace sysvar

namespace syscall {
extern "C" {

void sol_log_(const char*, uint64_t);
void sol_log_64_(uint64_t, uint64_t, uint64_t, uint64_t, uint64_t);
void sol_log_compute_units_();
void sol_log_pubkey(const Pubkey* pubkey);

void* sol_alloc_free_(uint64_t size, void* ptr);

void sol_panic_(const char*, uint64_t, uint64_t, uint64_t);

uint64_t sol_keccak256(
    const Bytes* bytes,
    int bytes_len,
    uint8_t* result // must hold 32 bytes
);

uint64_t sol_sha256(
    const Bytes* bytes,
    int bytes_len,
    uint8_t* result // must hold 32 bytes
);

uint64_t sol_create_program_address(
    const SignerSeed* seeds,
    int seeds_len,
    const Pubkey* program_id,
    Pubkey* program_address
);

uint64_t sol_try_find_program_address(
    const SignerSeed* seeds,
    int seeds_len,
    const Pubkey* program_id,
    Pubkey* program_address,
    uint8_t* bump_seed
);

uint64_t sol_invoke_signed_c(
  const Instruction* instruction,
  const AccountInfo* account_infos,
  int account_infos_len,
  const SignerSeeds* signers_seeds,
  int signers_seeds_len
);

uint64_t sol_get_clock_sysvar(sysvar::Clock* clock);
uint64_t sol_get_epoch_schedule_sysvar(sysvar::EpochSchedule* epoch_schedule);
uint64_t sol_get_rent_sysvar(sysvar::Rent* rent);

//TODO: test these once "add syscalls for memory operations" has been enabled.
// https://github.com/solana-labs/solana/pull/16447/files
void sol_memcpy_(void* dst, const void* src, uint64_t len);
void sol_memmove_(void* dst, const void* src, uint64_t len);
void sol_memcmp_(const void* left, const void* right, uint64_t len, int32_t* result);
void sol_memset_(void* dst, uint64_t byte, uint64_t len);

} // extern "C"
} // namespace syscall
} // namespace sol
