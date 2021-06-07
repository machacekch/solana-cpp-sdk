#pragma once
/**
 * Classes for signing with seeds
 *
 * A SignerSeed can be created from a string, from the underlying byte representation of an object, or from a byte array.
 *
 * Example:
 * auto pda = find_pda(program_id, "SignerSeed from string", SignerSeed(pubkey), SignerSeed(pointer, byte_count));
 *
 * Importantly, a SignerSeed does NOT copy, but only references the bytes.
 * It is the user's responsibility to ensure the validity of the referenced memory for as long as the SignerSeed is used.
 *
 * The SignerSeeds class references an array of SignerSeed objects.
 * To sign for multiple accounts, multiple SignerSeeds can be passed in a cross-program invocation. (see sol_syscall_wrappers.h)
 *
 * Example:
 * invoke_signed(create_account_instruction, account_infos, {
 *     {{"bank", SignerSeed(payer_pda.bump_seed)}},  // sign for payment
 *     {{"1234", SignerSeed(created_pda.bump_seed)}} // sign for the new account
 * });
 */

#include <sol_int.h>
#include <sol_limits.h>
#include <sol_syscall_wrappers.h>

namespace sol {

// Traits for specializing seed construction from objects.
template<class T>
struct SeedConstructor {
    // returns the address of the byte-array used as the seed
    static constexpr const uint8_t* addr(const T& object) {
        return reinterpret_cast<const uint8_t*>(&object);
    }

    // returns the length of the byte-array used as the seed
    static constexpr uint64_t len(const T& object) {
        return sizeof(object);
    }
};

// prevent seed construction from addresses as this is almost certainly not intended
template<class T>
struct SeedConstructor<T*> {
};

// SignerSeed objects reference (but don't own) byte-arrays that are to be passed as seeds to calls such as find_pda().
// It is the user's responsibility to ensure the validity of the referenced memory for as long as the SignerSeed is used.
class SignerSeed {
    const uint8_t* addr;
    uint64_t len;

public:
    SignerSeed(const SignerSeed& other) = default;

    SignerSeed(const void* addr, uint64_t bytes) :
        addr(reinterpret_cast<const uint8_t*>(addr)),
        len(bytes) {
        if (len > MAX_SEED_LEN) {
            log("ERROR: Seed too long");
            sol_panic();
        }
    }

    // creates a seed from a string, ignoring the trailing \0 character
    SignerSeed(const char* string) :
        addr(reinterpret_cast<const uint8_t*>(string)),
        len(strlen(string)) {
        if (len > MAX_SEED_LEN) {
            log("ERROR: Seed too long:");
            log(string);
            sol_panic();
        }
    }

    // creates a seed from the underlying byte-representation of an object
    template<class T>
    explicit SignerSeed(const T& object) :
        addr(SeedConstructor<T>::addr(object)),
        len(SeedConstructor<T>::len(object)) {
        if (len > MAX_SEED_LEN) {
            log("ERROR: Seed too long");
            sol_panic();
        }
    }

    //TODO: Decide whether to delete the move constructor:
    // There's a danger that the user passes a temporary that is destroyed before the function call using the SignerSeed is executed. This can be prevented by deleting this constructor. However, that also prevents the user from passing a temporary as part of the function call, which is more convenient than creating an object on the stack first.
    //
    // prevent temporaries as arguments: the object must outlive the SignerSeed
    //template<class T>
    //explicit SignerSeed(const T&& object) = delete;
};

// Represents an array of SignerSeed objects.
class SignerSeeds {
    const SignerSeed* addr;
    uint64_t len;

public:
    template<uint64_t ARRAY_SIZE>
    SignerSeeds(const SignerSeed (&seed_array)[ARRAY_SIZE]) :
        addr(seed_array),
        len(ARRAY_SIZE) {
        static_assert(ARRAY_SIZE <= MAX_SEEDS, "too many seeds given");
    }
};

} // namespace sol
