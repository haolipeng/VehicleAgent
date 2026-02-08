#ifndef FLB_CUSTOM_PLUGIN_H
#define FLB_CUSTOM_PLUGIN_H

#include <fluent-bit/flb_info.h>
#include <fluent-bit/flb_custom.h>
#include <fluent-bit/flb_log.h>

#define flb_plg_error(ctx, fmt, ...)                                    \
    if (flb_log_check_level(ctx->log_level, FLB_LOG_ERROR))             \
        flb_log_print(FLB_LOG_ERROR, NULL, 0, "[custom:%s:%s] " fmt,    \
                      ctx->p->name, flb_custom_name(ctx), ##__VA_ARGS__)

#define flb_plg_warn(ctx, fmt, ...)                                     \
    if (flb_log_check_level(ctx->log_level, FLB_LOG_WARN))              \
        flb_log_print(FLB_LOG_WARN, NULL, 0, "[custom:%s:%s] " fmt,     \
                      ctx->p->name, flb_custom_name(ctx), ##__VA_ARGS__)

#define flb_plg_info(ctx, fmt, ...)                                     \
    if (flb_log_check_level(ctx->log_level, FLB_LOG_INFO))              \
        flb_log_print(FLB_LOG_INFO, NULL, 0, "[custom:%s:%s] " fmt,     \
                      ctx->p->name, flb_custom_name(ctx), ##__VA_ARGS__)

#define flb_plg_debug(ctx, fmt, ...)                                    \
    if (flb_log_check_level(ctx->log_level, FLB_LOG_DEBUG))             \
        flb_log_print(FLB_LOG_DEBUG, NULL, 0, "[custom:%s:%s] " fmt,    \
                      ctx->p->name, flb_custom_name(ctx), ##__VA_ARGS__)

#define flb_plg_trace(ctx, fmt, ...)                                    \
    if (flb_log_check_level(ctx->log_level, FLB_LOG_TRACE))             \
        flb_log_print(FLB_LOG_TRACE, NULL, 0,                           \
                      "[custom:%s:%s at %s:%i] " fmt,                   \
                      ctx->p->name, flb_custom_name(ctx), __FLB_FILENAME__, \
                      __LINE__, ##__VA_ARGS__)
#endif
