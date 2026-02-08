#ifndef FLB_ENGINE_H
#define FLB_ENGINE_H

#include <fluent-bit/flb_info.h>
#include <fluent-bit/flb_config.h>
#include <fluent-bit/flb_input.h>
#include <fluent-bit/flb_output.h>
#include <fluent-bit/flb_thread_storage.h>

#define FLB_ENGINE_OUTPUT_EVENT_BATCH_SIZE 1

int flb_engine_start(struct flb_config *config);
int flb_engine_failed(struct flb_config *config);
int flb_engine_flush(struct flb_config *config,
                     struct flb_input_plugin *in_force);
int flb_engine_exit(struct flb_config *config);
int flb_engine_exit_status(struct flb_config *config, int status);
int flb_engine_shutdown(struct flb_config *config);
int flb_engine_destroy_tasks(struct mk_list *tasks);
void flb_engine_reschedule_retries(struct flb_config *config);
void flb_engine_stop_ingestion(struct flb_config *config);

/* Engine event loop */
void flb_engine_evl_init();
struct mk_event_loop *flb_engine_evl_get();
void flb_engine_evl_set(struct mk_event_loop *evl);

#endif
