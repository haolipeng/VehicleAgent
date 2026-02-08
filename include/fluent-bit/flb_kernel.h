#ifndef FLB_KERNEL_H
#define FLB_KERNEL_H

#include <inttypes.h>
#include <monkey/mk_core.h>

/* Numeric kernel version */
#define FLB_KERNEL_VERSION(a, b, c) (((a) << 16) + ((b) << 8) + (c))

struct flb_kernel {
    uint8_t minor;
    uint8_t major;
    uint8_t patch;
    uint32_t n_version;
    mk_ptr_t s_version;
};

struct flb_kernel *flb_kernel_info();
void flb_kernel_destroy(struct flb_kernel *kernel);

#endif
