#ifndef FLB_INPUT_METRIC_H
#define FLB_INPUT_METRIC_H

#include <fluent-bit/flb_info.h>
#include <cmetrics/cmetrics.h>
#include <cmetrics/cmt_encode_msgpack.h>

int flb_input_metrics_append(struct flb_input_instance *ins,
                             const char *tag, size_t tag_len,
                             struct cmt *cmt);

int flb_input_metrics_append_skip_processor_stages(
        struct flb_input_instance *ins,
        size_t processor_starting_stage,
        const char *tag, size_t tag_len,
        struct cmt *cmt);
#endif
