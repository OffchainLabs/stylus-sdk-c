
#ifndef __STORAGE_H
#define __STORAGE_H

#include <stddef.h>
#include <stdint.h>
#include <bebi.h>
#include <hostio.h>

#ifdef __cplusplus
extern "C" {
#endif

// base slot for array which is has allocated storage slot
// (this is just native keccak of storage into base_out)
void array_base_slot(bebi32 const storage, bebi32 base_out);

// calculate storage location inside array.
// base here is location of first value, so the output from array_base_slot
int array_slot_offset(bebi32 const base, size_t val_size, uint64_t index, bebi32 slot_out, size_t *offset_out);

// calculate slot for a map to put "key"
// If key requires padding it must be applied before calling this function
void map_slot(bebi32 const storage, uint8_t const *key, size_t key_len, bebi32 slot_out);

// storage pointer is not currently used
// passing it is used to keep track of pure/mutating/non-mutating functions
inline void storage_load(const void* storage, const uint8_t *key, uint8_t *dest) {
    storage_load_bytes32(key, dest);
}

// storage pointer is not currently used
// passing it is used to keep track of pure/mutating/non-mutating functions
inline void storage_store(void *storage, const uint8_t *key, const uint8_t *value) {
    storage_store_bytes32(key, value);
}

#ifdef __cplusplus
}
#endif

#endif