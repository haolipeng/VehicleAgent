/* Fluent Bit intermediate metric representation */

#ifndef FLB_INTERMEDIATE_METRIC_H
#define FLB_INTERMEDIATE_METRIC_H

#include <fluent-bit/flb_time.h>
#include <monkey/mk_core.h>
#include <msgpack.h>

/* Metric Type- Gague or Counter */
#define GAUGE 1
#define COUNTER 2

/* Metric Unit */
#define PERCENT "Percent"
#define BYTES "Bytes"

struct flb_intermediate_metric
{
    msgpack_object key;
    msgpack_object value;
    int metric_type;
    const char *metric_unit;
    struct flb_time timestamp;

    struct mk_list _head;
};

#endif