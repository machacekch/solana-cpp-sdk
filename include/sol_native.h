#pragma once
/**
 * Interfaces to native programs provided by the Solana runtime
 *
 * Example:
 * Pubkey system_pubkey = native::SystemProgram::pubkey();
 * native::SystemProgram::CreateAccountParams params(Rent().minimum_balance(data_len), data_len, program_id);
 * Instruction instruction(system_pubkey, account_metas, params);
 *
 * TODO: Only (some) instructions of the System Program are supported at this time. Others are to be added.
 */

#include <sol_account.h>
#include <sol_base58.h>
#include <sol_int.h>

namespace sol::native {

class SystemProgram {
public:
    static constexpr Pubkey pubkey() {
        return from_base58("11111111111111111111111111111111");
    }

    /// Create a new account
    ///
    /// # Account references
    ///   0. [WRITE, SIGNER] Funding account
    ///   1. [WRITE, SIGNER] New account
    class __attribute__((packed)) CreateAccountParams {
        uint32_t tag;
        Lamports lamports;
        uint64_t bytes;
        Pubkey owner;
    
    public:
        CreateAccountParams(Lamports lamports, uint64_t bytes, const Pubkey& owner) :
            tag(0),
            lamports(lamports),
            bytes(bytes),
            owner(owner) {
        }
    };
    static_assert(sizeof(CreateAccountParams) == 4 + 8 + 8 + 32);

    /// Assign account to a program
    ///
    /// # Account references
    ///   0. [WRITE, SIGNER] Assigned account public key
    class __attribute__((packed)) AssignParams {
        uint32_t tag;
        Pubkey owner;
    
    public:
        AssignParams(const Pubkey& owner) :
            tag(1),
            owner(owner) {
        }
    };
    static_assert(sizeof(AssignParams) == 4 + 32);

    /// Transfer lamports
    ///
    /// # Account references
    ///   0. [WRITE, SIGNER] Funding account
    ///   1. [WRITE] Recipient account
    class __attribute__((packed)) TransferParams {
        uint32_t tag;
        Lamports lamports;
    
    public:
        TransferParams(Lamports lamports) :
            tag(2),
            lamports(lamports) {
        }
    };
    static_assert(sizeof(TransferParams) == 4 + 8);

    //TODO: CreateAccountWithSeed with tag 3

    /// Consumes a stored nonce, replacing it with a successor
    ///
    /// # Account references
    ///   0. [WRITE] Nonce account
    ///   1. [] RecentBlockhashes sysvar
    ///   2. [SIGNER] Nonce authority
    class __attribute__((packed)) AdvanceNonceAccountParams {
        uint32_t tag;
    
    public:
        AdvanceNonceAccountParams() :
            tag(4) {
        }
    };
    static_assert(sizeof(AdvanceNonceAccountParams) == 4);

    /// Withdraw funds from a nonce account
    ///
    /// # Account references
    ///   0. [WRITE] Nonce account
    ///   1. [WRITE] Recipient account
    ///   2. [] RecentBlockhashes sysvar
    ///   3. [] Rent sysvar
    ///   4. [SIGNER] Nonce authority
    class __attribute__((packed)) WithdrawNonceAccountParams {
        uint32_t tag;
        Lamports lamports;
    
    public:
        WithdrawNonceAccountParams(Lamports lamports) :
            tag(5),
            lamports(lamports) {
        }
    };
    static_assert(sizeof(WithdrawNonceAccountParams) == 4 + 8);

    /// Drive state of Uninitalized nonce account to Initialized, setting the nonce value
    ///
    /// # Account references
    ///   0. [WRITE] Nonce account
    ///   1. [] RecentBlockhashes sysvar
    ///   2. [] Rent sysvar
    class __attribute__((packed)) InitializeNonceAccountParams {
        uint32_t tag;
        Pubkey authority;
    
    public:
        InitializeNonceAccountParams(const Pubkey& authority) :
            tag(6),
            authority(authority) {
        }
    };
    static_assert(sizeof(InitializeNonceAccountParams) == 4 + 32);

    /// Change the entity authorized to execute nonce instructions on the account
    ///
    /// # Account references
    ///   0. [WRITE] Nonce account
    ///   1. [SIGNER] Nonce authority
    class __attribute__((packed)) AuthorizeNonceAccountParams {
        uint32_t tag;
        Pubkey authority;
    
    public:
        AuthorizeNonceAccountParams(const Pubkey& authority) :
            tag(7),
            authority(authority) {
        }
    };
    static_assert(sizeof(AuthorizeNonceAccountParams) == 4 + 32);

    /// Allocate space in a (possibly new) account without funding
    ///
    /// # Account references
    ///   0. [WRITE, SIGNER] New account
    class __attribute__((packed)) AllocateParams {
        uint32_t tag;
        uint64_t bytes;
    
    public:
        AllocateParams(uint64_t bytes) :
            tag(8),
            bytes(bytes) {
        }
    };
    static_assert(sizeof(AllocateParams) == 4 + 8);

    //TODO: AllocateWithSeed with tag 9

    //TODO: AssignWithSeed with tag 10

    //TODO: TransferWithSeed with tag 11
};

} // namespace sol::native
