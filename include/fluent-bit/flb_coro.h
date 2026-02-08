#ifndef FLB_CORO_H
#define FLB_CORO_H

/* Required by OSX */
#ifndef _XOPEN_SOURCE
#define _XOPEN_SOURCE
#endif

#ifndef _DEFAULT_SOURCE
#define _DEFAULT_SOURCE
#endif

#include <fluent-bit/flb_info.h>
#include <fluent-bit/flb_macros.h>
#include <fluent-bit/flb_log.h>
#include <fluent-bit/flb_mem.h>

#include <monkey/mk_core.h>

#include <stdlib.h>
#include <limits.h>
#include <libco.h>

#ifdef FLB_HAVE_VALGRIND
#include <valgrind/valgrind.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

struct flb_coro {

#ifdef FLB_HAVE_VALGRIND
    unsigned int valgrind_stack_id;
#endif

    /* libco 'contexts' */
    cothread_t caller;
    cothread_t callee;

    void *data;
};

#ifdef FLB_SYSTEM_MACOS
#ifdef __aarch64__
#define STACK_FACTOR 1.5 /* Use 36KiB for coro stacks */
#else
#define STACK_FACTOR 2   /* Use 24KiB for coro stacks */
#endif
#else
#define STACK_FACTOR 1
#endif

#ifdef FLB_CORO_STACK_SIZE
#define FLB_CORO_STACK_SIZE_BYTE      FLB_CORO_STACK_SIZE
#else
#define FLB_CORO_STACK_SIZE_BYTE      ((3 * STACK_FACTOR * PTHREAD_STACK_MIN) / 2)
#endif

#define FLB_CORO_DATA(coro)      (((char *) coro) + sizeof(struct flb_coro))

static FLB_INLINE void flb_coro_yield(struct flb_coro *coro, int ended)
{
    co_switch(coro->caller);
}

static FLB_INLINE void flb_coro_destroy(struct flb_coro *coro)
{
    flb_trace("[coro] destroy coroutine=%p data=%p", coro,
              FLB_CORO_DATA(coro));

#ifdef FLB_HAVE_VALGRIND
    VALGRIND_STACK_DEREGISTER(coro->valgrind_stack_id);
#endif

    if (coro->callee != NULL) {
        co_delete(coro->callee);
    }

    flb_free(coro);
}

#define flb_coro_return(th) co_switch(th->caller)

void flb_coro_init();
void flb_coro_thread_init();

struct flb_coro *flb_coro_get();
void flb_coro_set(struct flb_coro *coro);

static FLB_INLINE void flb_coro_resume(struct flb_coro *coro)
{
    flb_coro_set(coro);
    coro->caller = co_active();
    co_switch(coro->callee);
}

static FLB_INLINE struct flb_coro *flb_coro_create(void *data)
{
    struct flb_coro *coro;

    /* Create a thread context and initialize */
    coro = (struct flb_coro *) flb_calloc(1, sizeof(struct flb_coro));
    if (!coro) {
        flb_errno();
        return NULL;
    }
    coro->data = data;
    return coro;
}

#ifdef __cplusplus
}
#endif

#endif /* !FLB_CORO_H */
