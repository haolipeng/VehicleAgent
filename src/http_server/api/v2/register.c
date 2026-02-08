#include <fluent-bit/flb_info.h>
#include <fluent-bit/flb_http_server.h>

#include "metrics.h"
#include "reload.h"

int api_v2_registration(struct flb_hs *hs)
{
    api_v2_reload(hs);
    api_v2_metrics(hs);
    return 0;
}
