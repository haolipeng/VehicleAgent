#ifndef FLB_ML_GROUP_H
#define FLB_ML_GROUP_H

#include <fluent-bit/flb_info.h>
#include <fluent-bit/multiline/flb_ml.h>
#include <fluent-bit/multiline/flb_ml_parser.h>

struct flb_ml_group *flb_ml_group_create(struct flb_ml *ml);
void flb_ml_group_destroy(struct flb_ml_group *group);
int flb_ml_group_add_parser(struct flb_ml *ctx, struct flb_ml_parser_ins *p);

/*
 * Append data to a multiline stream group respecting the configured
 * buffer limit. The length of the appended data might be reduced if
 * the limit is reached.
 */
int flb_ml_group_cat(struct flb_ml_stream_group *group,
                     const char *data, size_t len);

#endif
