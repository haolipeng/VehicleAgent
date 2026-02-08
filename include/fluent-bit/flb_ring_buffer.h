#ifndef FLB_RING_BUFFER_H
#define FLB_RING_BUFFER_H

#include <fluent-bit/flb_pipe.h>

struct flb_ring_buffer {
    void *ctx;                        /* pointer to backend context */
    void *event_loop;                 /* event loop where this ring buffer emits flush request signals */
    int flush_pending;                /* flag meant to prevent flush request signal flood */
    void *signal_event;               /* event loop entry for the flush request signal */
    flb_pipefd_t signal_channels[2];  /* flush request signaling channel */
    uint64_t data_window;             /* 0% - 100% occupancy window flush request */
    uint64_t data_size;               /* ring buffer size */
    void *data_buf;                   /* ring buffer */
};

struct flb_ring_buffer *flb_ring_buffer_create(uint64_t size);
void flb_ring_buffer_destroy(struct flb_ring_buffer *rb);

int flb_ring_buffer_add_event_loop(struct flb_ring_buffer *rb, void *evl, uint8_t window_size);

int flb_ring_buffer_write(struct flb_ring_buffer *rb, void *ptr, size_t size);
int flb_ring_buffer_read(struct flb_ring_buffer *rb, void *ptr, size_t size);

#endif
