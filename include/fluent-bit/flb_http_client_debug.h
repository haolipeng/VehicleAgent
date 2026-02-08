#ifndef FLB_HTTP_CLIENT_DEBUG_H
#define FLB_HTTP_CLIENT_DEBUG_H

#include <fluent-bit/flb_info.h>
#include <fluent-bit/flb_callback.h>

static inline void flb_http_client_debug_enable(struct flb_http_client *c,
                                                struct flb_callback *cb_ctx)
{
    c->cb_ctx = cb_ctx;
}

int flb_http_client_debug_setup(struct flb_callback *cb_ctx,
                                struct mk_list *props);
int flb_http_client_debug_cb(struct flb_http_client *c, char *name);

#endif
