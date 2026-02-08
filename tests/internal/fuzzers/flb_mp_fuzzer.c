#include <stdint.h>
#include <fluent-bit/flb_mem.h>
#include <fluent-bit/flb_mp.h>

#include "flb_fuzz_header.h"

int LLVMFuzzerTestOneInput(unsigned char *data, size_t size)
{
    /* Set flb_malloc_mod to be fuzzer-data dependent */
    if (size < 5) {
        return 0;
    }
    flb_malloc_p = 0;
    flb_malloc_mod = *(int*)data;
    data += 4;
    size -= 4;

    /* Avoid division by zero for modulo operations */
    if (flb_malloc_mod == 0) {
        flb_malloc_mod = 1;
    }

    unsigned char decider = *data;
    data++;
    size--;

    int out_records;
    size_t processed_bytes;
    if (decider % 2 == 0) {
      flb_mp_validate_log_chunk(data, size, &out_records, &processed_bytes);
    }
    else if (decider % 2 == 1) {
        flb_mp_validate_metric_chunk(data, size, &out_records, &processed_bytes);
    }
    return 0;
}
