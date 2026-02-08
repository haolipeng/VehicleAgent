
/*
 * Integration tests for engine lifecycle.
 * Verifies start/stop/restart stability using in_dummy + out_tcp
 * with flb_output_set_test to verify data flow.
 *
 * No dependency on in_lib/out_lib.
 * Uses out_tcp because only out_tcp and out_http support test_formatter.
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

/* Callback for formatter test */
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

/* ---- Test: Engine start/stop immediately, no crash ---- */
void flb_test_engine_start_stop()
{
    int ret;
    int in_ffd;
    int out_ffd;
    flb_ctx_t *ctx;

    ctx = flb_create();
    flb_service_set(ctx,
                    "Flush", "0.2",
                    "Grace", "1",
                    "Log_Level", "error",
                    NULL);

    in_ffd = flb_input(ctx, (char *) "dummy", NULL);
    TEST_CHECK(in_ffd >= 0);
    flb_input_set(ctx, in_ffd,
                  "tag", "test",
                  "samples", "1",
                  "dummy", "{\"msg\": \"lifecycle\"}",
                  NULL);

    out_ffd = flb_output(ctx, (char *) "tcp", NULL);
    TEST_CHECK(out_ffd >= 0);
    flb_output_set(ctx, out_ffd, "match", "*", "format", "json", NULL);

    /* Start and immediately stop */
    ret = flb_start(ctx);
    TEST_CHECK(ret == 0);

    flb_stop(ctx);
    flb_destroy(ctx);
}

/* ---- Test: Engine start, wait for flush, verify output, stop ---- */
void flb_test_engine_start_flush_stop()
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

    in_ffd = flb_input(ctx, (char *) "dummy", NULL);
    TEST_CHECK(in_ffd >= 0);
    flb_input_set(ctx, in_ffd,
                  "tag", "test",
                  "samples", "3",
                  "dummy", "{\"msg\": \"flush_test\"}",
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

    /* Wait for at least one flush cycle */
    flb_time_msleep(1500);

    num = get_output_num();
    if (!TEST_CHECK(num > 0)) {
        TEST_MSG("expected at least one output, got %d", num);
    }

    flb_stop(ctx);
    flb_destroy(ctx);
}

/* ---- Test: Graceful shutdown with Grace=-1 ---- */
void flb_test_engine_graceful_shutdown()
{
    int ret;
    int in_ffd;
    int out_ffd;
    flb_ctx_t *ctx;

    clear_output_num();

    ctx = flb_create();
    flb_service_set(ctx,
                    "Flush", "0.2",
                    "Grace", "-1",
                    "Log_Level", "error",
                    NULL);

    in_ffd = flb_input(ctx, (char *) "dummy", NULL);
    TEST_CHECK(in_ffd >= 0);
    flb_input_set(ctx, in_ffd,
                  "tag", "test",
                  "samples", "2",
                  "dummy", "{\"msg\": \"graceful\"}",
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

    /* Let some data flow */
    flb_time_msleep(1000);

    /* Grace=-1 means wait for all pending tasks to complete */
    flb_stop(ctx);
    flb_destroy(ctx);

    /* If we reach here without hanging or crashing, test passes */
}

/* ---- Test: Multiple pipelines (multiple input/output) ---- */
void flb_test_engine_multiple_pipelines()
{
    int ret;
    int in_ffd1, in_ffd2;
    int out_ffd1, out_ffd2;
    int num;
    flb_ctx_t *ctx;

    clear_output_num();

    ctx = flb_create();
    flb_service_set(ctx,
                    "Flush", "0.2",
                    "Grace", "1",
                    "Log_Level", "error",
                    NULL);

    /* Pipeline 1: dummy → tcp (tag: pipe1) */
    in_ffd1 = flb_input(ctx, (char *) "dummy", NULL);
    TEST_CHECK(in_ffd1 >= 0);
    flb_input_set(ctx, in_ffd1,
                  "tag", "pipe1",
                  "samples", "2",
                  "dummy", "{\"pipeline\": 1}",
                  NULL);

    out_ffd1 = flb_output(ctx, (char *) "tcp", NULL);
    TEST_CHECK(out_ffd1 >= 0);
    ret = flb_output_set(ctx, out_ffd1,
                         "match", "pipe1",
                         "format", "json",
                         NULL);
    TEST_CHECK(ret == 0);

    ret = flb_output_set_test(ctx, out_ffd1,
                              "formatter", cb_check_output,
                              NULL, NULL);
    TEST_CHECK(ret == 0);

    /* Pipeline 2: dummy → tcp (tag: pipe2) */
    in_ffd2 = flb_input(ctx, (char *) "dummy", NULL);
    TEST_CHECK(in_ffd2 >= 0);
    flb_input_set(ctx, in_ffd2,
                  "tag", "pipe2",
                  "samples", "2",
                  "dummy", "{\"pipeline\": 2}",
                  NULL);

    out_ffd2 = flb_output(ctx, (char *) "tcp", NULL);
    TEST_CHECK(out_ffd2 >= 0);
    ret = flb_output_set(ctx, out_ffd2,
                         "match", "pipe2",
                         "format", "json",
                         NULL);
    TEST_CHECK(ret == 0);

    ret = flb_output_set_test(ctx, out_ffd2,
                              "formatter", cb_check_output,
                              NULL, NULL);
    TEST_CHECK(ret == 0);

    ret = flb_start(ctx);
    TEST_CHECK(ret == 0);

    /* Wait for data from both pipelines */
    flb_time_msleep(1500);

    num = get_output_num();
    /* Both pipelines should produce output: at least 2 outputs total */
    if (!TEST_CHECK(num >= 2)) {
        TEST_MSG("expected at least 2 outputs from multiple pipelines, got %d", num);
    }

    flb_stop(ctx);
    flb_destroy(ctx);
}

/* Test list */
TEST_LIST = {
    {"engine_start_stop",          flb_test_engine_start_stop},
    {"engine_start_flush_stop",    flb_test_engine_start_flush_stop},
    {"engine_graceful_shutdown",   flb_test_engine_graceful_shutdown},
    {"engine_multiple_pipelines",  flb_test_engine_multiple_pipelines},
    {NULL, NULL}
};
