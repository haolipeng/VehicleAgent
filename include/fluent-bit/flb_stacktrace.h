#ifndef FLB_STACKTRACE_H
#define FLB_STACKTRACE_H

#include <fluent-bit/flb_info.h>

/* Libbacktrace support */
#if defined(FLB_HAVE_LIBBACKTRACE)
#include <backtrace.h>
#include <backtrace-supported.h>

struct flb_stacktrace {
    struct backtrace_state *state;
    int error;
    int line;
};

static void flb_stacktrace_error_callback(void *data,
                                          const char *msg, int errnum)
{
    struct flb_stacktrace *ctx = data;
    fprintf(stderr, "ERROR: %s (%d)", msg, errnum);
    ctx->error = 1;
}

static int flb_stacktrace_print_callback(void *data, uintptr_t pc,
                                         const char *filename, int lineno,
                                         const char *function)
{
    struct flb_stacktrace *p = data;

    fprintf(stderr, "#%-2i 0x%-17lx in  %s() at %s:%d\n",
            p->line,
            (unsigned long) pc,
            function == NULL ? "???" : function,
            filename == NULL ? "???" : filename + sizeof(FLB_SOURCE_DIR),
            lineno);
    p->line++;
    return 0;
}

static inline void flb_stacktrace_init(char *prog, struct flb_stacktrace *st)
{
    memset(st, '\0', sizeof(struct flb_stacktrace));
    st->state = backtrace_create_state(prog,
                                       BACKTRACE_SUPPORTS_THREADS,
                                       flb_stacktrace_error_callback, NULL);
}

static inline void flb_stacktrace_print(struct flb_stacktrace *st)
{
    backtrace_full(st->state, 3, flb_stacktrace_print_callback,
                   flb_stacktrace_error_callback, st);
}

#endif
#endif
