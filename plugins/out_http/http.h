#ifndef FLB_OUT_HTTP_H
#define FLB_OUT_HTTP_H

#define FLB_HTTP_OUT_MSGPACK        FLB_PACK_JSON_FORMAT_NONE
#define FLB_HTTP_OUT_GELF           20

#define FLB_HTTP_CONTENT_TYPE   "Content-Type"
#define FLB_HTTP_MIME_MSGPACK   "application/msgpack"
#define FLB_HTTP_MIME_JSON      "application/json"
#define FLB_HTTP_MIME_NDJSON    "application/x-ndjson"

struct flb_out_http {
    /* HTTP Auth */
    char *http_user;
    char *http_passwd;

    /* Proxy */
    const char *proxy;
    char *proxy_host;
    int proxy_port;

    /* Output format */
    int out_format;
    flb_sds_t format;

    int json_date_format;
    flb_sds_t json_date_key;
    flb_sds_t date_key;        /* internal use */

    /* HTTP URI */
    char *uri;
    char *host;
    int port;

    /* HTTP method */
    int http_method;

    /* GELF fields */
    struct flb_gelf_fields gelf_fields;

    /* which record key to use as body */
    flb_sds_t body_key;

    /* override headers with contents of the map in the key specified here */
    flb_sds_t headers_key;

    /* Include tag in header */
    flb_sds_t header_tag;

    /* Compression mode (gzip) */
    int compress_gzip;
    int compress_zstd;

    /* Allow duplicated headers */
    int allow_dup_headers;

    /* Log the response paylod */
    int log_response_payload;

    /* Response timeout */
    int response_timeout;

    /* Read idle timeout */
    int read_idle_timeout;

    /* Upstream connection to the backend server */
    struct flb_upstream *u;

    /* Arbitrary HTTP headers */
    struct mk_list *headers;

    /* Plugin instance */
    struct flb_output_instance *ins;
};

#endif
