#ifndef FLB_DOWNSTREAM_CONN_H
#define FLB_DOWNSTREAM_CONN_H

#include <fluent-bit/flb_info.h>
#include <fluent-bit/flb_socket.h>
#include <fluent-bit/flb_config.h>
#include <fluent-bit/flb_connection.h>

int flb_downstream_conn_release(struct flb_connection *connection);
int flb_downstream_conn_timeouts(struct mk_list *list);
int flb_downstream_conn_pending_destroy(struct flb_downstream *downstream);
struct flb_connection *flb_downstream_conn_get(struct flb_downstream *stream);

#endif
