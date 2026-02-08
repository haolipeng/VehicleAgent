#ifndef FLB_UPSTREAM_HA_H
#define FLB_UPSTREAM_HA_H

#include <fluent-bit/flb_info.h>
#include <fluent-bit/flb_sds.h>
#include <fluent-bit/flb_upstream_node.h>
#include <monkey/mk_core.h>

struct flb_upstream_ha {
    flb_sds_t name;            /* Upstream HA name        */
    void *last_used_node;      /* Last used node          */
    struct mk_list nodes;      /* List of available nodes */
};

struct flb_upstream_ha *flb_upstream_ha_create(const char *name);
void flb_upstream_ha_destroy(struct flb_upstream_ha *ctx);
void flb_upstream_ha_node_add(struct flb_upstream_ha *ctx,
                              struct flb_upstream_node *node);
struct flb_upstream_node *flb_upstream_ha_node_get(struct flb_upstream_ha *ctx);
struct flb_upstream_ha *flb_upstream_ha_from_file(const char *file,
                                                  struct flb_config *config);

#endif
