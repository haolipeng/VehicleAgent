#ifndef FLB_OUT_STDOUT
#define FLB_OUT_STDOUT

#include <fluent-bit/flb_output_plugin.h>
#include <fluent-bit/flb_sds.h>

struct flb_stdout {
    int out_format;
    int json_date_format;
    flb_sds_t json_date_key;
    flb_sds_t date_key;
    struct flb_output_instance *ins;
};

#endif
