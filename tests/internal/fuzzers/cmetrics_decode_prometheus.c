#include <cmetrics/cmt_decode_prometheus.h>


int
LLVMFuzzerTestOneInput(const uint8_t * data, size_t size)
{
    struct cmt *cmt = NULL;
    int result;

    /* At least one byte is needed for deciding which decoder to use */
    if (size < 1) {
        return 0;
    }

    struct cmt_decode_prometheus_parse_opts opts;
    result = cmt_decode_prometheus_create(&cmt, data, size, &opts);
    if (result == CMT_DECODE_PROMETHEUS_SUCCESS) {
        cmt_decode_prometheus_destroy(cmt);
    }

    return 0;
}
