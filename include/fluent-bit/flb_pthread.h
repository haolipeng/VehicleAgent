#ifndef FLB_PTHREAD_H
#define FLB_PTHREAD_H

#include <fluent-bit/flb_info.h>
#ifdef FLB_SYSTEM_WINDOWS
#include <monkey/mk_core/external/winpthreads.h>
#else
#include <pthread.h>
#endif

#endif
