#ifndef FLB_SLIST_H
#define FLB_SLIST_H

#include <fluent-bit/flb_info.h>
#include <fluent-bit/flb_sds.h>
#include <monkey/mk_core.h>

struct flb_slist_entry {
    flb_sds_t str;
    struct mk_list _head;
};

int flb_slist_create(struct mk_list *list);
int flb_slist_add(struct mk_list *head, const char *str);
int flb_slist_add_n(struct mk_list *head, const char *str, int len);
int flb_slist_add_sds(struct mk_list *head, flb_sds_t str);

void flb_slist_destroy(struct mk_list *list);
int flb_slist_split_string(struct mk_list *list, const char *str,
                           int separator, int max_split);
int flb_slist_split_tokens(struct mk_list *list, const char *str, int max_split);

void flb_slist_dump(struct mk_list *list);
struct flb_slist_entry *flb_slist_entry_get(struct mk_list *list, int n);

#endif
