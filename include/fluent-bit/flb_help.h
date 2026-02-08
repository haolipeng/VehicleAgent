#ifndef FLB_HELP_H
#define FLB_HELP_H

#include <fluent-bit/flb_info.h>
#include <fluent-bit/flb_sds.h>
#include <fluent-bit/flb_custom.h>
#include <fluent-bit/flb_input.h>
#include <fluent-bit/flb_filter.h>
#include <fluent-bit/flb_output.h>

/* JSON Helper version: current '1' */
#define FLB_HELP_SCHEMA_VERSION "1"

enum help_plugin_type {
    FLB_HELP_PLUGIN_CUSTOM = 0,
    FLB_HELP_PLUGIN_INPUT,
    FLB_HELP_PLUGIN_PROCESSOR,
    FLB_HELP_PLUGIN_FILTER,
    FLB_HELP_PLUGIN_OUTPUT,
};

int flb_help_custom(struct flb_custom_instance *ins, void **out_buf, size_t *out_size);
int flb_help_input(struct flb_input_instance *ins, void **out_buf, size_t *out_size);
int flb_help_processor(struct flb_processor_instance *ins, void **out_buf, size_t *out_size);
int flb_help_filter(struct flb_filter_instance *ins, void **out_buf, size_t *out_size);
int flb_help_output(struct flb_output_instance *ins, void **out_buf, size_t *out_size);

flb_sds_t flb_help_build_json_schema(struct flb_config *config);

#endif
