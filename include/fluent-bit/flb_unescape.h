#ifndef FLB_UNESCAPE_H
#define FLB_UNESCAPE_H

int flb_unescape_string(const char *buf, int buf_len, char **unesc_buf);
int flb_unescape_string_utf8(const char *in_buf, int sz, char *out_buf);
int flb_mysql_unquote_string(char *buf, int buf_len, char **unesc_buf);

#endif
