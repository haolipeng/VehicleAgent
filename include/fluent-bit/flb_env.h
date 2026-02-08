#ifndef FLB_ENV_H
#define FLB_ENV_H

#include <fluent-bit/flb_info.h>
#include <fluent-bit/flb_sds.h>

#define FLB_ENV_SIZE 64

struct flb_env {
    int warn_unused;        /* warn about unused environment variable */
    struct flb_hash_table *ht;
};

static inline void flb_env_warn_unused(struct flb_env *env, int warn)
{
    env->warn_unused = warn;
}

struct flb_env *flb_env_create();
void flb_env_destroy(struct flb_env *env);
int flb_env_set(struct flb_env *env, const char *key, const char *val);
const char *flb_env_get(struct flb_env *env, const char *key);
flb_sds_t flb_env_var_translate(struct flb_env *env, const char *value);

#endif
