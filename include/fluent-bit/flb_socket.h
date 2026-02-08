#ifndef FLB_SOCKET_H
#define FLB_SOCKET_H

#include <fluent-bit/flb_compat.h>

#ifdef _WIN32
#include <event.h>

#define flb_sockfd_t         evutil_socket_t

#define flb_socket_close(fd) evutil_closesocket(fd)
#define flb_socket_error(fd) evutil_socket_geterror(fd)

#define FLB_EINPROGRESS(e)   ((e) == WSAEWOULDBLOCK)
#define FLB_WOULDBLOCK()     (WSAGetLastError() == WSAEWOULDBLOCK)

#define FLB_INVALID_SOCKET   ((flb_sockfd_t) -1)
#else
#include <sys/types.h>
#include <sys/socket.h>

#define flb_sockfd_t         int

#define flb_socket_close(fd) close(fd)

#define FLB_EINPROGRESS(e)   ((e) == EINTR || (e) == EINPROGRESS)
#define FLB_WOULDBLOCK()     (errno == EAGAIN || errno == EWOULDBLOCK)

#define FLB_INVALID_SOCKET   ((flb_sockfd_t) -1)

int flb_socket_error(int fd);

#endif

#endif
