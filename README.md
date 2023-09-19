<br />
<p align="center">
  <a href="https://arbitrum.io/">
    <img src="https://arbitrum.io/assets/stylus/stylus_with_paint_bg.png" alt="Logo" width="100%">
  </a>

  <h3 align="center">The Stylus SDK</h3>

  <p align="center">
    <a href="https://developer.arbitrum.io/"><strong>C/C++ contracts on Arbitrum »</strong></a>
    <br />
  </p>
</p>

## General

The C/C++ SDK allows you to take full controll of the underline web-assembly executed in your smart contract.

## Required Tools

The Stylus VM executes WebAssembly, so you'll need a C/C++ compiler with support for wasm32 targets. Support for this varies, so some users may have to build `clang` or `gcc` from source. Your package manager may also include a compatible version.

We suggest using these tools:

* [`llvm`](https://releases.llvm.org/) Must include clang and have webassembly support enabled with bulk-memory option. Make sure that clang accepts --target=wasm32 and that llvm ships with wasm-ld binary. Availability varies between distributions, but many package managers have these in a reasonable configuration, defined as "llvm" and/or "clang" packages.
* [`cargo-stylus`](https://github.com/OffchainLabs/cargo-stylus) is used to generate c-code, cgeck and deploy contracts. We do not require rust support for wasm platforms.
* make, git

## C/C++ SDK library

`The SDK is not audited, and not stable. API of future versions might be incompatible with the one declared here.`

| Header                 | Info                          |
|:-----------------------|:------------------------------|
| [`stylus_types.h`](include/stylus_types.h) | Types used by the wasm entrypoint to define return values from stylus |
| [`stylus_entry.h`](include/stylus_entry.h) | Includes only used by the entry-point to a stylus smart contract |
| [`hostio.h`](include/hostio.h) | Functions supplied by the stylus environment to change and access the VM state (see Host I/O) |
| [`stylus_debug.h`](include/stylus_debug.h) | Host-ios that can oonly be used by a debug-enabled node. Best way to get a debug enabled node is to [run it locally](https://docs.arbitrum.io/stylus/how-tos/local-stylus-dev-node) |
| [`bebi.h`](include/bebi.h) | Tools for handling Big Endian Big Integers in wasm-32 |
| [`storage.h`](include/storage.h) | Provides utils to access contract storage |
| [`stylus_utils.h`](include/stylus_utils.h) | Higher-level utils that might help smart contract developers |
| [`string.h`](include/string.h) | Minimal and partial implementation of the standard interface |
| [`stdlib.h`](include/stdlib.h) | Minimal and partial implementation of the standard interface |

## Examples

The library includes two examples. Each example contais a makefile that can build the wasm from source using the command "make". The examples are annotated, and users are encouraged to read through the code.

### siphash

Shows example for a "precompile-like" compute only smart contract that processes input bytes and returns their hash. This examples uses very little of the sdk library.

### erc20

Shows example for an erc20-like smart contract. This examples uses the library as well as c-code generation capabilities of cargo-stylus.

## Host I/Os

[`include/hostios.h`](hostios.h). There you can call VM hooks directly, which allows you to do everything from looking up the current block number to calling other contracts.

For example, the VM provides an efficient implementation of [keccak256][keccak256] via
```c
void native_keccak256(const uint8_t * bytes, size_t len, uint8_t * output)
```

Unlike with the Rust SDK, however, you will have to work with raw pointers and deserialize arguments manually. This makes [`stylus.h`](stylus.h) an ideal environment for bytes-in bytes-out programming, but not general smart contract development.

For a comprehensive list of hostios, please see [The Host I/O Reference][hostios].

[hostios]: TODO
[keccak256]: https://en.wikipedia.org/wiki/SHA-3
[siphash]: examples/siphash/main.c

## Notes about using C to build wasm32

### Clang flags

The table below includes `clang` flags commonly used to build Stylus contracts. The [siphash][siphash] example uses most of the following, and is a great starting point for programs that opt out of the standard library.

| Flag                    | Info                                                          | Optional |
|:------------------------|---------------------------------------------------------------|:---------|
| --target=wasm32         | compile to wasm                                               |          |
| --no-standard-libraries | opt out of the stdandard library                              | ✅       |
| -mbulk-memory           | enable bulk-memory operations (accelerates memset and memcpy) | ✅       |
| -O2 / -O3 / -Oz         | optimize for speed or size                                    | ✅       |
| --no-entry              | let Stylus decide the entrypoint                              |          |
| --stack-first           | puts the shadow-stack at the beginning of the memory          | ✅       |
| -z stack-size=...       | sets size for the shadow-stack                                | ✅       |

The easiest way to deploy your C or C++ program is to use the [Cargo Stylus CLI tool][cargo], which has a `--wasm-file-path` flag that accepts arbitrary WASMs.
```sh
cargo stylus deploy --wasm-file-path <wasm> --endpoint <rpc> --private-key-path <secret>
```

### Performance
C binaries are both small and very efficient. The [`siphash`][siphash] example is only **609 bytes** onchain and costs **22 gas** to execute a 32-byte input. By contrast, 22 gas only buys 7 ADD instructions in Solidity.

How did we achieve this efficiency? All we had to do was Google for an example siphash program and add a simple entrypoint. In the Stylus model, you can deploy highly-optimized and thouroughly-audited, industry-standard reference implementations as-is. With the Stylus SDK, cryptography, algorithms, and other high-compute applications are both straightforward and economically viable.

## Roadmap

Stylus is currently testnet-only and not recommended for production use. This will change as we complete an audit and add additional features.

Arbitrum [Orbit L3s][Orbit] may opt into Stylus at any time. Arbitrum One and Arbitrum Nova will upgrade to Stylus should the DAO vote for it.

If you'd like to be a part of this journey, join us in the `#stylus` channel on [Discord][discord]!

[Orbit]: https://docs.arbitrum.io/launch-orbit-chain/orbit-gentle-introduction

## Don't know C?

The Stylus VM supports more than just C. In fact, any programming language that compiles down to WebAssembly could in principle be deployed to Stylus-enabled chains. The table below includes the official ports of the SDK, with more coming soon.

| Repo             | Use cases                   | License           |
|:-----------------|:----------------------------|:------------------|
| [Rust SDK][Rust] | Everything!                 | Apache 2.0 or MIT |
| [C/C++ SDK][C]   | Cryptography and algorithms | Apache 2.0 or MIT |
| [Bf SDK][Bf]     | Educational                 | Apache 2.0 or MIT |

Want to write your own? Join us in the `#stylus` channel on [discord][discord]!

[Rust]: https://github.com/OffchainLabs/stylus-sdk-rs
[C]: https://github.com/OffchainLabs/stylus-sdk-c
[Bf]: https://github.com/OffchainLabs/stylus-sdk-bf

[discord]: https://discord.com/invite/5KE54JwyTs

## License

&copy; 2022-2023 Offchain Labs, Inc.

This project is licensed under either of

- [Apache License, Version 2.0](https://www.apache.org/licenses/LICENSE-2.0) ([licenses/Apache-2.0](licenses/Apache-2.0))
- [MIT license](https://opensource.org/licenses/MIT) ([licenses/MIT](licenses/MIT))

at your option.

The [SPDX](https://spdx.dev) license identifier for this project is `MIT OR Apache-2.0`.
