#ifndef FLB_RANDOM_H
#define FLB_RANDOM_H

/*
 * Fill buffer with the random bytes. Return 0 on success;
 * -1 on error.
 */
int flb_random_bytes(unsigned char *buf, int len);

#endif
