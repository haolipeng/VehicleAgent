#ifndef FLB_ZSTD_H
#define FLB_ZSTD_H

#include <fluent-bit/flb_info.h>
#include <zstd.h>
#include <zstd_errors.h>

struct flb_decompression_context;

int flb_zstd_compress(void *in_data, size_t in_len, void **out_data, size_t *out_len);
int flb_zstd_uncompress(void *in_data, size_t in_len, void **out_data, size_t *out_len);

int flb_zstd_decompressor_dispatch(struct flb_decompression_context *context,
                                   void *output_buffer,
                                   size_t *output_length);
void *flb_zstd_decompression_context_create(void);
void flb_zstd_decompression_context_destroy(void *context);
#endif
