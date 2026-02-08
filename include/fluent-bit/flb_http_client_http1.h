#ifndef FLB_HTTP_CLIENT_HTTP1_H
#define FLB_HTTP_CLIENT_HTTP1_H

#include <fluent-bit/flb_http_common.h>

struct flb_http_client;
struct flb_http_client_session;

struct flb_http1_client_session {
    struct flb_http_client         *inner_session;
    int                             initialized;
    struct flb_http_client_session *parent;
};

int flb_http1_client_session_init(struct flb_http1_client_session *session);

void flb_http1_client_session_destroy(struct flb_http1_client_session *session);

int flb_http1_client_session_ingest(struct flb_http1_client_session *session,
                                    unsigned char *buffer,
                                    size_t length);

int flb_http1_request_begin(struct flb_http_request *request);

int flb_http1_request_commit(struct flb_http_request *request);

#endif
