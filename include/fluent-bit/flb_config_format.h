#ifndef FLB_CONFIG_FORMAT_H
#define FLB_CONFIG_FORMAT_H

#include <fluent-bit/flb_info.h>

#include "config_format/flb_cf.h"
#include "config_format/flb_cf_fluentbit.h"

#ifdef FLB_HAVE_LIBYAML
#include "config_format/flb_cf_yaml.h"
#endif

#define FLB_DEFAULT_CF_BUF_SIZE 4096

#endif
