#ifndef FLB_HTTP_SERVER_HTTP1
#define FLB_HTTP_SERVER_HTTP1

#include <monkey/mk_http.h>
#include <monkey/mk_http_parser.h>
#include <monkey/mk_utils.h>

struct flb_http_server_session;
struct flb_http_stream;

struct flb_http1_server_session {
    struct mk_http_session          inner_session;
    struct mk_http_request          inner_request;
    struct mk_http_parser           inner_parser;
    struct mk_server                inner_server;
    int                             initialized;
    struct flb_http_stream          stream;
    struct flb_http_server_session *parent;
};

/* RESPONSE */

struct flb_http_response *flb_http1_response_begin(
                                struct flb_http1_server_session *session, 
                                struct flb_http_stream *stream);

int flb_http1_response_commit(struct flb_http_response *response);

int flb_http1_response_set_header(struct flb_http_response *response, 
                              char *name, size_t name_length,
                              char *value, size_t value_length);

int flb_http1_response_set_status(struct flb_http_response *response, 
                              int status);

int flb_http1_response_set_body(struct flb_http_response *response, 
                            unsigned char *body, size_t body_length);

/* SESSION */

int flb_http1_server_session_init(struct flb_http1_server_session *session, 
                       struct flb_http_server_session *parent);

void flb_http1_server_session_destroy(struct flb_http1_server_session *session);

int flb_http1_server_session_ingest(struct flb_http1_server_session *session, 
                         unsigned char *buffer, 
                         size_t length);

#endif
