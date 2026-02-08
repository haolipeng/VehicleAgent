#ifndef FLB_ENGINE_DISPATCH_H
#define FLB_ENGINE_DISPATCH_H

#include <fluent-bit/flb_info.h>
#include <fluent-bit/flb_input.h>
#include <fluent-bit/flb_config.h>
#include <fluent-bit/flb_task.h>

int flb_engine_dispatch(uint64_t id, struct flb_input_instance *in,
                        struct flb_config *config);
int flb_engine_dispatch_retry(struct flb_task_retry *retry,
                              struct flb_config *config);
#endif
