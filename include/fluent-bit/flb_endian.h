/* This file provides a portability layer for endian functions like
 * {be,le}{16,32,64}toh().
 *
 * You can include <fluent-bit/flb_endian.h> instead of <endian.h> and
 * ditto <sys/endian.h>.
 */

#ifndef FLB_ENDIAN_H
#define FLB_ENDIAN_H

/* endian.h is non-standard and platforms disagree with each other on it.
 *
 * * GLIBC (and its family) supports <endian.h>.
 *
 * * BSD libc provides <sys/endian.h> instead (Note that OpenBSD since
 *   5.6 supports conventional function names).
 *
 * * macOS does not support <endian.h> but exposes another set of macros
 *   in libkern.
 */
#if defined(__GLIBC__)
#include <endian.h>
#elif defined(__OpenBSD__) || defined(__FreeBSD__) || defined(__NetBSD__)
#include <sys/endian.h>
#elif defined(__APPLE__)
#include <libkern/OSByteOrder.h>
#define le16toh(x) OSSwapLittleToHostInt16(x)
#define le32toh(x) OSSwapLittleToHostInt32(x)
#define le64toh(x) OSSwapLittleToHostInt64(x)
#define be16toh(x) OSSwapBigToHostInt16(x)
#define be32toh(x) OSSwapBigToHostInt32(x)
#define be64toh(x) OSSwapBigToHostInt64(x)
#endif

#define FLB_LITTLE_ENDIAN 0
#define FLB_BIG_ENDIAN    1

#ifndef FLB_BYTE_ORDER
    #ifdef FLB_HAVE_BIG_ENDIAN_SYSTEM
        #define FLB_BYTE_ORDER FLB_BIG_ENDIAN
    #else
        #define FLB_BYTE_ORDER FLB_LITTLE_ENDIAN
    #endif
#endif

#endif
