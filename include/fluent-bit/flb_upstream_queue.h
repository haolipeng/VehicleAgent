#ifndef FLB_UPSTREAM_QUEUE_H
#define FLB_UPSTREAM_QUEUE_H

#include <fluent-bit/flb_info.h>

struct flb_upstream_queue {
    /*
     * This field is a linked-list-head for upstream connections that
     * are available for usage. When a connection is taken, it's moved to the
     * 'busy_queue' list.
     */
    struct mk_list av_queue;

    /*
     * Linked list head for upstream connections that are in use by some
     * plugin. When released, they are moved to the 'av_queue' list.
     */
    struct mk_list busy_queue;

    /*
     * Ready to destroy connections are linked here, mostly because they are
     * not yet destroyed but will be destroyed in a later step of the event loop
     * to avoid any race condition with a late event.
     */
    struct mk_list destroy_queue;
};

#endif
