#ifndef FLB_IO_H
#define FLB_IO_H

#include <monkey/mk_core.h>

#include <fluent-bit/flb_info.h>
#include <fluent-bit/flb_coro.h>

/* Coroutine status 'flb_coro.status' */
#define FLB_IO_CONNECT     0  /* thread issue a connection request */
#define FLB_IO_WRITE       1  /* thread wants to write() data      */

/* Network operation modes */
#define FLB_IO_TCP         1  /* use plain TCP                          */
#define FLB_IO_TLS         2  /* use TLS/SSL layer                      */
#define FLB_IO_OPT_TLS     4  /* use TCP and optional TLS               */
#define FLB_IO_ASYNC       8  /* use async mode (depends on event loop) */
#define FLB_IO_TCP_KA     16  /* use async mode (depends on event loop) */

/* Other features */
#define FLB_IO_IPV6       32  /* network I/O uses IPv6                  */

struct flb_connection;

int flb_io_net_accept(struct flb_connection *connection,
                       struct flb_coro *th);

int flb_io_net_connect(struct flb_connection *u_conn,
                       struct flb_coro *th);

int flb_io_net_write(struct flb_connection *connection, const void *data,
                     size_t len, size_t *out_len);

ssize_t flb_io_net_read(struct flb_connection *connection, void *buf, size_t len);

int flb_io_fd_write(int fd, const void *data, size_t len, size_t *out_len);

ssize_t flb_io_fd_read(int fd, void *buf, size_t len);

#endif
