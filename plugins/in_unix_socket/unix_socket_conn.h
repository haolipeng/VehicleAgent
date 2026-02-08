#ifndef FLB_IN_UNIX_SOCKET_CONN_H
#define FLB_IN_UNIX_SOCKET_CONN_H

#include <fluent-bit/flb_pack.h>
#include <fluent-bit/flb_connection.h>

#define FLB_IN_UNIX_SOCKET_CHUNK "32768"

enum {
    UNIX_SOCKET_NEW        = 1,  /* it's a new connection                */
    UNIX_SOCKET_CONNECTED  = 2,  /* MQTT connection per protocol spec OK */
};

struct unix_socket_conn_stream {
    char *tag;
    size_t tag_len;
};

/* Respresents a connection */
struct unix_socket_conn {
    int status;                       /* Connection status                 */

    /* Buffer */
    char *buf_data;                   /* Buffer data                       */
    int  buf_len;                     /* Data length                       */
    int  buf_size;                    /* Buffer size                       */
    size_t rest;                      /* Unpacking offset                  */

    struct flb_input_instance *ins;   /* Parent plugin instance            */
    struct flb_in_unix_socket_config *ctx;    /* Plugin configuration context      */
    struct flb_pack_state pack_state; /* Internal JSON parser              */
    struct flb_connection *connection;

    struct mk_list _head;
};

struct unix_socket_conn *unix_socket_conn_add(struct flb_connection *connection, struct flb_in_unix_socket_config *ctx);
int unix_socket_conn_del(struct unix_socket_conn *conn);
int unix_socket_conn_event(void *data);

#endif
