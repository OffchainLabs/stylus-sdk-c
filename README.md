<br />
<p align="center">
  <a href="https://arbitrum.io/">
    <img src="https://arbitrum.io/wp-content/uploads/2023/08/stylus-thumbnail-1000.png" alt="Logo" width="100%">
  </a>

  <h3 align="center">The Stylus SDK</h3>

  <p align="center">
    <a href="https://developer.arbitrum.io/"><strong>C/C++ contracts on Arbitrum »</strong></a>
    <br />
  </p>
</p>

## Getting Started
The C/C++ SDK consists of two headers, only the first of which is necessary to write Stylus contracts.

| Header                 | Info                          |
|:-----------------------|:------------------------------|
| [`stylus.h`](stylus.h) | Provides a minimal entrypoint |
| [`hostio.h`](hostio.h) | Additional EVM affordances    |

The Stylus VM executes WebAssembly, so you'll need a C/C++ compiler with support for wasm32 targets. Support for this varies, so some users may have to build `clang` or `gcc` from source. Your package manager may also include a compatible version.

The table below includes `clang` flags commonly used to build Stylus contracts. The [siphash][siphash] example uses most of the following, and is a great starting point for programs that opt out of the standard library.

| Flag                    | Info                                                          | Optional |
|:------------------------|---------------------------------------------------------------|:---------|
| --target=wasm32         | compile to wasm                                               |          |
| --no-standard-libraries | opt out of the stdandard library                              | ✅       |
| -mbulk-memory           | enable bulk-memory operations (accelerates memset and memcpy) | ✅       |
| -Wl,--no-entry          | let Stylus decide the entrypoint                              |          |
| -O3                     | optimize for speed                                            | ✅       |
| -Oz                     | optimize for binary size                                      | ✅       |

The easiest way to deploy your C or C++ program is to use the [Cargo Stylus CLI tool][cargo], which has a `--wasm-file-path` flag that accepts arbitrary WASMs.
```sh
cargo stylus deploy --wasm-file-path <wasm> --endpoint <rpc> --private-key-path <secret>
```

[cargo]: https://github.com/OffchainLabs/cargo-stylus

## Performance
C binaries are both small and very efficient. The [`siphash`][siphash] example is only **609 bytes** onchain and costs **22 gas** to execute a 32-byte input. By contrast, 22 gas only buys 7 ADD instructions in Solidity.

How did we achieve this efficiency? All we had to do was Google for an example siphash program and add a simple entrypoint. In the Stylus model, you can deploy highly-optimized and thouroughly-audited, industry-standard reference implementations as-is. With the Stylus SDK, cryptography, algorithms, and other high-compute applications are both straightforward and economically viable.

## Host I/Os

Though this C/C++ library is oriented toward pure-compute use cases, it is possible to write stateful smart contracts that behave like Solidity and Rust ones by importing [`hostios.h`](hostios.h). There you can call VM hooks directly, which allows you to do everything from looking up the current block number to calling other contracts.

For example, the VM provides an efficient implementation of [keccak256][keccak256] via
```c
void native_keccak256(const uint8_t * bytes, size_t len, uint8_t * output)
```

Unlike with the Rust SDK, however, you will have to work with raw pointers and deserialize arguments manually. This makes [`stylus.h`](stylus.h) an ideal environment for bytes-in bytes-out programming, but not general smart contract development.

For a comprehensive list of hostios, please see [The Host I/O Reference][hostios].

[hostios]: TODO
[keccak256]: https://en.wikipedia.org/wiki/SHA-3
[siphash]: examples/siphash/main.c

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
