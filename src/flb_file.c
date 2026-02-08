
#include <fluent-bit/flb_file.h>
#include <fluent-bit/flb_log.h>
#include <fluent-bit/flb_mem.h>
#include <fluent-bit/flb_sds.h>

#include <stdio.h>

flb_sds_t flb_file_read(const char *path)
{
    long flen;
    FILE *f = NULL;
    flb_sds_t result = NULL;

    f = fopen(path, "rb");
    if (!f) {
        return NULL;
    }

    if (fseek(f, 0, SEEK_END) == -1) {
        goto err;
    }

    flen = ftell(f);
    if (flen < 0) {
        goto err;
    }

    if (fseek(f, 0, SEEK_SET) == -1) {
        goto err;
    }

    result = flb_sds_create_size(flen);
    if (!result) {
        goto err;
    }

    if (flen > 0 && fread(result, flen, 1, f) != 1) {
        goto err;
    }

    result[flen] = 0;
    flb_sds_len_set(result, flen);
    fclose(f);
    return result;

err:
    flb_errno();
    fclose(f);
    if (result) {
        flb_sds_destroy(result);
    }
    return NULL;
}
