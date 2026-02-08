#ifndef FLB_IN_TCP_CONFIG_H
#define FLB_IN_TCP_CONFIG_H

#include "tcp.h"

struct flb_in_tcp_config *tcp_config_init(struct flb_input_instance *i_ins);
int tcp_config_destroy(struct flb_in_tcp_config *config);

#endif
