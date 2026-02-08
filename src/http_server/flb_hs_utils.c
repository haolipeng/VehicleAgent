
#include <fluent-bit/flb_log.h>
#include <fluent-bit/flb_http_server.h>
#include <monkey/mk_lib.h>

int flb_hs_add_content_type_to_req(mk_request_t *request, int type)
{
    if (request == NULL) {
        return -1;
    }

    switch (type) {
    case FLB_HS_CONTENT_TYPE_JSON:
        mk_http_header(request,
                       FLB_HS_CONTENT_TYPE_KEY_STR, FLB_HS_CONTENT_TYPE_KEY_LEN,
                       FLB_HS_CONTENT_TYPE_JSON_STR, FLB_HS_CONTENT_TYPE_JSON_LEN);
        break;
    case FLB_HS_CONTENT_TYPE_PROMETHEUS:
        mk_http_header(request,
                       FLB_HS_CONTENT_TYPE_KEY_STR, FLB_HS_CONTENT_TYPE_KEY_LEN,
                       FLB_HS_CONTENT_TYPE_PROMETHEUS_STR, FLB_HS_CONTENT_TYPE_PROMETHEUS_LEN);
        break;
    default:
        flb_error("[%s] unknown type=%d", __FUNCTION__, type);
        return -1;
    }

    return 0;
}
