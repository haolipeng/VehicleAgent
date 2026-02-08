#ifndef FLB_HS_API_V1_METRICS_H
#define FLB_HS_API_V1_METRICS_H

#include <fluent-bit/flb_info.h>
#include <fluent-bit/flb_http_server.h>
#include <fluent-bit/flb_sds.h>

int api_v1_metrics(struct flb_hs *hs);
flb_sds_t metrics_help_txt(char *metric_name, flb_sds_t *metric_helptxt);

#endif
