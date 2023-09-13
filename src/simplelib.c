#include <string.h>

void *memcpy(void *destination, const void *source, size_t num) {
    uint8_t const *src = source;
    uint8_t *dst = destination;
    size_t idx = 0;
    size_t src_mod_8 = ((size_t)source) % 8;
    size_t dst_mod_8 = ((size_t)destination) % 8;
    if (src_mod_8 == dst_mod_8 && num > 8) {
        if (src_mod_8 != 0) {
            while (idx < num && (src_mod_8 + idx < 8)) {
                dst[idx] = src[idx];
                idx++;
            }
        }
        size_t num64 = (num - idx) / 8;
        if (num64 > 0) {
            uint64_t const *src64 = (uint64_t *)(source + idx);
            uint64_t *dst64 = (uint64_t *)(destination + idx);
            size_t idx64 = 0;
            while (idx64 < num64) {
                dst64[idx64] = src64[idx64];
                idx64++;
            }
            idx = idx + num64 * 8;
        }
    }
    while (idx < num) {
        dst[idx] = src[idx];
        idx++;
    }
    return destination;
}

void *memset(void *ptr, int value, size_t num )  {
    uint8_t *dst = ptr;
    uint8_t fill = (uint8_t) value;
    size_t idx = 0;
    if (num > 8) {
        size_t dst_mod_8 = ((size_t)ptr) % 8;
        if (dst_mod_8 != 0) {
            while (idx < num && (dst_mod_8 + idx < 8)) {
                dst[idx] = fill;
                idx++;
            }
        }
        size_t num64 = (num - idx) / 8;
        if (num64 > 0) {
            uint64_t *dst64 = (uint64_t *)(dst + idx);
            uint64_t fill64 = fill;
            fill64 <<= 8;
            fill64 |= fill;
            uint64_t tmp = fill64;
            fill64 <<= 16;
            fill64 |= tmp;
            size_t idx64 = 0;
            while (idx64 < num64) {
                dst64[idx64] = fill64;
                idx64++;
            }
            idx = idx + num64 * 8;
        }
    }
    while (idx < num) {
        dst[idx] = fill;
        idx++;
    }
    return ptr;
}

char *strncpy(char *dst, const char *src, size_t num) {
    size_t idx=0;
    while (idx<num && src[idx]!=0) {
        dst[idx]=src[idx];
        idx++;
    }
    while (idx<num && src[idx]!=0) {
        dst[idx]=0;
        idx++;
    }
    return dst;
}

size_t strlen(const char *str) {
    size_t idx=0;
    while (str[idx]!=0) {
        idx++;
    }
    return idx;
}
