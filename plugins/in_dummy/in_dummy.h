#ifndef FLB_IN_DUMMY_H
#define FLB_IN_DUMMY_H

#include <fluent-bit/flb_info.h>
#include <fluent-bit/flb_input.h>
#include <fluent-bit/flb_log_event_encoder.h>

#define DEFAULT_DUMMY_MESSAGE  "{\"message\":\"dummy\"}"
#define DEFAULT_DUMMY_METADATA "{}"
#define DEFAULT_RATE  "1"
#define DEFAULT_INTERVAL_SEC "0"
#define DEFAULT_INTERVAL_NSEC "0"

struct flb_dummy {
    int  coll_fd;

    int  rate;
    int  copies;
    int  samples;
    int  samples_count;
    int  interval_sec;
    int  interval_nsec;

    int dummy_timestamp_set;
    struct flb_time base_timestamp;
    struct flb_time dummy_timestamp;

    int  start_time_sec;
    int  start_time_nsec;

    int fixed_timestamp;
    int flush_on_startup;
    int test_hang_on_exit;  /* TEST ONLY: Used for hot reload watchdog testing */

    char *ref_metadata_msgpack;
    size_t ref_metadata_msgpack_size;

    char *ref_body_msgpack;
    size_t ref_body_msgpack_size;

    struct flb_log_event_encoder *encoder;

    struct flb_input_instance *ins;
};

#endif
