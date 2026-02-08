
#ifndef FLB_TESTS_INITALIZE_TLS_H
#define FLB_TESTS_INITALIZE_TLS_H

#include <fluent-bit/flb_config.h>
#include <fluent-bit/flb_mem.h>

struct flb_config *test_env_config = NULL;

static inline void flb_test_env_config_init(void)
{
    test_env_config = flb_config_init();

    if (test_env_config == NULL) {
        return;
    }
}

static inline void flb_test_env_config_destroy(void) {
    if (test_env_config != NULL) {
        flb_config_exit(test_env_config);
    }
}

#endif
