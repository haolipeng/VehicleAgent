#ifndef FLB_MP_CHUNK_H
#define FLB_MP_CHUNK_H

#include <fluent-bit/flb_info.h>
#include <fluent-bit/flb_log_event.h>
#include <cfl/cfl.h>

#define FLB_MP_CHUNK_RECORD_ERROR -1  /* Error while retrieving content */
#define FLB_MP_CHUNK_RECORD_OK     0  /* Content retrieved successfully */
#define FLB_MP_CHUNK_RECORD_EOF    1  /* No more content to retrieve */

/* Forward declaration to avoid circular dependencies */
struct flb_condition;

struct flb_mp_chunk_record {
    int modified;
    struct flb_log_event event;
    struct cfl_object *cobj_metadata;
    struct cfl_object *cobj_record;
    struct cfl_object *cobj_group_metadata;
    struct cfl_object *cobj_group_attributes;
    int owns_group_metadata;
    int owns_group_attributes;
    struct cfl_list _head;
};

struct flb_mp_chunk_cobj {
    int total_records;
    struct flb_log_event_encoder *log_encoder;
    struct flb_log_event_decoder *log_decoder;

    struct flb_mp_chunk_record *record_pos;
    struct cfl_list records;

    struct cfl_object *active_group_metadata;
    struct cfl_object *active_group_attributes;

    /* Condition for filtering records during processing */
    struct flb_condition *condition;
};


struct flb_mp_chunk_record *flb_mp_chunk_record_create(struct flb_mp_chunk_cobj *chunk_cobj);

int flb_mp_chunk_cobj_record_destroy(struct flb_mp_chunk_cobj *chunk_cobj,
                                     struct flb_mp_chunk_record *record);
int flb_mp_chunk_cobj_record_next(struct flb_mp_chunk_cobj *chunk_cobj,
                                  struct flb_mp_chunk_record **out_record);

struct flb_mp_chunk_cobj *flb_mp_chunk_cobj_create(struct flb_log_event_encoder *log_encoder,
                                                   struct flb_log_event_decoder *log_decoder);
int flb_mp_chunk_cobj_destroy(struct flb_mp_chunk_cobj *chunk_cobj);

int flb_mp_chunk_cobj_encode(struct flb_mp_chunk_cobj *chunk_cobj, char **out_buf, size_t *out_size);




#endif
