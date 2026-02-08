#include <fluent-bit/flb_info.h>
#include <fluent-bit/flb_log.h>
#include <fluent-bit/flb_socket.h>

#ifndef _WIN32

int flb_socket_error(int fd)
{
    int ret;
    int error = 0;
    socklen_t slen = sizeof(error);

    ret = getsockopt(fd, SOL_SOCKET, SO_ERROR, &error, &slen);
    if (ret == -1) {
        flb_debug("[socket] could not validate socket status for #%i (don't worry)",
                  fd);
        return -1;
    }

    if (error != 0) {
        return error;
    }

    return 0;
}

#endif
