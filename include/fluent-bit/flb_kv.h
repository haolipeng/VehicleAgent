#ifndef FLB_KV_H
#define FLB_KV_H

#include <fluent-bit/flb_info.h>
#include <fluent-bit/flb_sds.h>
#include <monkey/mk_core.h>

struct flb_kv {
  flb_sds_t key;
  flb_sds_t val;
  struct mk_list _head;
};

void flb_kv_init(struct mk_list *list);
struct flb_kv *flb_kv_item_create_len(struct mk_list *list,
                                      char *k_buf, size_t k_len,
                                      char *v_buf, size_t v_len);
struct flb_kv *flb_kv_item_create(struct mk_list *list,
                                  char *k_buf, char *v_buf);
struct flb_kv *flb_kv_item_set(struct mk_list *list,
                               char *k_buf, char *v_buf);
void flb_kv_item_destroy(struct flb_kv *kv);
void flb_kv_release(struct mk_list *list);
const char *flb_kv_get_key_value(const char *key, struct mk_list *list);

#endif
