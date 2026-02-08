#ifndef FLB_MACROS_H
#define FLB_MACROS_H

#include <stdbool.h>
#include <monkey/mk_core.h>

#define FLB_FALSE  false
#define FLB_TRUE   true

/* Return values */
#define FLB_ERROR   0
#define FLB_OK      1
#define FLB_RETRY   2

/* ala-printf format check */
#if defined(__GNUC__) || defined(__clang__)
#define FLB_FORMAT_PRINTF(fmt, args) __attribute__ ((format (printf, fmt, args)))
#else
#define FLB_FORMAT_PRINTF(fmt, args)
#endif

#ifdef _WIN32
#define FLB_INLINE inline
#else
#define FLB_INLINE inline __attribute__((always_inline))
#endif

#define FLB_EXPORT MK_EXPORT

#define flb_unlikely(x) mk_unlikely(x)
#define flb_likely(x)   mk_likely(x)

#define flb_bug(condition) do {                                         \
        if (flb_unlikely((condition)!=0)) {                             \
            fprintf(stderr, "Bug found in %s() at %s:%d",               \
                    __FUNCTION__, __FILE__, __LINE__);                  \
            abort();                                                    \
        }                                                               \
    } while(0)
#endif
