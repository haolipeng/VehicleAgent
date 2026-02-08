#ifndef FLB_INPUT_PLUGIN_H
#define FLB_INPUT_PLUGIN_H

#include <fluent-bit/flb_info.h>
#include <fluent-bit/flb_input.h>
#include <fluent-bit/flb_input_metric.h>
#include <fluent-bit/flb_log.h>

#define flb_input_plugin_log_suppress_check(ins, fmt, ...) \
    flb_log_suppress_check(ins->log_suppress_interval, fmt, ##__VA_ARGS__)


#define flb_plg_error(ctx, fmt, ...)                                    \
    if (flb_log_check_level(ctx->log_level, FLB_LOG_ERROR))             \
        if (flb_input_plugin_log_suppress_check(ctx, fmt, ##__VA_ARGS__) == FLB_FALSE) \
            flb_log_print(FLB_LOG_ERROR, NULL, 0, "[input:%s:%s] " fmt, \
                          ctx->p->name, flb_input_name(ctx), ##__VA_ARGS__)

#define flb_plg_warn(ctx, fmt, ...)                                     \
    if (flb_log_check_level(ctx->log_level, FLB_LOG_WARN))              \
        if (flb_input_plugin_log_suppress_check(ctx, fmt, ##__VA_ARGS__) == FLB_FALSE) \
            flb_log_print(FLB_LOG_WARN, NULL, 0, "[input:%s:%s] " fmt,  \
                          ctx->p->name, flb_input_name(ctx), ##__VA_ARGS__)

#define flb_plg_info(ctx, fmt, ...)                                     \
    if (flb_log_check_level(ctx->log_level, FLB_LOG_INFO))                             \
        if (flb_input_plugin_log_suppress_check(ctx, fmt, ##__VA_ARGS__) == FLB_FALSE) \
            flb_log_print(FLB_LOG_INFO, NULL, 0, "[input:%s:%s] " fmt,  \
                          ctx->p->name, flb_input_name(ctx), ##__VA_ARGS__)

#define flb_plg_debug(ctx, fmt, ...)                                    \
    if (flb_log_check_level(ctx->log_level, FLB_LOG_DEBUG))             \
        if (flb_input_plugin_log_suppress_check(ctx, fmt, ##__VA_ARGS__) == FLB_FALSE) \
            flb_log_print(FLB_LOG_DEBUG, NULL, 0, "[input:%s:%s] " fmt, \
                          ctx->p->name, flb_input_name(ctx), ##__VA_ARGS__)

#define flb_plg_trace(ctx, fmt, ...)                                    \
    if (flb_log_check_level(ctx->log_level, FLB_LOG_TRACE))             \
        if (flb_input_plugin_log_suppress_check(ctx, fmt, ##__VA_ARGS__) == FLB_FALSE) \
            flb_log_print(FLB_LOG_TRACE, NULL, 0,                       \
                          "[input:%s:%s at %s:%i] " fmt,                \
                          ctx->p->name, flb_input_name(ctx), __FLB_FILENAME__, \
                          __LINE__, ##__VA_ARGS__)
#endif
