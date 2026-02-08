#ifndef FLB_ML_MODE_H
#define FLB_ML_MODE_H

struct flb_ml *flb_ml_mode_create(struct flb_config *config, char *mode,
                                  int flush_ms, char *key);

/* Python language mode */
struct flb_ml *flb_ml_mode_python(struct flb_config *config,
                                  int flush_ms, char *key);

/* Java language mode */
struct flb_ml *flb_ml_mode_java(struct flb_config *config,
                                int flush_ms, char *key);

/* Go language mode */
struct flb_ml *flb_ml_mode_go(struct flb_config *config,
                              int flush_ms, char *key);

#endif
