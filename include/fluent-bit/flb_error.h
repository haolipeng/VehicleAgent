#ifndef FLB_ERROR_H
#define FLB_ERROR_H

#define FLB_ERR_CFG_FILE              10
#define FLB_ERR_CFG_FILE_FORMAT       11
#define FLB_ERR_CFG_FILE_STOP         12
#define FLB_ERR_CFG_FLUSH             20
#define FLB_ERR_CFG_FLUSH_CREATE      21
#define FLB_ERR_CFG_FLUSH_REGISTER    22
#define FLB_ERR_CUSTOM_INVALID        49
#define FLB_ERR_INPUT_INVALID         50
#define FLB_ERR_INPUT_UNDEF           51
#define FLB_ERR_INPUT_UNSUP           52
#define FLB_ERR_OUTPUT_UNDEF         100
#define FLB_ERR_OUTPUT_INVALID       101
#define FLB_ERR_OUTPUT_UNIQ          102
#define FLB_ERR_FILTER_INVALID       201
#define FLB_ERR_PROCESSOR_INVALID    202

/* Parser */
#define FLB_ERR_CFG_PARSER_FILE      300

/* Plugin */
#define FLB_ERR_CFG_PLUGIN_FILE      400

/* JSON errors */
#define FLB_ERR_JSON_INVAL           -501
#define FLB_ERR_JSON_PART            -502

/* Coroutine errors */
#define FLB_ERR_CORO_STACK_SIZE      -600

/* Reloading */
#define FLB_ERR_RELOADING_IN_PROGRESS 700

#endif
