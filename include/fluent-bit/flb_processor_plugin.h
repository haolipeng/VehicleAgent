#ifndef FLB_PROCESSOR_PLUGIN_H
#define FLB_PROCESSOR_PLUGIN_H

#include <fluent-bit/flb_info.h>
#include <fluent-bit/flb_processor.h>
#include <fluent-bit/flb_log.h>
#include <fluent-bit/flb_log_event_decoder.h>
#include <fluent-bit/flb_mp.h>
#include <fluent-bit/flb_mp_chunk.h>

#define flb_plg_log(ctx, level, fmt, ...)                                \
    if (flb_log_check_level(ctx->log_level, level))                      \
        flb_log_print(level, NULL, 0, "[processor:%s:%s] " fmt,          \
                      ctx->p->name,                                      \
                      flb_processor_instance_get_name(ctx), ##__VA_ARGS__)

#define flb_plg_error(ctx, fmt, ...) \
    flb_plg_log(ctx, FLB_LOG_ERROR, fmt, ##__VA_ARGS__)

#define flb_plg_warn(ctx, fmt, ...)  \
    flb_plg_log(ctx, FLB_LOG_WARN, fmt, ##__VA_ARGS__)

#define flb_plg_info(ctx, fmt, ...)  \
    flb_plg_log(ctx, FLB_LOG_INFO, fmt, ##__VA_ARGS__)

#define flb_plg_debug(ctx, fmt, ...) \
    flb_plg_log(ctx, FLB_LOG_DEBUG, fmt, ##__VA_ARGS__)

#define flb_plg_trace(ctx, fmt, ...) \
    flb_plg_log(ctx, FLB_LOG_TRACE, fmt, ##__VA_ARGS__)
#endif
