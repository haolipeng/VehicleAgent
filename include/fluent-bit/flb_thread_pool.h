#ifndef FLB_THREAD_POOL_H
#define FLB_THREAD_POOL_H

/* Thread status */
#define FLB_THREAD_POOL_ERROR    -1
#define FLB_THREAD_POOL_NONE      0
#define FLB_THREAD_POOL_RUNNING   1
#define FLB_THREAD_POOL_STOPPED   2

#include <fluent-bit/flb_info.h>
#ifdef FLB_SYSTEM_WINDOWS
#include <monkey/mk_core/external/winpthreads.h>
#else
#include <pthread.h>
#endif

struct worker_params {
    void (*func) (void *);
    void *data;
};

struct flb_tp_thread {
    int id;                        /* thread id inside the pool */
    int status;
    pthread_t tid;                 /* OS task id */
    struct worker_params params;   /* worker params before initialization */
    struct flb_worker *worker;     /* worker context */
    struct mk_list _head;          /* link to flb_tp->list_threads */
    struct flb_config *config;
};

struct flb_tp {
    struct mk_list list_threads;
    struct mk_list *thread_cur;
    struct flb_config *config;
};

struct flb_tp *flb_tp_create(struct flb_config *config);
void flb_tp_destroy(struct flb_tp *tp);

struct flb_tp_thread *flb_tp_thread_create(struct flb_tp *tp,
                                           void (*func)(void *), void *arg,
                                           struct flb_config *config);
struct flb_tp_thread *flb_tp_thread_get_rr(struct flb_tp *tp);
int flb_tp_thread_start(struct flb_tp *tp, struct flb_tp_thread *th);
int flb_tp_thread_start_id(struct flb_tp *tp, int id);
int flb_tp_thread_start_all(struct flb_tp *tp);
int flb_tp_thread_stop(struct flb_tp *tp, struct flb_tp_thread *th);
int flb_tp_thread_stop_all(struct flb_tp *tp);
int flb_tp_thread_destroy();

#endif
