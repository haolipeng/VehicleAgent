#include <fluent-bit/flb_info.h>
#include <fluent-bit/flb_thread_storage.h>
#include <fluent-bit/flb_coro.h>

FLB_TLS_DEFINE(struct flb_coro, flb_coro_key);

static pthread_mutex_t coro_mutex_init;

void flb_coro_init()
{
    FLB_TLS_INIT(flb_coro_key);
    pthread_mutex_init(&coro_mutex_init, NULL);
}

void flb_coro_thread_init()
{
    size_t s;
    cothread_t th;

    pthread_mutex_lock(&coro_mutex_init);
    th = co_create(256, NULL, &s);
    co_delete(th);
    pthread_mutex_unlock(&coro_mutex_init);
}

struct flb_coro *flb_coro_get()
{
    struct flb_coro *coro;

    coro = FLB_TLS_GET(flb_coro_key);
    return coro;
}

void flb_coro_set(struct flb_coro *coro)
{
    FLB_TLS_SET(flb_coro_key, coro);
}
