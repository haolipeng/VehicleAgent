#ifndef FLB_RELOAD_H
#define FLB_RELOAD_H

#include <fluent-bit/flb_info.h>
#include <fluent-bit/flb_lib.h>
#include <fluent-bit/flb_config.h>
#include <fluent-bit/flb_config_format.h>

#define FLB_RELOAD_IDLE             0
#define FLB_RELOAD_IN_PROGRESS      1
#define FLB_RELOAD_ABORTED         -1
#define FLB_RELOAD_HALTED          -2
#define FLB_RELOAD_NOT_ENABLED     -3
#define FLB_RELOAD_INVALID_CONTEXT -4

int flb_reload_property_check_all(struct flb_config *config);
int flb_reload_reconstruct_cf(struct flb_cf *src_cf, struct flb_cf *dest_cf);
int flb_reload(flb_ctx_t *ctx, struct flb_cf *cf_opts);

#endif
