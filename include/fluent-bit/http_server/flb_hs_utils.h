#ifndef FLB_HS_UTIL_H
#define FLB_HS_UTIL_H

#include <monkey/mk_lib.h>

int flb_hs_add_content_type_to_req(mk_request_t *request, int type);

/* Content-type */
enum content_type {
    FLB_HS_CONTENT_TYPE_JSON,
    FLB_HS_CONTENT_TYPE_PROMETHEUS,
    FLB_HS_CONTENT_TYPE_OTHER
};

#define FLB_HS_CONTENT_TYPE_KEY_STR "Content-Type"
#define FLB_HS_CONTENT_TYPE_KEY_LEN 12

#define FLB_HS_CONTENT_TYPE_JSON_STR  "application/json"
#define FLB_HS_CONTENT_TYPE_JSON_LEN  16
#define FLB_HS_CONTENT_TYPE_PROMETHEUS_STR "text/plain; version=0.0.4"
#define FLB_HS_CONTENT_TYPE_PROMETHEUS_LEN 25

#endif
