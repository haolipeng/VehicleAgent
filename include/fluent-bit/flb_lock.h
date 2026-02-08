#ifndef FLB_LOCK_H
#define FLB_LOCK_H

#include <fluent-bit/flb_pthread.h>
#include <stddef.h>

/* The current values mean the system will
 * wait for 100 seconds at most in 50 millisecond increments.
 *
 * This is the worst case scenario and in reality there will
 * be no wait in 99.9% of the cases.
 */

#define FLB_LOCK_INFINITE_RETRY_LIMIT 0
#define FLB_LOCK_DEFAULT_RETRY_LIMIT  100
#define FLB_LOCK_DEFAULT_RETRY_DELAY  50000

typedef pthread_mutex_t flb_lock_t;

int flb_lock_init(flb_lock_t *lock);

int flb_lock_destroy(flb_lock_t *lock);

int flb_lock_acquire(flb_lock_t *lock,
                     size_t retry_limit,
                     size_t retry_delay);

int flb_lock_release(flb_lock_t *lock,
                     size_t retry_limit,
                     size_t retry_delay);

#endif
