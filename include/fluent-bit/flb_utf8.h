#ifndef FLB_UTF8_H
#define FLB_UTF8_H

#define FLB_UTF8_ACCEPT   0
#define FLB_UTF8_REJECT   1
#define FLB_UTF8_CONTINUE 2

#include <fluent-bit/flb_info.h>
#include <inttypes.h>

/* returns length of next utf-8 sequence */
int flb_utf8_len(const char *s);
uint32_t flb_utf8_decode(uint32_t *state, uint32_t *codep, uint8_t byte);
void flb_utf8_print(char *input);

#endif
