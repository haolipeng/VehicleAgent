#ifndef FLB_INPUT_BLOB_H
#define FLB_INPUT_BLOB_H

#include <fluent-bit/flb_info.h>
#include <fluent-bit/flb_input.h>
#include <fluent-bit/flb_notification.h>
#include <fluent-bit/flb_log_event_encoder.h>

struct flb_blob_delivery_notification {
    struct flb_notification base;
    cfl_sds_t path;
    int success;
};


struct flb_blob_file {
    cfl_sds_t path;
};

void flb_input_blob_delivery_notification_destroy(void *instance);

int flb_input_blob_file_get_info(msgpack_object map, cfl_sds_t *source,
                                 cfl_sds_t *file_path, size_t *size);
int flb_input_blob_file_register(struct flb_input_instance *ins,
                                 struct flb_log_event_encoder *encoder,
                                 const char *tag, size_t tag_len,
                                 char *file_path, size_t size);
#endif
