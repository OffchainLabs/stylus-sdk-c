
#ifndef __BEBI_H
#define __BEBI_H

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef uint8_t bebi[];

// return values:
// -1 : ERROR rhs_size > lhs_size
// 0 : O.k
// 1 : O.k but there was an overflow
inline int bebi_add(bebi lhs, size_t lhs_size, const bebi rhs, size_t rhs_size) {
    if (rhs_size > lhs_size) {
        return -1;
    }
    size_t left = lhs_size;
    size_t right = rhs_size;
    uint8_t carry = 0;
    while (left > 0 && right > 0) {
        left--;
        right--;
        uint8_t res = lhs[left] + rhs[right] + carry;
        carry = res < lhs[left] ? 1 : 0;
        lhs[left] = res;
    }
    while (left > 0 && carry) {
        left--;
        uint8_t res = lhs[left] + carry;
        carry = res < lhs[left] ? 1 : 0;
        lhs[left] = res;
    }
    return carry;
}

// return values:
// -1 : ERROR rhs_size > lhs_size
// 0 : O.k
// 1 : O.k but there was an overflow
inline int bebi_sub(bebi lhs, size_t lhs_size, const bebi rhs, size_t rhs_size) {
    if (rhs_size > lhs_size) {
        return -1;
    }
    size_t left = lhs_size;
    size_t right = rhs_size;
    uint8_t carry = 0;
    while (left > 0 && right > 0) {
        left--;
        right--;
        uint8_t res = lhs[left] - rhs[right] - carry;
        carry = res > lhs[left] ? 1 : 0;
        lhs[left] = res;
    }
    while (left > 0 && carry) {
        left--;
        uint8_t res = lhs[left] - carry;
        carry = res > lhs[left] ? 1 : 0;
        lhs[left] = res;
    }
    return carry;
}

// return values:
// lhs > rhs: 1
// lhs < rhs: -1
// lhs == rhs: 0
inline int bebi_cmp(const bebi lhs, size_t lhs_size, const bebi rhs, size_t rhs_size) {
    size_t left = 0;
    size_t right = 0;
    while (lhs_size - left > rhs_size) {
        if (lhs[left] != 0) {
            return 1;
        }
        left++;
    }
    while (rhs_size - right > lhs_size) {
        if (rhs[right] != 0) {
            return -1;
        }
        right++;
    }
    while (left < lhs_size) {
        if (lhs[left] > rhs[right]) {
            return 1;
        }
        if (lhs[left] < rhs[right]) {
            return -1;
        }
        right++;
        left++;
    }
    return 0;
}

inline void bebi_set_u8(bebi dst, size_t offset, uint8_t val) {
    dst[offset] = val;
}

inline bool bebi_is_0(const bebi bebi, size_t size) {
    size_t idx = 0;
    while (idx < size) {
        if (bebi[idx] != 0) {
            return false;
        }
        idx++;
    }
    return true;
}

inline uint8_t bebi_get_u8(const bebi src, size_t offset) {
    return src[offset];
}

inline void bebi_set_u16(bebi dst, size_t offset, uint16_t val) {
    dst[offset+1] = val & 0xff;
    val >>= 8;
    dst[offset] = val & 0xff;
}

inline uint16_t bebi_get_u16(const bebi src, size_t offset) {
    uint16_t val;
    val = src[offset];
    val <<= 8;
    val |= src[offset+1];
    return val;
}

inline void bebi_set_u32(bebi dst, size_t offset, uint32_t val) {
    dst[offset+3] = val & 0xff;
    val >>= 8;
    dst[offset+2] = val & 0xff;
    val >>= 8;
    dst[offset+1] = val & 0xff;
    val >>= 8;
    dst[offset] = val & 0xff;
}

inline uint32_t bebi_get_u32(const bebi src, size_t offset) {
    uint32_t val;
    val = src[offset];
    val <<= 8;
    val |= src[offset+1];
    val <<= 8;
    val |= src[offset+2];
    val <<= 8;
    val |= src[offset+3];
    return val;
}

inline void bebi_set_u64(bebi dst, size_t offset, uint64_t val) {
    dst[offset+7] = val & 0xff;
    val >>= 8;
    dst[offset+6] = val & 0xff;
    val >>= 8;
    dst[offset+5] = val & 0xff;
    val >>= 8;
    dst[offset+4] = val & 0xff;
    val >>= 8;
    dst[offset+3] = val & 0xff;
    val >>= 8;
    dst[offset+2] = val & 0xff;
    val >>= 8;
    dst[offset+1] = val & 0xff;
    val >>= 8;
    dst[offset] = val & 0xff;
}

inline uint64_t bebi_get_u64(const bebi src, size_t offset) {
    uint64_t val;
    val = src[offset];
    val <<= 8;
    val |= src[offset+1];
    val <<= 8;
    val |= src[offset+2];
    val <<= 8;
    val |= src[offset+3];
    val <<= 8;
    val |= src[offset+4];
    val <<= 8;
    val |= src[offset+5];
    val <<= 8;
    val |= src[offset+6];
    val <<= 8;
    val |= src[offset+7];
    return val;
}

typedef uint8_t bebi32[32];

int bebi32_add(bebi32 lhs, const bebi32 rhs);
int bebi32_sub(bebi32 lhs, const bebi32 rhs);
int bebi32_add_u64(bebi32 lhs, uint64_t rhs);

bool bebi32_is_0(const bebi bebi);

uint16_t bebi32_get_u16(const bebi32 dst);
uint32_t bebi32_get_u32(const bebi32 dst);
uint64_t bebi32_get_u64(const bebi32 dst);

// note: beb32_set_u* also pads with zeroes
void bebi32_set_u16(bebi32 dst, uint16_t val);
void bebi32_set_u32(bebi32 dst, uint32_t val);
void bebi32_set_u64(bebi32 dst, uint64_t val);

bool bebi32_is_u16(const bebi32 dst);
bool bebi32_is_u32(const bebi32 dst);
bool bebi32_is_u64(const bebi32 dst);
bool bebi32_is_u160(const bebi32 dst);

int bebi32_cmp(const bebi32 lhs, const bebi32 rhs);

#ifdef __cplusplus
}
#endif

#endif // __BEBI_H