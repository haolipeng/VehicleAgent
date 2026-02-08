#ifndef FLB_NET_DNS_H
#define FLB_NET_DNS_H

struct flb_net_dns {
    struct mk_list lookups;
    struct mk_list lookups_drop;
};

#endif

