# Solana C++ SDK
A header-only C++ SDK for on-chain programs (a.k.a. smart contracts) on the [Solana](https://solana.com/) blockchain.

# Running the examples
Install the [Solana Tool Suite](https://docs.solana.com/cli/install-solana-cli-tools) version 1.7.0 or higher. To update your existing installation run `solana-install init "v1.7.0"`. Be sure to switch to devnet for experimenting with the SDK: `solana config set -ud`.

Run `make` in this repository's root directory and follow the instructions to deploy the examples, e.g.:
```bash
$ solana program deploy dist/account_logger_example.so
```
Note the `Program Id` you're given when deploying a program; you'll need it for sending transactions. To that end, there are some simple Node.js scripts in the `client` directory, from which to call `npm install` to download the dependencies. Client scripts take the `Program Id` as the first argument, as well as additional arguments, depending on the script. For example:
```
$ npm run account_logger_example HX9t2pYjL1HZ9da1xTpWCgwaJRUqSCvgHs2RaA4stwsq
$ npm run user_registry_example 867RPtW9GzwCg3xWs9PjJaSTT7cR7LqGq8g79jKEh49W myname
```

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
invoke_signed(create_account_instruction, account_infos, {
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
This SDK is not officially supported by the Solana project. It should be considered alpha quality software at this point. Interfaces and implementation are subject to change.
