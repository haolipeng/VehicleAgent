#ifndef FLB_NOTIFICATION_H
#define FLB_NOTIFICATION_H

#include <fluent-bit/flb_info.h>
#include <fluent-bit/flb_config.h>
#include <fluent-bit/flb_socket.h>
#include <fluent-bit/flb_config.h>
#include <cfl/cfl_sds.h>


#define FLB_NOTIFICATION_TYPE_BLOB_DELIVERY 1


struct flb_notification {
    int   plugin_type;
    void *plugin_instance;
    int   notification_type;
    int   dynamically_allocated;

    void (*destructor)(void *);
};

int flb_notification_enqueue(int plugin_type,
                             char *instance_name,
                             struct flb_notification *notification,
                             struct flb_config *config);

int flb_notification_receive(flb_pipefd_t channel,
                             struct flb_notification **notification);

int flb_notification_deliver(struct flb_notification *notification);

void flb_notification_cleanup(struct flb_notification *notification);

#endif
