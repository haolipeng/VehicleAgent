
/*
 * Integration tests for TCP input pipeline.
 * Uses external TCP socket to send JSON → in_tcp receives →
 * out_tcp + flb_output_set_test("formatter", cb) verifies data.
 *
 * No dependency on in_lib/out_lib.
 */

#include <fluent-bit.h>
#include <fluent-bit/flb_sds.h>
#include <fluent-bit/flb_time.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "flb_tests_runtime.h"

#define TCP_TEST_PORT  "15170"
#define TCP_TEST_HOST  "127.0.0.1"

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

/* Store the last received data for content verification */
static pthread_mutex_t data_mutex = PTHREAD_MUTEX_INITIALIZER;
static char last_output[8192];
static size_t last_output_len = 0;

static void store_output(const char *data, size_t len)
{
    pthread_mutex_lock(&data_mutex);
    if (len >= sizeof(last_output)) {
        len = sizeof(last_output) - 1;
    }
    memcpy(last_output, data, len);
    last_output[len] = '\0';
    last_output_len = len;
    pthread_mutex_unlock(&data_mutex);
}

static size_t get_last_output(char *buf, size_t buf_size)
{
    size_t len;
    pthread_mutex_lock(&data_mutex);
    len = last_output_len;
    if (len >= buf_size) {
        len = buf_size - 1;
    }
    memcpy(buf, last_output, len);
    buf[len] = '\0';
    pthread_mutex_unlock(&data_mutex);
    return len;
}

static void clear_last_output()
{
    pthread_mutex_lock(&data_mutex);
    last_output[0] = '\0';
    last_output_len = 0;
    pthread_mutex_unlock(&data_mutex);
}

/* Callback for out_tcp formatter test mode */
static void cb_check_result(void *ctx, int ffd, int res_ret,
                            void *res_data, size_t res_size, void *data)
{
    int num = get_output_num();
    flb_sds_t out_line = res_data;

    if (res_data != NULL) {
        store_output(out_line, flb_sds_len(out_line));
    }
    set_output_num(num + 1);

    flb_sds_destroy(out_line);
}

/* Helper: connect a TCP socket to the given host:port, with retries */
static int connect_tcp(const char *host, int port, int retries)
{
    int fd;
    struct sockaddr_in addr;
    int i;
    int ret;

    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    inet_pton(AF_INET, host, &addr.sin_addr);

    for (i = 0; i < retries; i++) {
        fd = socket(AF_INET, SOCK_STREAM, 0);
        if (fd < 0) {
            flb_time_msleep(200);
            continue;
        }

        ret = connect(fd, (struct sockaddr *)&addr, sizeof(addr));
        if (ret == 0) {
            return fd;
        }
        close(fd);
        flb_time_msleep(200);
    }

    return -1;
}

/* ---- Test: TCP client sends JSON → in_tcp receives → verify data ---- */
void flb_test_tcp_input_json()
{
    int ret;
    int in_ffd;
    int out_ffd;
    int fd;
    int num;
    flb_ctx_t *ctx;
    char buf[4096];
    const char *json_msg = "{\"sensor\": \"temperature\", \"value\": 42}\n";

    clear_output_num();
    clear_last_output();

    ctx = flb_create();
    flb_service_set(ctx,
                    "Flush", "0.2",
                    "Grace", "1",
                    "Log_Level", "error",
                    NULL);

    /* TCP input listens on port */
    in_ffd = flb_input(ctx, (char *) "tcp", NULL);
    TEST_CHECK(in_ffd >= 0);
    flb_input_set(ctx, in_ffd,
                  "tag", "tcp_test",
                  "listen", TCP_TEST_HOST,
                  "port", TCP_TEST_PORT,
                  "format", "json",
                  NULL);

    /* out_tcp with formatter test mode (out_stdout does not support test_formatter) */
    out_ffd = flb_output(ctx, (char *) "tcp", NULL);
    TEST_CHECK(out_ffd >= 0);
    ret = flb_output_set(ctx, out_ffd,
                         "match", "*",
                         "format", "json",
                         NULL);
    TEST_CHECK(ret == 0);

    ret = flb_output_set_test(ctx, out_ffd,
                              "formatter", cb_check_result,
                              NULL, NULL);
    TEST_CHECK(ret == 0);

    ret = flb_start(ctx);
    TEST_CHECK(ret == 0);

    /* Wait for TCP listener to be ready */
    flb_time_msleep(500);

    /* Connect and send JSON */
    fd = connect_tcp(TCP_TEST_HOST, atoi(TCP_TEST_PORT), 10);
    if (!TEST_CHECK(fd >= 0)) {
        TEST_MSG("failed to connect to in_tcp");
        flb_stop(ctx);
        flb_destroy(ctx);
        return;
    }

    ret = send(fd, json_msg, strlen(json_msg), 0);
    TEST_CHECK(ret > 0);
    close(fd);

    /* Wait for data to flow through the pipeline */
    flb_time_msleep(1500);

    num = get_output_num();
    if (!TEST_CHECK(num > 0)) {
        TEST_MSG("no outputs received");
    }

    /* Verify content */
    get_last_output(buf, sizeof(buf));
    if (!TEST_CHECK(strstr(buf, "sensor") != NULL)) {
        TEST_MSG("expected 'sensor' in output, got: %s", buf);
    }
    if (!TEST_CHECK(strstr(buf, "temperature") != NULL)) {
        TEST_MSG("expected 'temperature' in output, got: %s", buf);
    }

    flb_stop(ctx);
    flb_destroy(ctx);
}

/* ---- Test: Multiple JSON messages sent over TCP ---- */
void flb_test_tcp_input_multi_msg()
{
    int ret;
    int in_ffd;
    int out_ffd;
    int fd;
    int num;
    flb_ctx_t *ctx;
    const char *msg1 = "{\"seq\": 1, \"data\": \"first\"}\n";
    const char *msg2 = "{\"seq\": 2, \"data\": \"second\"}\n";
    const char *msg3 = "{\"seq\": 3, \"data\": \"third\"}\n";

    clear_output_num();
    clear_last_output();

    ctx = flb_create();
    flb_service_set(ctx,
                    "Flush", "0.2",
                    "Grace", "1",
                    "Log_Level", "error",
                    NULL);

    in_ffd = flb_input(ctx, (char *) "tcp", NULL);
    TEST_CHECK(in_ffd >= 0);
    flb_input_set(ctx, in_ffd,
                  "tag", "tcp_test",
                  "listen", TCP_TEST_HOST,
                  "port", TCP_TEST_PORT,
                  "format", "json",
                  NULL);

    out_ffd = flb_output(ctx, (char *) "tcp", NULL);
    TEST_CHECK(out_ffd >= 0);
    ret = flb_output_set(ctx, out_ffd,
                         "match", "*",
                         "format", "json",
                         NULL);
    TEST_CHECK(ret == 0);

    ret = flb_output_set_test(ctx, out_ffd,
                              "formatter", cb_check_result,
                              NULL, NULL);
    TEST_CHECK(ret == 0);

    ret = flb_start(ctx);
    TEST_CHECK(ret == 0);

    flb_time_msleep(500);

    fd = connect_tcp(TCP_TEST_HOST, atoi(TCP_TEST_PORT), 10);
    if (!TEST_CHECK(fd >= 0)) {
        TEST_MSG("failed to connect to in_tcp");
        flb_stop(ctx);
        flb_destroy(ctx);
        return;
    }

    /* Send three messages */
    ret = send(fd, msg1, strlen(msg1), 0);
    TEST_CHECK(ret > 0);
    ret = send(fd, msg2, strlen(msg2), 0);
    TEST_CHECK(ret > 0);
    ret = send(fd, msg3, strlen(msg3), 0);
    TEST_CHECK(ret > 0);
    close(fd);

    /* Wait for pipeline to process */
    flb_time_msleep(2000);

    num = get_output_num();
    if (!TEST_CHECK(num > 0)) {
        TEST_MSG("expected outputs, got %d", num);
    }

    flb_stop(ctx);
    flb_destroy(ctx);
}

/* ---- Test: Large JSON payload over TCP ---- */
void flb_test_tcp_input_large_payload()
{
    int ret;
    int in_ffd;
    int out_ffd;
    int fd;
    int num;
    flb_ctx_t *ctx;
    char *large_msg;
    int i;
    size_t msg_len;

    clear_output_num();
    clear_last_output();

    /* Build a ~4KB JSON payload */
    /* {"bigkey": "AAAAAA...AAA"}\n */
    msg_len = 4096 + 64;
    large_msg = flb_malloc(msg_len);
    if (!TEST_CHECK(large_msg != NULL)) {
        TEST_MSG("malloc failed");
        return;
    }
    strcpy(large_msg, "{\"bigkey\": \"");
    for (i = strlen(large_msg); i < 4096 + (int)strlen("{\"bigkey\": \""); i++) {
        large_msg[i] = 'A';
    }
    large_msg[i] = '\0';
    strcat(large_msg, "\"}\n");

    ctx = flb_create();
    flb_service_set(ctx,
                    "Flush", "0.2",
                    "Grace", "1",
                    "Log_Level", "error",
                    NULL);

    in_ffd = flb_input(ctx, (char *) "tcp", NULL);
    TEST_CHECK(in_ffd >= 0);
    flb_input_set(ctx, in_ffd,
                  "tag", "tcp_test",
                  "listen", TCP_TEST_HOST,
                  "port", TCP_TEST_PORT,
                  "format", "json",
                  "buffer_size", "8192",
                  "chunk_size", "8192",
                  NULL);

    out_ffd = flb_output(ctx, (char *) "tcp", NULL);
    TEST_CHECK(out_ffd >= 0);
    ret = flb_output_set(ctx, out_ffd,
                         "match", "*",
                         "format", "json",
                         NULL);
    TEST_CHECK(ret == 0);

    ret = flb_output_set_test(ctx, out_ffd,
                              "formatter", cb_check_result,
                              NULL, NULL);
    TEST_CHECK(ret == 0);

    ret = flb_start(ctx);
    TEST_CHECK(ret == 0);

    flb_time_msleep(500);

    fd = connect_tcp(TCP_TEST_HOST, atoi(TCP_TEST_PORT), 10);
    if (!TEST_CHECK(fd >= 0)) {
        TEST_MSG("failed to connect to in_tcp");
        flb_free(large_msg);
        flb_stop(ctx);
        flb_destroy(ctx);
        return;
    }

    ret = send(fd, large_msg, strlen(large_msg), 0);
    TEST_CHECK(ret > 0);
    close(fd);

    flb_time_msleep(2000);

    num = get_output_num();
    if (!TEST_CHECK(num > 0)) {
        TEST_MSG("no outputs for large payload");
    }

    /* Verify the large key is present */
    {
        char check_buf[256];
        get_last_output(check_buf, sizeof(check_buf));
        if (!TEST_CHECK(strstr(check_buf, "bigkey") != NULL)) {
            TEST_MSG("expected 'bigkey' in output, got: %s", check_buf);
        }
    }

    flb_free(large_msg);
    flb_stop(ctx);
    flb_destroy(ctx);
}

/* Test list */
TEST_LIST = {
    {"tcp_input_json",         flb_test_tcp_input_json},
    {"tcp_input_multi_msg",    flb_test_tcp_input_multi_msg},
    {"tcp_input_large_payload", flb_test_tcp_input_large_payload},
    {NULL, NULL}
};
