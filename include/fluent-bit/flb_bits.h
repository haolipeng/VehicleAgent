#ifndef FLB_BITS_H
#define FLB_BITS_H

#define FLB_BITS_U64_SET(a, b)   ((uint64_t) a << 32 | b)
#define FLB_BITS_U64_HIGH(val)   ((uint64_t) val >> 32)
#define FLB_BITS_U64_LOW(val)    ((uint64_t) val & 0xffffffff)
#define FLB_BITS_CLEAR(val, n)   (val & ~(1 << n))
#define FLB_BIT_MASK(__TYPE__, __ONE_COUNT__)   \
    ((__TYPE__) (-((__ONE_COUNT__) != 0)))                              \
    & (((__TYPE__) -1) >> ((sizeof(__TYPE__) * CHAR_BIT) - (__ONE_COUNT__)))

#endif
