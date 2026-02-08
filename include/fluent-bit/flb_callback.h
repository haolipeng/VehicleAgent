#ifndef FLB_CALLBACK_H
#define FLB_CALLBACK_H

#include <fluent-bit/flb_info.h>
#include <fluent-bit/flb_hash_table.h>
#include <fluent-bit/flb_sds.h>

struct flb_callback_entry {
    flb_sds_t name;
    void (*cb)(char *, void *, void *);
    struct mk_list _head;
};

struct flb_callback {
    flb_sds_t name;             /* Context name */
    struct flb_hash_table *ht;  /* Hash table */
    struct mk_list entries;     /* List for callback entries */
    struct flb_config *config;  /* Fluent Bit context */
};

struct flb_callback *flb_callback_create(char *name);
void flb_callback_destroy(struct flb_callback *ctx);
int flb_callback_set(struct flb_callback *ctx, char *name,
                     void (*cb)(char *, void *, void *));

int flb_callback_do(struct flb_callback *ctx, char *name, void *p1, void *p2);
int flb_callback_exists(struct flb_callback *ctx, char *name);

#endif
