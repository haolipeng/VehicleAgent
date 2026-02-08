#ifndef FLB_UPSTREAM_CONN_H
#define FLB_UPSTREAM_CONN_H

#include <fluent-bit/flb_info.h>
#include <fluent-bit/flb_socket.h>
#include <fluent-bit/flb_config.h>
#include <fluent-bit/flb_connection.h>

int flb_upstream_conn_recycle(struct flb_connection *conn, int val);
struct flb_connection *flb_upstream_conn_get(struct flb_upstream *u);
int flb_upstream_conn_release(struct flb_connection *u_conn);
int flb_upstream_conn_timeouts(struct mk_list *list);
int flb_upstream_conn_pending_destroy(struct flb_upstream *u);
int flb_upstream_conn_pending_destroy_list(struct mk_list *list);
int flb_upstream_conn_active_destroy_list(struct mk_list *list);

#endif
