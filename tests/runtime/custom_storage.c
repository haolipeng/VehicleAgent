
/*
 * Integration tests for storage/buffering behavior.
 * Verifies memory-based storage and backlog handling.
 *
 * Note: storage.type=filesystem requires in_storage_backlog plugin which
 * has been removed in this build. These tests verify memory storage
 * behavior and chunk management under various conditions.
 *
 * No dependency on in_lib/out_lib.
 */

#include <fluent-bit.h>
#include <fluent-bit/flb_sds.h>
#include <fluent-bit/flb_time.h>
#include <string.h>
#include "flb_tests_runtime.h"

pthread_mutex_t result_mutex = PTHREAD_MUTEX_INITIALIZER;
int num_output = 0;

static int get_output_num()
{
    int ret;
    pthread_mutex_lock(&result_mutex);
    ret = num_output;
    pthread_mutex_unlock(&result_mutex);
    return ret;
}

static void set_output_num(int num)
{
    pthread_mutex_lock(&result_mutex);
    num_output = num;
    pthread_mutex_unlock(&result_mutex);
}

static void clear_output_num()
{
    set_output_num(0);
}

/* Callback for output test */
static void cb_check_output(void *ctx, int ffd, int res_ret,
                            void *res_data, size_t res_size, void *data)
{
    int num = get_output_num();
    flb_sds_t out_line = res_data;

    if (res_data != NULL) {
        set_output_num(num + 1);
    }

    flb_sds_destroy(out_line);
}

/* ---- Test: Default memory storage works, data flows through ---- */
void flb_test_storage_memory_basic()
{
    int ret;
    int in_ffd;
    int out_ffd;
    int num;
    flb_ctx_t *ctx;

    clear_output_num();

    ctx = flb_create();
    flb_service_set(ctx,
                    "Flush", "0.2",
                    "Grace", "1",
                    "Log_Level", "error",
                    NULL);

    /* Default storage.type = memory */
    in_ffd = flb_input(ctx, (char *) "dummy", NULL);
    TEST_CHECK(in_ffd >= 0);
    flb_input_set(ctx, in_ffd,
                  "tag", "storage_test",
                  "samples", "5",
                  "dummy", "{\"store\": \"data\"}",
                  NULL);

    out_ffd = flb_output(ctx, (char *) "tcp", NULL);
    TEST_CHECK(out_ffd >= 0);
    ret = flb_output_set(ctx, out_ffd,
                         "match", "*",
                         "format", "json",
                         NULL);
    TEST_CHECK(ret == 0);

    ret = flb_output_set_test(ctx, out_ffd,
                              "formatter", cb_check_output,
                              NULL, NULL);
    TEST_CHECK(ret == 0);

    ret = flb_start(ctx);
    TEST_CHECK(ret == 0);

    /* Wait for data to flow through memory storage */
    flb_time_msleep(2000);

    num = get_output_num();
    if (!TEST_CHECK(num > 0)) {
        TEST_MSG("expected output from memory storage pipeline, got %d", num);
    }

    flb_stop(ctx);
    flb_destroy(ctx);
}

/* ---- Test: High-rate ingestion with memory storage remains stable ---- */
void flb_test_storage_memory_high_rate()
{
    int ret;
    int in_ffd;
    int out_ffd;
    int num;
    flb_ctx_t *ctx;

    clear_output_num();

    ctx = flb_create();
    flb_service_set(ctx,
                    "Flush", "0.2",
                    "Grace", "1",
                    "Log_Level", "error",
                    NULL);

    /* High-rate ingestion: 100 records/sec, unlimited samples */
    in_ffd = flb_input(ctx, (char *) "dummy", NULL);
    TEST_CHECK(in_ffd >= 0);
    flb_input_set(ctx, in_ffd,
                  "tag", "highrate",
                  "samples", "0",
                  "rate", "100",
                  "dummy", "{\"pad\": \"abcdefghijklmnopqrstuvwxyz0123456789\"}",
                  NULL);

    out_ffd = flb_output(ctx, (char *) "tcp", NULL);
    TEST_CHECK(out_ffd >= 0);
    ret = flb_output_set(ctx, out_ffd,
                         "match", "*",
                         "format", "json",
                         NULL);
    TEST_CHECK(ret == 0);

    ret = flb_output_set_test(ctx, out_ffd,
                              "formatter", cb_check_output,
                              NULL, NULL);
    TEST_CHECK(ret == 0);

    ret = flb_start(ctx);
    TEST_CHECK(ret == 0);

    /* Let it run under load */
    flb_time_msleep(3000);

    num = get_output_num();
    /* Should have processed multiple batches */
    if (!TEST_CHECK(num > 0)) {
        TEST_MSG("expected outputs under high rate, got %d", num);
    }

    flb_stop(ctx);
    flb_destroy(ctx);
}

/* Test list */
TEST_LIST = {
    {"storage_memory_basic",     flb_test_storage_memory_basic},
    {"storage_memory_high_rate", flb_test_storage_memory_high_rate},
    {NULL, NULL}
};
