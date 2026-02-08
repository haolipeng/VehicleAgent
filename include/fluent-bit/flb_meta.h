#ifndef FLB_META_H
#define FLB_META_H

#include <fluent-bit/flb_info.h>
#include <fluent-bit/flb_config.h>

int flb_meta_run(struct flb_config *ctx, const char *cmd, const char *params);

#endif
