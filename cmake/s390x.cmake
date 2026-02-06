if(CMAKE_SYSTEM_PROCESSOR MATCHES "^(SystemZ|s390x)")
  message(STATUS "Forcing characters to be signed, as on x86_64.")
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fsigned-char")
endif ()
