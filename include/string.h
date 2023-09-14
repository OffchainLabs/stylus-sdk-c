#ifndef __SIMPLELIB_STRING_H
#define __SIMPLELIB_STRING_H

/**
 * Not at all a full implementation of string.h,
 * just the parts of it supported by simplelib
 * 
 * requirements: -
 * c-file: simplelib.c
 */

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

void *memcpy(void *destination, const void *source, size_t num);
void *memset(void *ptr, int value, size_t num );

char *strncpy(char *dst, const char *src, size_t num);
size_t strlen(const char *str);

#ifdef __cplusplus
}
#endif

#endif // __SIMPLELIB_STRING_H
