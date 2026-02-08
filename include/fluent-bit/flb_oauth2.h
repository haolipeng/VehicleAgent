#ifndef FLB_OAUTH2_H
#define FLB_OAUTH2_H

#include <fluent-bit/flb_info.h>
#include <fluent-bit/flb_sds.h>
#include <fluent-bit/tls/flb_tls.h>

#define FLB_OAUTH2_PORT          "443"
#define FLB_OAUTH2_HTTP_ENCODING "application/x-www-form-urlencoded"

struct flb_oauth2 {
    flb_sds_t auth_url;
    flb_sds_t payload;

    /* Parsed URL */
    flb_sds_t host;
    flb_sds_t port;
    flb_sds_t uri;

    /* Token times set by the caller */
    time_t issued;
    time_t expires;

    /* Token info after successful auth */
    flb_sds_t access_token;
    flb_sds_t token_type;
    uint64_t  expires_in;

    /* TLS Context */
#ifdef FLB_HAVE_TLS
    struct flb_tls *tls;
#else
    void *tls;
#endif

    /* Upstream context */
    struct flb_upstream *u;
};

struct flb_oauth2 *flb_oauth2_create(struct flb_config *config,
                                     const char *auth_url, int expire_sec);
void flb_oauth2_destroy(struct flb_oauth2 *ctx);
int flb_oauth2_token_len(struct flb_oauth2 *ctx);
void flb_oauth2_payload_clear(struct flb_oauth2 *ctx);
int flb_oauth2_payload_append(struct flb_oauth2 *ctx,
                              const char *key_str, int key_len,
                              const char *val_str, int val_len);
char *flb_oauth2_token_get_ng(struct flb_oauth2 *ctx);
char *flb_oauth2_token_get(struct flb_oauth2 *ctx);
int flb_oauth2_token_expired(struct flb_oauth2 *ctx);

int flb_oauth2_parse_json_response(const char *json_data, size_t json_size,
                        struct flb_oauth2 *ctx);

#endif
