#ifndef FLB_SDS_LIST_H
#define FLB_SDS_LIST_H

#include <fluent-bit/flb_sds.h>
#include <monkey/mk_core.h>

struct flb_sds_list_entry {
    flb_sds_t str;
    struct mk_list _head;
};

struct flb_sds_list {
    struct mk_list strs;
};

size_t flb_sds_list_size(struct flb_sds_list *list);
struct flb_sds_list *flb_sds_list_create();
int flb_sds_list_destroy(struct flb_sds_list*);
int flb_sds_list_add(struct flb_sds_list*, char*, size_t);
int flb_sds_list_del(struct flb_sds_list_entry*);
int flb_sds_list_del_last_entry(struct flb_sds_list*);
int flb_sds_list_destroy_str_array(char **array);
char **flb_sds_list_create_str_array(struct flb_sds_list *list);

#endif
