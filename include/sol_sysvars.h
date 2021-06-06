#pragma once
/**
 * Sysvars for accessing cluster state
 *
 * These are constructed from an AccountInfo, or via a syscall when the parameterless constructor is used.
 * The object contains a copy of the data in either case.
 *
 * Example:
 * log(sysvar::Clock().unix_timestamp());
 *
 * TODO: Add support for the other sysvars.
 */

#include <sol_account.h>
#include <sol_base58.h>
#include <sol_int.h>
#include <sol_logging.h>
#include <sol_memory.h>
#include <sol_syscalls.h>

namespace sol::sysvar {

class Clock {
    uint64_t _slot;                    // the current network/bank Slot
    int64_t _epoch_start_timestamp;    // the timestamp of the first Slot in this Epoch
    uint64_t _epoch;                   // the bank Epoch
    uint64_t _leader_schedule_epoch;   // the future Epoch for which the leader schedule has most recently been calculated
    int64_t _unix_timestamp;           // originally computed from genesis creation time and network time in slots (drifty); corrected using validator timestamp oracle as of timestamp_correction and timestamp_bounding features

public:
    static constexpr Pubkey pubkey() {
        return from_base58("SysvarC1ock11111111111111111111111111111111");
    }

    Clock() {
        syscall::sol_get_clock_sysvar(this);
    }

    Clock(const AccountInfo& account) {
        if (account.pubkey() != pubkey()) {
            log("ERROR: Wrong account passed to Clock constructor:");
            log(account.pubkey());
            sol_panic();
        }
        memcpy(this, account.data(), sizeof(*this));
    }

    uint64_t slot() const {
        return _slot;
    }

    int64_t epoch_start_timestamp() const {
        return _epoch_start_timestamp;
    }

    uint64_t epoch() const {
        return _epoch;
    }

    uint64_t leader_schedule_epoch() const {
        return _leader_schedule_epoch;
    }

    int64_t unix_timestamp() const {
        return _unix_timestamp;
    }
};

class EpochSchedule { //TODO: needs testing
    uint64_t _slots_per_epoch;
    uint64_t _leader_schedule_slot_offset;
    bool _warmup;
    Epoch _first_normal_epoch;
    Slot _first_normal_slot;

public:
    static constexpr Pubkey pubkey() {
        return from_base58("SysvarEpochSchedu1e111111111111111111111111");
    }

    EpochSchedule() {
        syscall::sol_get_epoch_schedule_sysvar(this);
        _warmup = *reinterpret_cast<uint8_t*>(&_warmup); // to not rely on a specific numeric representation of "true"
    }

    EpochSchedule(const AccountInfo& account) {
        if (account.pubkey() != pubkey()) {
            log("ERROR: Wrong account passed to EpochSchedule constructor:");
            log(account.pubkey());
            sol_panic();
        }
        memcpy(this, account.data(), sizeof(*this));
        _warmup = *reinterpret_cast<uint8_t*>(&_warmup); // to not rely on a specific numeric representation of "true"
    }

    uint64_t slots_per_epoch() const {
        return _slots_per_epoch;
    }

    uint64_t leader_schedule_slot_offset() const {
        return _leader_schedule_slot_offset;
    }

    bool warmup() const {
        return _warmup;
    }

    Epoch first_normal_epoch() const {
        return _first_normal_epoch;
    }

    Slot first_normal_slot() const {
        return _first_normal_slot;
    }
};

class Rent {
    Lamports _lamports_per_byte_year;  // rental rate
    double _exemption_threshold;       // exemption threshold, in years
    uint8_t _burn_percent;             // what percentage of collected rent is to be destroyed

    static constexpr uint64_t account_storage_overhead = 128;

public:
    static constexpr Pubkey pubkey() {
        return from_base58("SysvarRent111111111111111111111111111111111");
    }

    Rent() {
        syscall::sol_get_rent_sysvar(this);
    }

    Rent(const AccountInfo& account) {
        if (account.pubkey() != pubkey()) {
            log("ERROR: Wrong account passed to Rent constructor:");
            log(account.pubkey());
            sol_panic();
        }
        memcpy(this, account.data(), sizeof(*this));
    }

    Lamports lamports_per_byte_year() const {
        return _lamports_per_byte_year;
    }

    double exemption_threshold() const {
        return _exemption_threshold;
    }

    uint8_t burn_percent() const {
        return _burn_percent;
    }

    Lamports minimum_balance(uint64_t data_len) const {
        return Lamports((account_storage_overhead + data_len) * lamports_per_byte_year() * exemption_threshold());
    }
};

void log(const Clock& clock) {
    syscall::sol_log_64_(
        clock.slot(),
        clock.epoch_start_timestamp(),
        clock.epoch(),
        clock.leader_schedule_epoch(),
        clock.unix_timestamp()
    );
}

void log(const EpochSchedule& epoch_schedule) {
    syscall::sol_log_64_(
        epoch_schedule.slots_per_epoch(),
        epoch_schedule.leader_schedule_slot_offset(),
        epoch_schedule.warmup(),
        epoch_schedule.first_normal_epoch(),
        epoch_schedule.first_normal_slot()
    );
}

void log(const Rent& rent) {
    double exemption_threshold = rent.exemption_threshold(); // reinterpreted as uint64_t for logging
    syscall::sol_log_64_(
        rent.lamports_per_byte_year(),
        *reinterpret_cast<const uint64_t*>(&exemption_threshold),
        rent.burn_percent(),
        0,
        0
    );
}

} // namespace sol::sysvar
