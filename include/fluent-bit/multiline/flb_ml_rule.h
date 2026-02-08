#ifndef FLB_ML_RULE_H
#define FLB_ML_RULE_H

#include <fluent-bit/flb_info.h>
#include <fluent-bit/flb_sds.h>
#include <fluent-bit/multiline/flb_ml.h>

int flb_ml_rule_create(struct flb_ml_parser *ml_parser,
                       flb_sds_t from_states,
                       char *regex_pattern,
                       flb_sds_t to_state,
                       char *end_pattern);
void flb_ml_rule_destroy(struct flb_ml_rule *rule);
void flb_ml_rule_destroy_all(struct flb_ml_parser *ml_parser);
int flb_ml_rule_process(struct flb_ml_parser *ml_parser,
                        struct flb_ml_stream *mst,
                        struct flb_ml_stream_group *group,
                        msgpack_object *full_map,
                        void *buf, size_t size, struct flb_time *tm,
                        msgpack_object *val_content,
                        msgpack_object *val_pattern);
int flb_ml_rule_init(struct flb_ml_parser *ml_parser);

#endif
