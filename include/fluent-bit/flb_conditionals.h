#ifndef FLB_CONDITIONS_H
#define FLB_CONDITIONS_H

#include <fluent-bit/flb_info.h>
#include <fluent-bit/flb_sds.h>
#include <fluent-bit/flb_regex.h>
#include <fluent-bit/flb_mp.h>
#include <fluent-bit/flb_cfl_record_accessor.h>
#include <monkey/mk_core.h>
#include <fluent-bit/flb_mp_chunk.h>

struct flb_condition_rule;

typedef struct cfl_variant *(*flb_condition_get_variant_fn)(struct flb_condition_rule *rule,
                                                            void *ctx);

/* Context types enum */
enum record_context_type {
    RECORD_CONTEXT_BODY = 0,
    RECORD_CONTEXT_METADATA = 1,
    RECORD_CONTEXT_GROUP_METADATA,
    RECORD_CONTEXT_GROUP_ATTRIBUTES,
    RECORD_CONTEXT_OTEL_RESOURCE_ATTRIBUTES,
    RECORD_CONTEXT_OTEL_SCOPE_ATTRIBUTES,
    RECORD_CONTEXT_OTEL_SCOPE_METADATA
};

struct flb_condition;

enum flb_condition_operator {
    FLB_COND_OP_AND,
    FLB_COND_OP_OR
};

enum flb_rule_operator {
    FLB_RULE_OP_EQ,
    FLB_RULE_OP_NEQ,
    FLB_RULE_OP_GT,
    FLB_RULE_OP_LT,
    FLB_RULE_OP_GTE,
    FLB_RULE_OP_LTE,
    FLB_RULE_OP_REGEX,
    FLB_RULE_OP_NOT_REGEX,
    FLB_RULE_OP_IN,
    FLB_RULE_OP_NOT_IN
};

struct flb_condition_rule {
    struct flb_cfl_record_accessor *ra;  /* Record accessor for the field */
    enum record_context_type context;    /* Whether rule applies to body or metadata */
    enum flb_rule_operator op;
    union {
        flb_sds_t str_val;
        double num_val;
        struct {
            flb_sds_t *values;
            int count;
        } array;
    } value;
    struct flb_regex *regex;
    struct mk_list _head;
};

struct flb_condition {
    enum flb_condition_operator op;
    struct mk_list rules;
};

/* Core condition functions */
struct flb_condition *flb_condition_create(enum flb_condition_operator op);

int flb_condition_add_rule(struct flb_condition *cond,
                          const char *field,
                          enum flb_rule_operator op,
                          void *value,
                          int value_count,
                          enum record_context_type context);

void flb_condition_destroy(struct flb_condition *cond);

/* Evaluation function */
int flb_condition_evaluate_ex(struct flb_condition *cond,
                             void *ctx,
                             flb_condition_get_variant_fn get_variant);
int flb_condition_evaluate(struct flb_condition *cond,
                          struct flb_mp_chunk_record *record);

#endif /* FLB_CONDITIONS_H */