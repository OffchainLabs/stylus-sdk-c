// Copyright 2022-2023, Offchain Labs, Inc.
// For licensing, see https://github.com/stylus-sdk-c/blob/stylus/licenses/COPYRIGHT.md

#ifndef __STYLUS_TYPES_H
#define __STYLUS_TYPES_H

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum ArbStatus {
    Success = 0,
    Failure,
} ArbStatus;

typedef struct ArbResult {
    const ArbStatus status;
    const uint8_t * output;
    const size_t output_len;
} ArbResult;

// this will revert execution without data by causing an error
inline void revert() {
    asm("unreachable"); // causes a mchine error that revers the transaction
}

#ifdef __cplusplus
}
#endif

#endif
