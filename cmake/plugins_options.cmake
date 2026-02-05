macro(DEFINE_OPTION option_name description default_value)
    set(temp_value ${default_value})
    if(FLB_MINIMAL)
        set(temp_value OFF)
    endif()
    option(${option_name} "${description}" ${temp_value})
endmacro()

# Add the FLB_MINIMAL option
option(FLB_MINIMAL "Enable minimal build configuration" No)

# Inputs (sources, data collectors)
# =================================
DEFINE_OPTION(FLB_IN_CPU                      "Enable CPU input plugin"                      ON)
DEFINE_OPTION(FLB_IN_DUMMY                    "Enable Dummy input plugin"                    ON)
DEFINE_OPTION(FLB_IN_EMITTER                  "Enable emitter input plugin"                  ON)
DEFINE_OPTION(FLB_IN_LIB                      "Enable library mode input plugin"             ON)
DEFINE_OPTION(FLB_IN_MEM                      "Enable Memory input plugin"                   ON)
DEFINE_OPTION(FLB_IN_TCP                      "Enable TCP input plugin"                      ON)
DEFINE_OPTION(FLB_IN_UNIX_SOCKET              "Enable Unix socket input plugin"              ON)

# Filters
# =======
DEFINE_OPTION(FLB_FILTER_MODIFY               "Enable modify filter"                         ON)
DEFINE_OPTION(FLB_FILTER_RECORD_MODIFIER      "Enable record_modifier filter"                ON)
DEFINE_OPTION(FLB_FILTER_STDOUT               "Enable stdout filter"                         ON)

# Outputs (destinations)
# ======================
DEFINE_OPTION(FLB_OUT_FILE                    "Enable file output plugin"                    ON)
DEFINE_OPTION(FLB_OUT_HTTP                    "Enable HTTP output plugin"                    ON)
DEFINE_OPTION(FLB_OUT_LIB                     "Enable library mode output plugin"            ON)
DEFINE_OPTION(FLB_OUT_NULL                    "Enable dev null output plugin"                ON)
DEFINE_OPTION(FLB_OUT_STDOUT                  "Enable STDOUT output plugin"                  ON)
DEFINE_OPTION(FLB_OUT_TCP                     "Enable TCP output plugin"                     ON)
