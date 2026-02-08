/*
 * Stub implementation for record accessor functions.
 * These functions are disabled in this build configuration.
 */

#include <fluent-bit/flb_info.h>
#include <fluent-bit/flb_mem.h>
#include <fluent-bit/flb_sds.h>
#include <monkey/mk_core.h>
#include <msgpack.h>

/* Stub structure for flb_record_accessor */
struct flb_record_accessor {
    flb_sds_t pattern;
    struct mk_list subkeys;
};

struct flb_record_accessor *flb_ra_create(char *str, int translate_env)
{
    struct flb_record_accessor *ra;

    (void) translate_env;

    if (!str) {
        return NULL;
    }

    ra = flb_calloc(1, sizeof(struct flb_record_accessor));
    if (!ra) {
        return NULL;
    }

    ra->pattern = flb_sds_create(str);
    if (!ra->pattern) {
        flb_free(ra);
        return NULL;
    }

    mk_list_init(&ra->subkeys);
    return ra;
}

void flb_ra_destroy(struct flb_record_accessor *ra)
{
    if (!ra) {
        return;
    }

    if (ra->pattern) {
        flb_sds_destroy(ra->pattern);
    }

    flb_free(ra);
}

struct flb_ra_value *flb_ra_get_value_object(struct flb_record_accessor *ra,
                                              msgpack_object map)
{
    (void) ra;
    (void) map;
    return NULL;
}

int flb_ra_subkey_count(struct flb_record_accessor *ra)
{
    (void) ra;
    return 0;
}

int flb_ra_get_kv_pair(struct flb_record_accessor *ra, msgpack_object map,
                       msgpack_object **start_key,
                       msgpack_object **out_key, msgpack_object **out_val)
{
    (void) ra;
    (void) map;
    (void) start_key;
    (void) out_key;
    (void) out_val;
    return -1;
}

void flb_ra_key_value_destroy(struct flb_ra_value *v)
{
    if (!v) {
        return;
    }
    flb_free(v);
}
