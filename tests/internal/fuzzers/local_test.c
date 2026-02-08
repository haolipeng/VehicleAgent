
#include <fluent-bit/flb_info.h>
#include <fluent-bit/flb_log.h>
#include <fluent-bit/flb_mem.h>
#include <monkey/mk_core.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

/* declare external function test */
int LLVMFuzzerTestOneInput(unsigned char *data, size_t size);

int main(int argc, char **argv)
{
    int i;
    int ret;
    FILE *fp;
    char *buffer;
    long bytes;
    struct stat st;

    if (argc < 2) {
        flb_error("usage: %s TESTCASE_FILE", argv[0]);
        exit(EXIT_FAILURE);
    }

    /* Validate the file */
    ret = stat(argv[1], &st);
    if (ret == -1) {
        flb_errno();
        flb_error("cannot stat(2) testcase file '%s'", argv[1]);
        exit(EXIT_FAILURE);
    }

    if (!(fp = fopen(argv[1], "rb"))) {
        flb_errno();
        flb_error("cannot fopen(2) testcase file '%s'", argv[1]);
        return -1;
    }

    buffer = flb_malloc(st.st_size);
    if (!buffer) {
        flb_errno();
        return -1;
    }

    bytes = fread(buffer, st.st_size, 1, fp);
    if (bytes < 1) {
        fclose(fp);
        flb_free(buffer);
        return -1;
    }
    fclose(fp);

    /* Invoke the fuzzer entry-point function */
    for (i = 0; i < 100; i++) {
        ret = LLVMFuzzerTestOneInput((unsigned char *) buffer, st.st_size);
    }
    flb_free(buffer);
    return 0;
}
