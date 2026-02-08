
#ifndef FLB_FILE_H
#define FLB_FILE_H

#include <fluent-bit/flb_sds.h>

flb_sds_t flb_file_read(const char *path);
// TODO int flb_file_write(const char *path, flb_sds_t contents);

#endif
