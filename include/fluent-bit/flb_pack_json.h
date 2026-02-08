#ifndef FLB_PACK_JSON_H
#define FLB_PACK_JSON_H

#include <fluent-bit/flb_pack.h>

#define FLB_PACK_JSON_BACKEND_JSMN    1
#define FLB_PACK_JSON_BACKEND_YYJSON  2


struct flb_pack_opts {
    /* which backend to use (default, jsmn, yyjson) */
    int backend;

    /* optional: required only for JSMN in streaming mode */
    struct flb_pack_state *state;
};

int flb_pack_json_ext(const char *json, size_t len,
                      char **out_buf, size_t *out_size,
                      int *out_root_type,
                      struct flb_pack_opts *opts);

int flb_pack_json_recs_ext(const char *json, size_t len,
                           char **out_buf, size_t *out_size,
                           int *out_root_type, int *out_records,
                           size_t *consumed, struct flb_pack_opts *opts);

#endif

