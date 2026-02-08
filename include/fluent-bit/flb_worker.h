#ifndef FLB_WORKER_H
#define FLB_WORKER_H

#include <fluent-bit/flb_config.h>

struct flb_config;

struct flb_worker {
    struct mk_event event;

    /* Callback data */
    void (*func) (void *);     /* function    */
    void *data;                /* opaque data */
    pthread_t tid;             /* thread ID   */

    /* Logging */
    struct flb_log_cache *log_cache;
#ifdef _WIN32
    intptr_t log[2];
#else
    flb_pipefd_t log[2];
#endif

    /* Runtime context */
    void *config;
    void *log_ctx;

    pthread_mutex_t mutex;
    struct mk_list _head;    /* link to head at config->workers */
};

int flb_worker_init(struct flb_config *config);
struct flb_worker *flb_worker_get();

struct flb_worker *flb_worker_context_create(void (*func) (void *), void *arg,
                                             struct flb_config *config);

int flb_worker_create(void (*func) (void *), void *arg, pthread_t *tid,
                      struct flb_config *config);
struct flb_worker *flb_worker_lookup(pthread_t tid, struct flb_config *config);
int flb_worker_exit(struct flb_config *config);
int flb_worker_log_level(struct flb_worker *worker);

#endif
