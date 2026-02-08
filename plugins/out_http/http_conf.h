#ifndef FLB_OUT_HTTP_CONF_H
#define FLB_OUT_HTTP_CONF_H

#include <fluent-bit/flb_info.h>
#include <fluent-bit/flb_output.h>

#include "http.h"

struct flb_out_http *flb_http_conf_create(struct flb_output_instance *ins,
                                          struct flb_config *config);
void flb_http_conf_destroy(struct flb_out_http *ctx);

#endif
