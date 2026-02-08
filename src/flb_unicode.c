
#include <fluent-bit/flb_unicode.h>
#include <fluent-bit/unicode/flb_conv.h>
#include <stddef.h>

int flb_unicode_convert(int preferred_encoding, const char *input, size_t length,
                        char **output, size_t *out_size)
{
#ifdef FLB_HAVE_UNICODE_ENCODER
    return flb_simdutf_connector_convert_from_unicode(preferred_encoding, input, length,
                                                      output, out_size);
#else
    return FLB_UNICODE_CONVERT_UNSUPPORTED;
#endif
}

int flb_unicode_validate(const char *record, size_t size)
{
#ifdef FLB_HAVE_UNICODE_ENCODER
    return flb_simdutf_connector_validate_utf8(record, size);
#else
    return -1;
#endif
}

int flb_unicode_generic_supported_encoding(const char *encoding_name)
{
    return flb_conv_supported_encoding(encoding_name);
}

int flb_unicode_generic_select_encoding_type(const char *encoding_name)
{
    return flb_conv_select_encoding_type(encoding_name);
}

int flb_unicode_generic_convert_to_utf8(const char *encoding_name,
                                        const unsigned char *input, unsigned char **output, size_t length)
{
    return flb_conv_convert_to_utf8(encoding_name, input, output, length, FLB_FALSE);
}

int flb_unicode_generic_convert_from_utf8(const char *encoding_name,
                                          const unsigned char *input, unsigned char **output, size_t length)
{
    return flb_conv_convert_from_utf8(encoding_name, input, output, length, FLB_FALSE);
}
