#ifndef FLB_IN_UNIX_SOCKET_CONFIG_H
#define FLB_IN_UNIX_SOCKET_CONFIG_H

#include "unix_socket.h"

struct flb_in_unix_socket_config *unix_socket_config_init(struct flb_input_instance *i_ins);
int unix_socket_config_destroy(struct flb_in_unix_socket_config *config);

#endif
