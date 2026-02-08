#include <fluent-bit/flb_info.h>
#include <fluent-bit/flb_mem.h>
#include <fluent-bit/flb_log.h>

#include <fluent-bit/flb_event.h>
#include <fluent-bit/flb_sds.h>
struct flb_event_chunk *flb_event_chunk_create(int type,
                                               int total_events,
                                               char *tag_buf, int tag_len,
                                               char *buf_data, size_t buf_size)
{
    struct flb_event_chunk *evc;

    /* event chunk context */
    evc = flb_malloc(sizeof(struct flb_event_chunk));
    if (!evc) {
        flb_errno();
        return NULL;
    }

    /* create a copy of the tag */
    evc->tag = flb_sds_create_len(tag_buf, tag_len);
    if (!evc->tag) {
        flb_free(evc);
        return NULL;
    }

#ifdef FLB_HAVE_CHUNK_TRACE
    evc->trace = NULL;
#endif

    evc->type = type;
    evc->data = buf_data;
    evc->size = buf_size;
    evc->total_events = total_events;

    return evc;
}

/* Update the buffer reference */
int flb_event_chunk_update(struct flb_event_chunk *evc,
                           char *buf_data, size_t buf_size)
{
    evc->data = buf_data;
    evc->size = buf_size;

    return 0;
}

void flb_event_chunk_destroy(struct flb_event_chunk *evc)
{
    if (!evc) {
        return;
    }

    if (evc->tag) {
        flb_sds_destroy(evc->tag);
    }
    flb_free(evc);
}
