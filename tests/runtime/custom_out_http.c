
/*
 * Integration tests for out_http plugin.
 * Uses in_dummy (no dependency on in_lib/out_lib) + flb_output_set_test
 * to intercept formatted output in a callback.
 */

#include <fluent-bit.h>
#include <fluent-bit/flb_sds.h>
#include <fluent-bit/flb_time.h>
#include <string.h>
#include <float.h>
#include <math.h>
#include <msgpack.h>
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

struct str_list {
    size_t size;
    char **lists;
};

/* Callback to check expected string substrings in formatted output */
static void cb_check_str_list(void *ctx, int ffd, int res_ret,
                              void *res_data, size_t res_size, void *data)
{
    char *p;
    flb_sds_t out_line = res_data;
    int num = get_output_num();
    size_t i;
    struct str_list *l = (struct str_list *)data;

    if (!TEST_CHECK(res_data != NULL)) {
        TEST_MSG("res_data is NULL");
        return;
    }

    if (!TEST_CHECK(l != NULL)) {
        TEST_MSG("l is NULL");
        flb_sds_destroy(out_line);
        return;
    }

    if (!TEST_CHECK(res_ret == 0)) {
        TEST_MSG("callback ret=%d", res_ret);
    }

    for (i = 0; i < l->size; i++) {
        p = strstr(out_line, l->lists[i]);
        if (!TEST_CHECK(p != NULL)) {
            TEST_MSG("  Got   :%s\n  expect:%s", out_line, l->lists[i]);
        }
    }
    set_output_num(num + 1);

    flb_sds_destroy(out_line);
}

static int msgpack_strncmp(char *str, size_t str_len, msgpack_object obj)
{
    int ret = -1;

    if (str == NULL) {
        flb_error("str is NULL");
        return -1;
    }

    switch (obj.type) {
    case MSGPACK_OBJECT_STR:
        if (obj.via.str.size != str_len) {
            return -1;
        }
        ret = strncmp(str, obj.via.str.ptr, str_len);
        break;
    case MSGPACK_OBJECT_POSITIVE_INTEGER:
        {
            unsigned long val = strtoul(str, NULL, 10);
            if (val == (unsigned long)obj.via.u64) {
                ret = 0;
            }
        }
        break;
    case MSGPACK_OBJECT_NEGATIVE_INTEGER:
        {
            long long val = strtoll(str, NULL, 10);
            if (val == (long long)obj.via.i64) {
                ret = 0;
            }
        }
        break;
    case MSGPACK_OBJECT_FLOAT32:
    case MSGPACK_OBJECT_FLOAT64:
        {
            double val = strtod(str, NULL);
            if (fabs(val - obj.via.f64) < DBL_EPSILON) {
                ret = 0;
            }
        }
        break;
    default:
        flb_error("not supported type=%d", obj.type);
    }

    return ret;
}

/* Callback to check expected key-value pairs in msgpack output */
static void cb_check_msgpack_kv(void *ctx, int ffd, int res_ret,
                                void *res_data, size_t res_size, void *data)
{
    msgpack_unpacked result;
    msgpack_object obj;
    size_t off = 0;
    struct str_list *l = (struct str_list *)data;
    int i_map;
    int map_size;
    int i_list;
    int num = get_output_num();

    if (!TEST_CHECK(res_data != NULL)) {
        TEST_MSG("res_data is NULL");
        return;
    }

    if (!TEST_CHECK(data != NULL)) {
        flb_error("data is NULL");
        return;
    }

    msgpack_unpacked_init(&result);
    while (msgpack_unpack_next(&result, res_data, res_size, &off) == MSGPACK_UNPACK_SUCCESS) {
        obj = result.data;
        if (obj.type != MSGPACK_OBJECT_ARRAY || obj.via.array.size != 2) {
            continue;
        }
        obj = obj.via.array.ptr[1];
        if (obj.type != MSGPACK_OBJECT_MAP) {
            continue;
        }
        map_size = obj.via.map.size;
        for (i_map = 0; i_map < map_size; i_map++) {
            if (obj.via.map.ptr[i_map].key.type != MSGPACK_OBJECT_STR) {
                continue;
            }
            for (i_list = 0; i_list < l->size / 2; i_list++) {
                if (msgpack_strncmp(l->lists[i_list*2],
                                    strlen(l->lists[i_list*2]),
                                    obj.via.map.ptr[i_map].key) == 0 &&
                    msgpack_strncmp(l->lists[i_list*2+1],
                                    strlen(l->lists[i_list*2+1]),
                                    obj.via.map.ptr[i_map].val) == 0) {
                    num++;
                }
            }
        }
    }
    set_output_num(num);
    msgpack_unpacked_destroy(&result);
}

/* ---- Test: out_http format=json ---- */
void flb_test_out_http_format_json()
{
    int ret;
    int in_ffd;
    int out_ffd;
    int num;
    flb_ctx_t *ctx;

    char *expected_strs[] = {"key1", "fluent"};
    struct str_list expected = {
        .size = sizeof(expected_strs) / sizeof(char *),
        .lists = &expected_strs[0],
    };

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
                  "samples", "1",
                  "dummy", "{\"key1\": \"fluent\", \"key2\": 123}",
                  NULL);

    out_ffd = flb_output(ctx, (char *) "http", NULL);
    TEST_CHECK(out_ffd >= 0);
    ret = flb_output_set(ctx, out_ffd,
                         "match", "*",
                         "host", "127.0.0.1",
                         "port", "8088",
                         "uri", "/test",
                         "format", "json",
                         NULL);
    TEST_CHECK(ret == 0);

    ret = flb_output_set_test(ctx, out_ffd,
                              "formatter", cb_check_str_list,
                              &expected, NULL);
    TEST_CHECK(ret == 0);

    ret = flb_start(ctx);
    TEST_CHECK(ret == 0);

    flb_time_msleep(1500);

    num = get_output_num();
    if (!TEST_CHECK(num > 0)) {
        TEST_MSG("no outputs");
    }

    flb_stop(ctx);
    flb_destroy(ctx);
}

/* ---- Test: out_http format=msgpack ---- */
void flb_test_out_http_format_msgpack()
{
    int ret;
    int in_ffd;
    int out_ffd;
    int num;
    flb_ctx_t *ctx;

    char *expected_strs[] = {"key1", "fluent", "key2", "123"};
    struct str_list expected = {
        .size = sizeof(expected_strs) / sizeof(char *),
        .lists = &expected_strs[0],
    };

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
                  "samples", "1",
                  "dummy", "{\"key1\": \"fluent\", \"key2\": 123}",
                  NULL);

    out_ffd = flb_output(ctx, (char *) "http", NULL);
    TEST_CHECK(out_ffd >= 0);
    ret = flb_output_set(ctx, out_ffd,
                         "match", "*",
                         "host", "127.0.0.1",
                         "port", "8088",
                         "uri", "/test",
                         "format", "msgpack",
                         NULL);
    TEST_CHECK(ret == 0);

    ret = flb_output_set_test(ctx, out_ffd,
                              "formatter", cb_check_msgpack_kv,
                              &expected, NULL);
    TEST_CHECK(ret == 0);

    ret = flb_start(ctx);
    TEST_CHECK(ret == 0);

    flb_time_msleep(1500);

    num = get_output_num();
    if (!TEST_CHECK(num == expected.size / 2)) {
        TEST_MSG("got %d, expected %lu", num, expected.size / 2);
    }

    flb_stop(ctx);
    flb_destroy(ctx);
}

/* ---- Test: out_http custom headers ---- */
void flb_test_out_http_headers()
{
    int ret;
    int in_ffd;
    int out_ffd;
    int num;
    flb_ctx_t *ctx;

    /* Verify the JSON body is produced (header verification is at HTTP level,
     * but we can at least confirm the formatter runs correctly with headers set) */
    char *expected_strs[] = {"key1", "fluent"};
    struct str_list expected = {
        .size = sizeof(expected_strs) / sizeof(char *),
        .lists = &expected_strs[0],
    };

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
                  "samples", "1",
                  "dummy", "{\"key1\": \"fluent\", \"key2\": 123}",
                  NULL);

    out_ffd = flb_output(ctx, (char *) "http", NULL);
    TEST_CHECK(out_ffd >= 0);
    ret = flb_output_set(ctx, out_ffd,
                         "match", "*",
                         "host", "127.0.0.1",
                         "port", "8088",
                         "uri", "/test",
                         "format", "json",
                         "header", "X-Custom-Header  my-value",
                         "header", "X-Another  another-value",
                         NULL);
    TEST_CHECK(ret == 0);

    ret = flb_output_set_test(ctx, out_ffd,
                              "formatter", cb_check_str_list,
                              &expected, NULL);
    TEST_CHECK(ret == 0);

    ret = flb_start(ctx);
    TEST_CHECK(ret == 0);

    flb_time_msleep(1500);

    num = get_output_num();
    if (!TEST_CHECK(num > 0)) {
        TEST_MSG("no outputs");
    }

    flb_stop(ctx);
    flb_destroy(ctx);
}

/* Test list */
TEST_LIST = {
    {"out_http_format_json",    flb_test_out_http_format_json},
    {"out_http_format_msgpack", flb_test_out_http_format_msgpack},
    {"out_http_headers",        flb_test_out_http_headers},
    {NULL, NULL}
};
