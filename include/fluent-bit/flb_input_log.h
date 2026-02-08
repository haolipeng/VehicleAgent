#ifndef FLB_INPUT_LOG_H
#define FLB_INPUT_LOG_H

#include <fluent-bit/flb_info.h>
#include <fluent-bit/flb_input.h>

int flb_input_log_append(struct flb_input_instance *ins,
                         const char *tag, size_t tag_len,
                         const void *buf, size_t buf_size);

int flb_input_log_append_records(struct flb_input_instance *ins,
                                 size_t records,
                                 const char *tag, size_t tag_len,
                                 const void *buf, size_t buf_size);

int flb_input_log_append_skip_processor_stages(struct flb_input_instance *ins,
                                               size_t processor_starting_stage,
                                               const char *tag,
                                               size_t tag_len,
                                               const void *buf,
                                               size_t buf_size);
#endif
