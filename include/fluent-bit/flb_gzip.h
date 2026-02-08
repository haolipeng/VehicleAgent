#ifndef FLB_GZIP_H
#define FLB_GZIP_H

#include <fluent-bit/flb_info.h>
#include <fluent-bit/flb_macros.h>
#include <monkey/mk_http.h>

struct flb_decompression_context;

int flb_gzip_compress(void *in_data, size_t in_len,
                      void **out_data, size_t *out_len);
int flb_gzip_uncompress(void *in_data, size_t in_len,
                        void **out_data, size_t *out_size);

/*
 * Uncompress a gzip payload with trailing data. On success in_remaining is set to the number
 * of input bits not part of the first gzip payload.
 */
int flb_gzip_uncompress_multi(void *in_data, size_t in_len,
                        void **out_data, size_t *out_size, size_t *in_remaining);

void *flb_gzip_decompression_context_create();
void flb_gzip_decompression_context_destroy(void *context);

int flb_gzip_decompressor_dispatch(struct flb_decompression_context *context,
                                   void *out_data, size_t *out_size);

int flb_is_http_session_gzip_compressed(struct mk_http_session *session);

#endif
