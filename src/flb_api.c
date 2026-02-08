#include <fluent-bit/flb_info.h>
#include <fluent-bit/flb_api.h>
#include <fluent-bit/flb_mem.h>
#include <fluent-bit/flb_log.h>

#include <fluent-bit/flb_input.h>
#include <fluent-bit/flb_output.h>
#include <fluent-bit/flb_custom.h>

struct flb_api *flb_api_create()
{
    struct flb_api *api;

    api = flb_malloc(sizeof(struct flb_api));
    if (!api) {
        flb_errno();
        return NULL;
    }

    api->output_get_property = flb_output_get_property;
    api->input_get_property = flb_input_get_property;
    api->custom_get_property = flb_custom_get_property;

#ifdef FLB_HAVE_METRICS
    api->output_get_cmt_instance = flb_output_get_cmt_instance;
    api->input_get_cmt_instance = flb_input_get_cmt_instance;
#endif

    api->log_print = flb_log_print;
    api->input_log_check = flb_input_log_check;
    api->output_log_check = flb_output_log_check;
    api->custom_log_check = flb_custom_log_check;

    return api;
}

void flb_api_destroy(struct flb_api *api)
{
    flb_free(api);
}
