#include <fluent-bit/flb_info.h>
#include <fluent-bit/flb_mem.h>
#include <fluent-bit/flb_pack.h>
#include <fluent-bit/flb_utils.h>
#include <fluent-bit/flb_version.h>
#include <fluent-bit/flb_http_server.h>
#include <msgpack.h>

/* Create a JSON buffer with informational data about the running service */
static int endpoint_root(struct flb_hs *hs)
{
    int c;
    flb_sds_t out_buf;
    msgpack_packer mp_pck;
    msgpack_sbuffer mp_sbuf;
    struct mk_list *head;
    struct mk_list *list;
    struct flb_split_entry *entry;

    /* initialize buffers */
    msgpack_sbuffer_init(&mp_sbuf);
    msgpack_packer_init(&mp_pck, &mp_sbuf, msgpack_sbuffer_write);

    msgpack_pack_map(&mp_pck, 1);
    msgpack_pack_str(&mp_pck, 10);
    msgpack_pack_str_body(&mp_pck, "fluent-bit", 10);

    /* entries under fluent-bit parent:
     *
     * - version
     * - edition
     * - built flags
     */
    msgpack_pack_map(&mp_pck, 3);

    /* fluent-bit['version'] */
    msgpack_pack_str(&mp_pck, 7);
    msgpack_pack_str_body(&mp_pck, "version", 7);
    msgpack_pack_str(&mp_pck, sizeof(FLB_VERSION_STR) - 1);
    msgpack_pack_str_body(&mp_pck, FLB_VERSION_STR, sizeof(FLB_VERSION_STR) - 1);

    /* fluent-bit['edition'] */
    msgpack_pack_str(&mp_pck, 7);
    msgpack_pack_str_body(&mp_pck, "edition", 7);
#ifdef FLB_ENTERPRISE
    msgpack_pack_str(&mp_pck, 10);
    msgpack_pack_str_body(&mp_pck, "Enterprise", 10);
#else
    msgpack_pack_str(&mp_pck, 9);
    msgpack_pack_str_body(&mp_pck, "Community", 9);
#endif

    /* fluent-bit['flags'] */
    msgpack_pack_str(&mp_pck, 5);
    msgpack_pack_str_body(&mp_pck, "flags", 5);

    c = 0;
    list = flb_utils_split(FLB_INFO_FLAGS, ' ', -1);
    mk_list_foreach(head, list) {
        entry = mk_list_entry(head, struct flb_split_entry, _head);
        if (strncmp(entry->value, "FLB_", 4) == 0) {
            c++;
        }
    }

    msgpack_pack_array(&mp_pck, c);
    mk_list_foreach(head, list) {
        entry = mk_list_entry(head, struct flb_split_entry, _head);
        if (strncmp(entry->value, "FLB_", 4) == 0) {
            msgpack_pack_str(&mp_pck, entry->len);
            msgpack_pack_str_body(&mp_pck, entry->value, entry->len);
        }
    }
    flb_utils_split_free(list);

    /* export as JSON */
    out_buf = flb_msgpack_raw_to_json_sds(mp_sbuf.data, mp_sbuf.size, FLB_TRUE);
    msgpack_sbuffer_destroy(&mp_sbuf);

    if (out_buf) {
        hs->ep_root_buf  = out_buf;
        hs->ep_root_size = flb_sds_len(out_buf);
    }

    return -1;
}

int flb_hs_endpoints(struct flb_hs *hs)
{
    endpoint_root(hs);
    return 0;
}

/* Release cached data from endpoints */
int flb_hs_endpoints_free(struct flb_hs *hs)
{
    if (hs->ep_root_buf) {
        flb_sds_destroy(hs->ep_root_buf);
    }

    return 0;
}
