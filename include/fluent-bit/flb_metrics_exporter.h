#include <fluent-bit/flb_info.h>
#ifdef FLB_HAVE_METRICS

#ifndef FLB_METRICS_EXPORTER_H
#define FLB_METRICS_EXPORTER_H

#include <fluent-bit/flb_info.h>
#include <fluent-bit/flb_config.h>
#include <fluent-bit/flb_metrics.h>

struct flb_me {
    int fd;
    struct flb_config *config;
    struct mk_event event;
};

int flb_me_fd_event(int fd, struct flb_me *me);
struct flb_me *flb_me_create(struct flb_config *ctx);
int flb_me_destroy(struct flb_me *me);
struct cmt *flb_me_get_cmetrics(struct flb_config *ctx);

#endif
#endif /* FLB_HAVE_METRICS */
