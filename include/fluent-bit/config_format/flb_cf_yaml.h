#ifndef FLB_CONFIG_FORMAT_YAML_H
#define FLB_CONFIG_FORMAT_YAML_H

struct flb_cf *flb_cf_yaml_create(struct flb_cf *cf, char *file_path,
                                  char *buf, size_t size);
#endif
