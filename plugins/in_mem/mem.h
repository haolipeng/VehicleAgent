#ifndef FLB_IN_MEM_H
#define FLB_IN_MEM_H

#include <fluent-bit/flb_input.h>
#include <fluent-bit/flb_config.h>
#include <fluent-bit/flb_utils.h>
#include <fluent-bit/flb_log_event_encoder.h>
#include <msgpack.h>

#define DEFAULT_INTERVAL_SEC  "1"
#define DEFAULT_INTERVAL_NSEC "0"

struct flb_in_mem_info {
    uint64_t mem_total;
    uint64_t mem_used;
    uint64_t mem_free;
    uint64_t swap_total;
    uint64_t swap_used;
    uint64_t swap_free;
};

struct flb_in_mem_config {
    int    idx;
    int    page_size;
    int    interval_sec;
    int    interval_nsec;
    pid_t  pid;
    struct flb_input_instance *ins;
    struct flb_log_event_encoder log_encoder;
};

#endif
