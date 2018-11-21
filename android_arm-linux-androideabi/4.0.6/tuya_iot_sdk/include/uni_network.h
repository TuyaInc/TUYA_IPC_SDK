#ifndef _TY_HTTP_H
#define _TY_HTTP_H

#include "tuya_cloud_types.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <sys/time.h>

#ifdef __cplusplus
	extern "C" {
#endif

/** 255.255.255.255 */
#define TY_IPADDR_NONE         ((UINT_T)0xffffffffUL)
/** 127.0.0.1 */
#define TY_IPADDR_LOOPBACK     ((UINT_T)0x7f000001UL)
/** 0.0.0.0 */
#define TY_IPADDR_ANY          ((UINT_T)0x00000000UL)
/** 255.255.255.255 */
#define TY_IPADDR_BROADCAST    ((UINT_T)0xffffffffUL)

typedef enum {
    PROTOCOL_TCP = 0,
    PROTOCOL_UDP = 1,
}UNW_PROTOCOL_TYPE;

typedef enum {
    TRANS_RECV = 0,
    TRANS_SEND = 1,
}UNW_TRANS_TYPE_E;

// system errno
typedef INT_T UNW_ERRNO_T;
#define UNW_SUCCESS       0
#define UNW_FAIL          -1
#define UNW_EINTR         -2
#define UNW_EBADF         -3
#define UNW_EAGAIN        -4
#define UNW_EFAULT        -5
#define UNW_EBUSY         -6
#define UNW_EINVAL        -7
#define UNW_ENFILE        -8
#define UNW_EMFILE        -9
#define UNW_ENOSPC        -10
#define UNW_EPIPE         -11
#define UNW_EWOULDBLOCK   -12
#define UNW_ENOTSOCK      -13
#define UNW_ENOPROTOOPT   -14
#define UNW_EADDRINUSE    -15
#define UNW_EADDRNOTAVAIL -16
#define UNW_ENETDOWN      -17
#define UNW_ENETUNREACH   -18
#define UNW_ENETRESET     -19
#define UNW_ECONNRESET    -20
#define UNW_ENOBUFS       -21
#define UNW_EISCONN       -22
#define UNW_ENOTCONN      -23
#define UNW_ETIMEDOUT     -24
#define UNW_ECONNREFUSED  -25
#define UNW_EHOSTDOWN     -26
#define UNW_EHOSTUNREACH  -27
#define UNW_ENOMEM        -28
#define UNW_EMSGSIZE      -29

/*
FD相关操作
*/
typedef fd_set UNW_FD_SET;
// fd operation used for select
#define UNW_FD_SET(n,p) FD_SET(n, p) 
#define UNW_FD_CLR(n, p) FD_CLR(n, p) 
#define UNW_FD_ISSET(n,p) FD_ISSET(n,p) 
#define UNW_FD_ZERO(p) FD_ZERO(p)

typedef struct 
{
    CHAR_T ip[16];    /* WIFI的IP地址 xxx.xxx.xxx.xxx */
    CHAR_T mask[16];  /* WIFI的IP子网掩码 xxx.xxx.xxx.xxx    */
    CHAR_T gw[16];    /* WIFI的IP网关 xxx.xxx.xxx.xxx  */
}NW_IP_S;

typedef struct 
{
    BYTE_T mac[6]; /* WIFI的MAC地址 */
}NW_MAC_S;


// ip addr
typedef UINT_T UNW_IP_ADDR_T;

// inet to addr
#define UNW_INET_ADDR(x) inet_addr(x)

typedef struct addrinfo ADDRINFO;


UNW_ERRNO_T unw_get_errno(VOID);
INT_T unw_select(IN CONST INT_T maxfd,INOUT UNW_FD_SET *readfds,INOUT UNW_FD_SET *writefds,\
               OUT UNW_FD_SET *errorfds,IN CONST UINT_T ms_timeout);

INT_T unw_get_nonblock(IN CONST INT_T fd);
INT_T unw_set_block(IN CONST INT_T fd,IN CONST BOOL_T block);
INT_T unw_close(IN CONST INT_T fd);
INT_T unw_shutdown(IN CONST INT_T fd,IN CONST INT_T how);
INT_T unw_socket_create(IN CONST UNW_PROTOCOL_TYPE type);
INT_T unw_connect(IN CONST INT_T fd,IN CONST UNW_IP_ADDR_T addr,IN CONST USHORT_T port);
INT_T unw_connect_raw(IN CONST INT_T fd, void *p_socket,IN CONST INT_T len);
INT_T unw_bind(IN CONST INT_T fd,IN CONST UNW_IP_ADDR_T addr,IN CONST USHORT_T port);
INT_T unw_listen(IN CONST INT_T fd,IN CONST INT_T backlog);
INT_T unw_active(IN CONST INT_T fd,OUT UNW_IP_ADDR_T *addr,OUT USHORT_T *port);
INT_T unw_send(IN CONST INT_T fd, IN CONST VOID *buf, IN CONST UINT_T nbytes);
INT_T unw_send_to(IN CONST INT_T fd, IN CONST VOID *buf, IN CONST UINT_T nbytes,\
                IN CONST UNW_IP_ADDR_T addr,IN CONST USHORT_T port);
INT_T unw_recv(IN CONST INT_T fd, OUT VOID *buf, IN CONST UINT_T nbytes);
INT_T unw_recvfrom(IN CONST INT_T fd,OUT VOID *buf,IN CONST UINT_T nbytes,\
                 OUT UNW_IP_ADDR_T *addr,OUT USHORT_T *port);
INT_T unw_set_timeout(IN CONST INT_T fd,IN CONST INT_T ms_timeout,\
                    IN CONST UNW_TRANS_TYPE_E type);
INT_T unw_set_bufsize(IN CONST INT_T fd,IN CONST INT_T buf_size,\
                    IN CONST UNW_TRANS_TYPE_E type);
INT_T unw_set_reuse(IN CONST INT_T fd);
INT_T unw_disable_nagle(IN CONST INT_T fd);
INT_T unw_set_boardcast(IN CONST INT_T fd);
INT_T unw_gethostbyname(IN CONST CHAR_T *domain,OUT UNW_IP_ADDR_T *addr);
INT_T unw_getaddrinfo(IN CONST CHAR_T *hostname, IN CONST CHAR_T *service,
				IN CONST UNW_PROTOCOL_TYPE type, OUT ADDRINFO** res);
INT_T unw_getsockname(IN INT_T fd, OUT INT_T *len);
UNW_IP_ADDR_T unw_str2addr(IN CHAR_T *ip_str);
INT_T unw_accept(IN CONST INT_T fd,OUT UNW_IP_ADDR_T *addr,OUT USHORT_T *port);
INT_T unw_recv_nd_size(IN CONST INT_T fd, OUT VOID *buf, \
                             IN CONST UINT_T buf_size,IN CONST UINT_T nd_size);
INT_T unw_set_keepalive(IN INT_T fd,IN CONST BOOL_T alive,\
                              IN CONST UINT_T idle,IN CONST UINT_T intr,\
                              IN CONST UINT_T cnt);
INT_T unw_socket_bind(IN INT_T fd,IN CONST CHAR_T *ip);
INT_T unw_set_cloexec(IN CONST INT_T fd);

#ifdef __cplusplus
}
#endif
#endif

