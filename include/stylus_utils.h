/**
 * Note: not at all a full implementation of string.h,
 * just the parts of it supported by simplelib
 */
#ifndef __STYLUS_UTILS_H
#define __STYLUS_UTILS_H

#include <stddef.h>
#include <stdint.h>
#include <stylus_types.h>
#include <hostio.h>
#include <bebi.h>

#ifdef __cplusplus
extern "C" {
#endif

inline void msg_sender_padded(bebi sender) {
    msg_sender(sender+12);
}

ArbResult inline _return_nodata(ArbStatus status) {
    ArbResult res = {status, NULL , 0};
    return res;
}

ArbResult _return_short_string(ArbStatus status, char *string);

#ifdef __cplusplus
}
#endif

#endif // __STYLUS_UTILS_H
