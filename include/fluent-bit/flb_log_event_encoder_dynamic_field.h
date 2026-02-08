#ifndef FLB_LOG_EVENT_ENCODER_DYNAMIC_FIELD_H
#define FLB_LOG_EVENT_ENCODER_DYNAMIC_FIELD_H

#include <fluent-bit/flb_time.h>
#include <fluent-bit/flb_sds.h>
#include <fluent-bit/flb_mp.h>
#include <cfl/cfl_list.h>

#include <msgpack.h>
#include <ctype.h>

struct flb_log_event_encoder_dynamic_field_scope {
    size_t                   offset;
    struct flb_mp_map_header header;
    int                      type;
    struct cfl_list          _head;
};

struct flb_log_event_encoder_dynamic_field {
    int                      initialized;
    size_t                   entry_count;
    size_t                   data_offset;
    msgpack_packer           packer;
    msgpack_sbuffer          buffer;
    struct cfl_list          scopes;

    char                    *data;
    size_t                   size;
    int                      type;
};

struct flb_log_event_encoder_dynamic_field_scope *
    flb_log_event_encoder_dynamic_field_scope_current(
        struct flb_log_event_encoder_dynamic_field *field);

int flb_log_event_encoder_dynamic_field_scope_enter(
    struct flb_log_event_encoder_dynamic_field *field,
    int type);

int flb_log_event_encoder_dynamic_field_scope_leave(
    struct flb_log_event_encoder_dynamic_field *field,
    struct flb_log_event_encoder_dynamic_field_scope *scope,
    int commit);

int flb_log_event_encoder_dynamic_field_begin_map(
    struct flb_log_event_encoder_dynamic_field *field);

int flb_log_event_encoder_dynamic_field_begin_array(
    struct flb_log_event_encoder_dynamic_field *field);

int flb_log_event_encoder_dynamic_field_commit_map(
    struct flb_log_event_encoder_dynamic_field *field);

int flb_log_event_encoder_dynamic_field_commit_array(
    struct flb_log_event_encoder_dynamic_field *field);

int flb_log_event_encoder_dynamic_field_rollback_map(
    struct flb_log_event_encoder_dynamic_field *field);

int flb_log_event_encoder_dynamic_field_rollback_array(
    struct flb_log_event_encoder_dynamic_field *field);

int flb_log_event_encoder_dynamic_field_append(
    struct flb_log_event_encoder_dynamic_field *field);

int flb_log_event_encoder_dynamic_field_flush(
    struct flb_log_event_encoder_dynamic_field *field);

int flb_log_event_encoder_dynamic_field_reset(
    struct flb_log_event_encoder_dynamic_field *field);

int flb_log_event_encoder_dynamic_field_init(
    struct flb_log_event_encoder_dynamic_field *field,
    int type);

void flb_log_event_encoder_dynamic_field_destroy(
    struct flb_log_event_encoder_dynamic_field *field);

#endif
