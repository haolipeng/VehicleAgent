#include <fluent-bit/flb_info.h>

#if !defined(FLB_PARSER_H) && defined(FLB_HAVE_REGEX)
#define FLB_PARSER_H

#include <fluent-bit/flb_regex.h>
#include <fluent-bit/flb_config.h>
#include <fluent-bit/flb_time.h>
#include <msgpack.h>

#define FLB_PARSER_JSON  2

struct flb_parser_types {
    char *key;
    int  key_len;
    int type;
};

struct flb_parser {
    /* configuration */
    int type;             /* parser type */
    char *name;           /* format name */
    char *p_regex;        /* pattern for main regular expression */
    int skip_empty;       /* skip empty regex matches */
    char *time_fmt;       /* time format */
    char *time_fmt_full;  /* original given time format */
    char *time_key;       /* field name that contains the time */
    int time_offset;      /* fixed UTC offset */
    int time_system_timezone; /* use the system timezone as a fallback */
    int time_keep;        /* keep time field */
    int time_strict;      /* parse time field strictly */
    int logfmt_no_bare_keys; /* in logfmt parsers, require all keys to have values */
    char *time_frac_secs; /* time format have fractional seconds ? */
    struct flb_parser_types *types; /* type casting */
    int types_len;

    /* Field decoders */
    struct mk_list *decoders;

    /* internal */
    int time_with_year;   /* do time_fmt consider a year (%Y) ? */
    char *time_fmt_year;
    int time_with_tz;     /* do time_fmt consider a timezone ?  */
    struct flb_regex *regex;
    struct mk_list _head;
};

enum {
    FLB_PARSER_TYPE_INT = 1,
    FLB_PARSER_TYPE_FLOAT,
    FLB_PARSER_TYPE_BOOL,
    FLB_PARSER_TYPE_STRING,
    FLB_PARSER_TYPE_HEX,
};

static inline time_t flb_parser_tm2time(const struct flb_tm *src,
                                        int use_system_timezone)
{
    struct tm tmp;
    time_t res;

    tmp = src->tm;
    if (use_system_timezone) {
        tmp.tm_isdst = -1;
        res = mktime(&tmp);
    } else {
        res = timegm(&tmp) - flb_tm_gmtoff(src);
    }
    return res;
}


struct flb_parser *flb_parser_create(const char *name, const char *format,
                                     const char *p_regex,
                                     int skip_empty,
                                     const char *time_fmt, const char *time_key,
                                     const char *time_offset,
                                     int time_keep,
                                     int time_strict,
                                     int time_system_timezone,
                                     int logfmt_no_bare_keys,
                                     struct flb_parser_types *types,
                                     int types_len,
                                     struct mk_list *decoders,
                                     struct flb_config *config);
int flb_parser_conf_file_stat(const char *file, struct flb_config *config);
int flb_parser_conf_file(const char *file, struct flb_config *config);
int flb_parser_load_parser_definitions(const char *cfg, struct flb_cf *cf,
                                       struct flb_config *config);
int flb_parser_load_multiline_parser_definitions(const char *cfg, struct flb_cf *cf,
                                                 struct flb_config *config);

void flb_parser_destroy(struct flb_parser *parser);
struct flb_parser *flb_parser_get(const char *name, struct flb_config *config);
int flb_parser_do(struct flb_parser *parser, const char *buf, size_t length,
                  void **out_buf, size_t *out_size, struct flb_time *out_time);

void flb_parser_exit(struct flb_config *config);
int flb_parser_tzone_offset(const char *str, int len, int *tmdiff);
int flb_parser_time_lookup(const char *time, size_t tsize, time_t now,
                           struct flb_parser *parser,
                           struct flb_tm *tm, double *ns);
int flb_parser_typecast(const char *key, int key_len,
                        const char *val, int val_len,
                        msgpack_packer *pck,
                        struct flb_parser_types *types,
                        int types_len);
#endif
