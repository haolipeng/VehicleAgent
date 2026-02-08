#include <cmetrics/cmt_decode_msgpack.h>
#include <cmetrics/cmt_decode_opentelemetry.h>


int
LLVMFuzzerTestOneInput(const uint8_t * data, size_t size)
{
    struct cfl_list decoded_contexts;
    struct cmt *cmt = NULL;
    size_t off = 0;
    uint8_t decider;
    int result;

    /* At least one byte is needed for deciding which decoder to use */
    if (size < 1) {
        return 0;
    }

    decider = data[0] % 2;

    /* Adjust data pointer since the first byte is used */
    data += 1;
    size -= 1;

    /* Fuzz a given decoder */
    if (decider == 0) {
        result = cmt_decode_opentelemetry_create(&decoded_contexts, data, size,
                                                 &off);
        if (result == CMT_DECODE_OPENTELEMETRY_SUCCESS) {
            cmt_decode_opentelemetry_destroy (&decoded_contexts);
        }
    }
    else {
        result = cmt_decode_msgpack_create(&cmt, (char *) data, size, &off);
        if (result == 0) {
            cmt_destroy(cmt);
        }
    }

    return 0;
}
