#ifndef FLB_HTTP_CLIENT_HTTP2_H
#define FLB_HTTP_CLIENT_HTTP2_H

#include <fluent-bit/flb_http_common.h>
#include <nghttp2/nghttp2.h>
#include <nghttp2/nghttp2.h>

struct flb_http_client_session;

struct flb_http2_client_session {
    nghttp2_session                *inner_session;
    int                             initialized;
    struct flb_http_client_session *parent;
};

int flb_http2_client_session_init(struct flb_http2_client_session *session);

void flb_http2_client_session_destroy(struct flb_http2_client_session *session);

int flb_http2_client_session_ingest(struct flb_http2_client_session *session,
                                    unsigned char *buffer,
                                    size_t length);

int flb_http2_request_begin(struct flb_http_request *request);

int flb_http2_request_commit(struct flb_http_request *request);

#endif
