#include <fluent-bit/flb_info.h>
#include <fluent-bit/flb_http_server.h>

#include "uptime.h"
#include "metrics.h"
#include "storage.h"
#include "plugins.h"
#include "health.h"
#include "trace.h"

int api_v1_registration(struct flb_hs *hs)
{
    api_v1_uptime(hs);
    api_v1_metrics(hs);
    api_v1_plugins(hs);

#ifdef FLB_HAVE_CHUNK_TRACE
    api_v1_trace(hs);
#endif /* FLB_HAVE_CHUNK_TRACE */

    if (hs->config->health_check == FLB_TRUE) {
        api_v1_health(hs);
    }

    if (hs->config->storage_metrics == FLB_TRUE) {
        api_v1_storage_metrics(hs);
    }

    return 0;
}
