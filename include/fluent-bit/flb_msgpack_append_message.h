#ifndef FLB_MSGPACK_APPEND_MESSAGE_H
#define FLB_MSGPACK_APPEND_MESSAGE_H

/* Error codes */
#define FLB_MAP_EXPAND_SUCCESS   0
#define FLB_MAP_NOT_MODIFIED    -1
#define FLB_MAP_EXPANSION_ERROR -2
#define FLB_MAP_EXPANSION_INVALID_VALUE_TYPE -3

#include <fluent-bit/flb_pack.h>

int flb_msgpack_append_message_to_record(char **result_buffer,
                                         size_t *result_size,
                                         flb_sds_t message_key_name,
                                         char *base_object_buffer,
                                         size_t base_object_size,
                                         char *message_buffer,
                                         size_t message_size,
                                         int message_type);
#endif
