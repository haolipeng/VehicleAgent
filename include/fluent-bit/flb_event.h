#ifndef FLB_EVENT_H
#define FLB_EVENT_H

#include <fluent-bit/flb_info.h>
#include <fluent-bit/flb_sds.h>
#include <fluent-bit/flb_input_chunk.h>

/* Event types */
#define FLB_EVENT_TYPE_LOGS       FLB_INPUT_CHUNK_TYPE_LOGS
#define FLB_EVENT_TYPE_METRICS    FLB_INPUT_CHUNK_TYPE_METRICS
#define FLB_EVENT_TYPE_BLOBS      FLB_INPUT_CHUNK_TYPE_BLOBS

#define FLB_EVENT_TYPE_HAS_TRACE FLB_INPUT_CHUNK_HAS_TRACE

/*
 * The flb_event_chunk structure is a full context used in the output plugins
 * flush callback. It contains the type of records (logs, metrics), the tag,
 * msgpack buffer, it size and a hint of the serialized msgpack events.
 */
struct flb_event_chunk {
    int type;               /* event type */
    flb_sds_t tag;          /* tag associated */
    void *data;             /* event content */
    size_t size;            /* size of event */
    size_t total_events;    /* total number of serialized events */
#ifdef FLB_HAVE_CHUNK_TRACE
    struct flb_chunk_trace *trace;
#endif
};

struct flb_event_chunk *flb_event_chunk_create(int type,
                                               int total_events,
                                               char *tag_buf, int tag_len,
                                               char *buf_data, size_t buf_size);

int flb_event_chunk_update(struct flb_event_chunk *evc,
                           char *buf_data, size_t buf_size);

void flb_event_chunk_destroy(struct flb_event_chunk *evc);

#endif
