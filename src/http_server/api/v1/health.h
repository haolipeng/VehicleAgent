

#ifndef FLB_HS_API_V1_HEALTH_H
#define FLB_HS_API_V1_HEALTH_H

#include <fluent-bit/flb_info.h>
#include <fluent-bit/flb_http_server.h>

struct flb_health_check_metrics_counter {

     /*
     * health check error limit,
     * setup by customer through config: HC_Errors_Count
     */
    int error_limit;

    /* counter the error number in metrics*/
    int error_counter;

    /*
    * health check retry failed limit,
    * setup by customer through config: HC_Retry_Failure_Count
    */
    int retry_failure_limit;

    /* count the retry failed number in metrics*/
    int retry_failure_counter;

    /*period limit, setup by customer through config: HC_Period*/
    int period_limit;

    /* count the seconds in one period*/
    int period_counter;

};


/*
 * error and retry failure buffers that contains certain cached data to be used
 * by health check.
 */
struct flb_hs_hc_buf {
    int users;
    int error_count;
    int retry_failure_count;
    struct mk_list _head;
};

/* health endpoint*/
int api_v1_health(struct flb_hs *hs);

/* clean up health resource when shutdown*/
void flb_hs_health_destroy();
#endif
