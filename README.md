# Solana C++ SDK
A header-only C++ SDK for on-chain programs (a.k.a. smart contracts) on the [Solana](https://solana.com/) blockchain.

# Running the examples
Install the [Solana Tool Suite](https://docs.solana.com/cli/install-solana-cli-tools) or update your existing installation by running `solana-install init "v1.7.0"`. Version 1.7.0 or later is required to use all the SDK's features.

# Highlights
These are some benefits of using the C++ SDK over writing plain C.

### Type safety
* The SDK rarely requires users to operate with raw memory.
* It enforces const-correctness throughout.
* References are preferred over pointers.

### Compile-time Base58 conversion
Public keys can be created from their Base58 encoding without any run-time overhead:
```c++
constexpr Pubkey clock_key = from_base58("SysvarC1ock11111111111111111111111111111111");
```

### Sysvars via syscalls
A recent feature allowing programs to access [Sysvar Cluster Data](https://docs.solana.com/developing/runtime-facilities/sysvars) on-the-fly without the need to specify those accounts in the transaction:
```c++
sysvar::Clock clock;
sysvar::Rent rent;
if (account_info.lamports() < rent.minimum_balance(account_info.data_len())) {
    log("ERROR: Account", account_info.pubkey(), "is not rent-exempt at time", clock.unix_timestamp());
    sol_panic();
}
```
The SDK currently supports the `Clock`, `EpochSchedule` and `Rent` sysvars. They can also be read from the sysvar accounts by passing the `AccountInfo` to the constructor.

### SystemInstruction constructors
[Native Programs](https://docs.solana.com/developing/runtime-facilities/programs) are commonly called via cross-program invocations. The SDK provides constructors for some of the [instructions supported by the System Program](https://docs.rs/solana-sdk/1.7.0/solana_sdk/system_instruction/enum.SystemInstruction.html):
```c++
Pubkey system_pubkey = native::SystemProgram::pubkey();
native::SystemProgram::CreateAccountParams params(Rent().minimum_balance(data_len), data_len, program_id);
Instruction create_account_instruction(system_pubkey, account_metas, params);
```

### Easier handling of seeds
Program-derived addresses are obtained by calling the variadic `find_pda` or `create_pda` functions:
```c++
FoundPDA pda = find_pda(program_id, "user account prefix", SignerSeed(username));
log("pubkey:", pda.pubkey, "bump seed:", pda.bump_seed);
```

For a cross-program invocation, arrays of seeds are passed to the `invoke_signed` function as follows:
```c++
invoke_signed(create_account_instruction, {system_program, payer, created_account}, {
    {{"bank", SignerSeed(payer_pda.bump_seed)}},  // sign for payment
    {{"1234", SignerSeed(created_pda.bump_seed)}} // sign for the new account
});
```

### Convenient logging
A variadic function with overloads for SDK types:
```c++
log("Logging account", account_info, "at time", sysvar::Clock());
```

### Namespaces
With few exceptions such as the `sol_panic` macro, names are under the `sol::` namespace and don't pollute the global namespace.

# Disclaimer
This SDK is not officially supported or endorsed by the Solana project. It should be considered alpha quality software at this point. Interfaces and implementation are subject to change.
