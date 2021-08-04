#pragma once
/**
 * Classes for working with accounts
 *
 * AccountInfo is what the instruction's accounts are deserialized into. (see sol_serialization.h)
 * AccountMeta sets the is_writable and is_signer flags in cross-program invocations. (see sol_instruction.h)
 */

#include <sol_int.h>
#include <sol_logging.h>
#include <sol_pubkey.h>
#include <sol_syscalls.h>

namespace sol {

class AccountInfo;
class Pubkey;

class AccountInfo {
    template<uint64_t MAX_ACCOUNTS_EXPECTED>
    friend bool deserialize(
        const uint8_t* input,
        AccountInfo (&account_infos)[MAX_ACCOUNTS_EXPECTED],
        uint64_t& accounts_given,
        const uint8_t* (&instruction_data),
        uint64_t& instruction_data_len,
        const Pubkey* (&program_id)
    );

    Pubkey* _pubkey;      /** Public key of the account */
    Lamports* _lamports;  /** Number of lamports owned by this account */
    uint64_t _data_len;   /** Length of data in bytes */
    uint8_t* _data;       /** On-chain data within this account */
    Pubkey* _owner;       /** Program that owns this account */
    Epoch _rent_epoch;    /** The epoch at which this account will next owe rent */
    bool _is_signer;      /** Transaction was signed by this account's key? */
    bool _is_writable;    /** Is the account writable? */
    bool _executable;     /** This account's data contains a loaded program (and is now read-only) */

public:
    const Pubkey& pubkey() const {
        return *_pubkey;
    }

    const Lamports& lamports() const {
        return *_lamports;
    }

    Lamports& lamports() {
        return *_lamports;
    }

    const uint64_t& data_len() const {
        return _data_len;
    }

    const uint8_t* data() const {
        return _data;
    }

    uint8_t* data() {
        return _data;
    }

    const Pubkey& owner() const {
        return *_owner;
    }

    const Epoch& rent_epoch() const {
        return _rent_epoch;
    }

    bool is_signer() const {
        return _is_signer;
    }

    bool is_writable() const {
        return _is_writable;
    }

    bool executable() const {
        return _executable;
    }
};

struct AccountMeta {
    const Pubkey* pubkey;
    bool is_writable;
    bool is_signer;
};

inline void log(const AccountInfo& account) {
    log(account.pubkey());
    log("  - is_signer, is_writable, executable, lamports, rent_epoch");
    syscall::sol_log_64_(account.is_signer(), account.is_writable(), account.executable(), account.lamports(), account.rent_epoch());
    log("  - owner", account.owner());
    log("  - data");
    const uint8_t* data = account.data();
    for (uint64_t i = 0; i + 3 < account.data_len(); i += 4) {
        syscall::sol_log_64_(i, data[i], data[i + 1], data[i + 2], data[i + 3]);
    }
    const uint64_t i = (account.data_len() / 4) * 4;
    const uint64_t remainder = account.data_len() % 4;
    if (remainder != 0) {
        syscall::sol_log_64_(i, data[i], remainder > 1 ? data[i + 1] : 0, remainder > 2 ? data[i + 2] : 0, remainder > 3 ? data[i + 3] : 0);
    }
}

} // namespace sol
