#ifndef FLB_CSV_H
#define FLB_CSV_H

#include <fluent-bit/flb_info.h>
#include <fluent-bit/flb_sds.h>
#include <fluent-bit/flb_time.h>

enum {
    FLB_CSV_SUCCESS = 0,
    FLB_CSV_ALLOC_FAILED = -1,
    FLB_CSV_INVALID_STATE = -2,
    FLB_CSV_EOF = -3,
};

typedef void (*flb_csv_field_parsed_callback)(void *data,
                                              const char *field,
                                              size_t field_len);

struct flb_csv_state {
    flb_csv_field_parsed_callback field_callback;
    flb_sds_t buffered_data;
    flb_sds_t escape_buffer;
    size_t offset;
    size_t start;
    size_t length;
    size_t field_count;
    int state;
    bool field_parsed;
    bool has_dquote;
    void *data;
};

void flb_csv_init(struct flb_csv_state *state,
                  flb_csv_field_parsed_callback field_callback,
                  void *data);

int flb_csv_parse_record(struct flb_csv_state *state,
                         char **bufptr,
                         size_t *buflen,
                         size_t *field_count);

void flb_csv_destroy(struct flb_csv_state *state);


#endif
