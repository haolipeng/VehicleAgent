#include <fluent-bit/flb_info.h>
#include <fluent-bit/flb_uri.h>
#include <string.h>

#include "flb_tests_internal.h"

void uri_create_destroy()
{
    struct flb_uri *uri;
    const char *uri_str = "https://fluentbit.io";

    uri = flb_uri_create(uri_str);
    if (!TEST_CHECK(uri != NULL)) {
        TEST_MSG("flb_uri_create failed");
        return;
    }

    flb_uri_destroy(uri);
}

void uri_get()
{
    struct flb_uri *uri;
    struct flb_uri_field *field;
    const char *uri_str = "https://fluentbit.io";

    uri = flb_uri_create(uri_str);
    if (!TEST_CHECK(uri != NULL)) {
        TEST_MSG("flb_uri_create failed");
        return;
    }

    field = flb_uri_get(uri, 0);
    if (!TEST_CHECK(field != NULL)) {
        TEST_MSG("flb_uri_get failed");
        return;
    }

    field = flb_uri_get(uri, -1);
    if (!TEST_CHECK(field == NULL)) {
        TEST_MSG("flb_uri_get should fail");
        return;
    }

    flb_uri_destroy(uri);
}

void uri_encode()
{
    flb_sds_t encoded_uri;
    const char *input = "&# ";
    const char *expect = "%26%23%20";

    encoded_uri = flb_uri_encode(input, strlen(input));
    if (!TEST_CHECK(encoded_uri != NULL)) {
        TEST_MSG("flb_uri_encode failed");
        return;
    }

    flb_sds_destroy(encoded_uri);
}

TEST_LIST = {
    { "uri_create_destroy", uri_create_destroy },
    { "uri_get", uri_get },
    { "uri_encode", uri_encode },
    { 0 }
};
