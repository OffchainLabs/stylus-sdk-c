/**
 * Note: not at all a full implementation of stdlib.h,
 * just the parts of it supported by simplelib
 */
#ifndef __SIMPLELIB_STDLIB_H
#define __SIMPLELIB_STDLIB_H

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

void *malloc(size_t size);
void free(void *ptr);

#ifdef __cplusplus
}
#endif

#endif // __SIMPLELIB_STDLIB_H
