#include <fluent-bit/flb_compat.h>
#include <fcntl.h>

#ifdef FLB_HAVE_GETENTROPY
#include <unistd.h>
#endif
#ifdef FLB_HAVE_GETENTROPY_SYS_RANDOM
#include <sys/random.h>
#endif

#define MAX_GETENTROPY_LEN 256

/*
 * This module provides a random number generator for common use cases.
 *
 * On Windows, we use BCryptGenRandom() from CNG API. This function
 * is available since Windows Vista, and should be compliant to the
 * official recommendation.
 *
 * On other platforms, we use getentropy(3) if available, otherwise
 * /dev/urandom as a secure random source.
 */

int flb_random_bytes(unsigned char *buf, int len)
{
#ifdef FLB_SYSTEM_WINDOWS
    NTSTATUS ret;
    ret = BCryptGenRandom(NULL, buf, len, BCRYPT_USE_SYSTEM_PREFERRED_RNG);
    if (!BCRYPT_SUCCESS(ret)) {
        return -1;
    }
    return 0;
#else
    int     fd;
    ssize_t bytes;

#if defined(FLB_HAVE_GETENTROPY) || defined(FLB_HAVE_GETENTROPY_SYS_RANDOM)
    while (len > 0) {
        if (len > MAX_GETENTROPY_LEN) {
            bytes = MAX_GETENTROPY_LEN;
        }
        else {
            bytes = len;
        }
        if (getentropy(buf, bytes) < 0) {
#ifdef ENOSYS
            /* Fall back to urandom if the syscall is not available (Linux only) */
            if (errno == ENOSYS) {                
                goto try_urandom;
            }
#endif
            return -1;
        }
        len -= bytes;
        buf += bytes;
    }
    return 0;

try_urandom:
#endif /* FLB_HAVE_GETENTROPY || FLB_HAVE_GETENTROPY_SYS_RANDOM */
    fd = open("/dev/urandom", O_RDONLY);
    if (fd == -1) {
        return -1;
    }

    while (len > 0) {
        bytes = read(fd, buf, len);
        if (bytes <= 0) {
            close(fd);
            return -1;
        }
        len -= bytes;
        buf += bytes;
    }
    close(fd);
    return 0;
#endif /* FLB_SYSTEM_WINDOWS */
}
