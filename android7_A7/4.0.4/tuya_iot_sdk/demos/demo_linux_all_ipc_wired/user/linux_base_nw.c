/***********************************************************
*  File: linux_base_nw.c
*  Author: nzy
*  Date: 20171213
***********************************************************/
#include <stdio.h>
#include <stdlib.h>

#include <string.h>
#include <unistd.h>

#include <net/if.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <pthread.h>
#include "base_nw_intf.h"
#include "uni_log.h"

/***********************************************************
*************************micro define***********************
***********************************************************/
// 实际对应的端口名称
#define NET_DEV "enp0s25"

/***********************************************************
*************************function define********************
***********************************************************/
// 获取对应端口的ip地址
OPERATE_RET hwl_bnw_get_ip(OUT NW_IP_S *ip)
{
    int sock;
    char ipaddr[50];

    struct   sockaddr_in *sin;
    struct   ifreq ifr;

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
         PR_ERR("socket create failse...GetLocalIp!");
         return OPRT_COM_ERROR;
    }

    memset(&ifr, 0, sizeof(ifr));
    strncpy(ifr.ifr_name, NET_DEV, sizeof(ifr.ifr_name) - 1);

    if( ioctl( sock, SIOCGIFADDR, &ifr) < 0 ) {
         PR_ERR("ioctl error");
         close(sock);
         return OPRT_COM_ERROR;
    }

    sin = (struct sockaddr_in *)&ifr.ifr_addr;
    strcpy(ip->ip,inet_ntoa(sin->sin_addr));
    close(sock);

    return OPRT_OK;
}

// 获取对应端口的连接状况
BOOL_T hwl_bnw_station_conn(VOID)
{
    int sock;
    struct   sockaddr_in *sin;
    struct   ifreq ifr;

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
         PR_ERR("socket create failse...GetLocalIp!");
         return OPRT_COM_ERROR;
    }

    memset(&ifr, 0, sizeof(ifr));
    strncpy(ifr.ifr_name, NET_DEV, sizeof(ifr.ifr_name) - 1);

    if(ioctl(sock,SIOCGIFFLAGS,&ifr) < 0) {
        PR_ERR("ioctl error");
        close(sock);
        return FALSE;
    }
    close(sock);

    if(0 == (ifr.ifr_flags & IFF_UP)) { 
        return FALSE;
    }  

    return TRUE;
}

// 若网关为wifi+有线模式,需要用户自己实现wifi连接的函数，配网时app会将指定路由的ssid、passwd传下来
// sdk会自动调用
OPERATE_RET hwl_bnw_set_station_connect(IN CONST CHAR_T *ssid,IN CONST CHAR_T *passwd)
{
    return OPRT_COM_ERROR;
}

// 若网关为wifi+有线模式，返回true，此时app会调用“hwl_bnw_set_station_connect”，将指定路由的ssid、passwd传下来
BOOL_T hwl_bnw_need_wifi_cfg(VOID)
{
    return FALSE;
}

// 若网关为wifi+有线模式，接口返回wifi与路由器的实际的连接信号值，需用户自己实现
OPERATE_RET hwl_bnw_station_get_conn_ap_rssi(OUT SCHAR_T *rssi)
{
    *rssi = 99;

    return OPRT_OK;
}

// 获取设备mac
OPERATE_RET hwl_bnw_get_mac(OUT NW_MAC_S *mac)
{
    int sock;
    struct   sockaddr_in *sin;
    struct   ifreq ifr;

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
         PR_ERR("socket create failse...GetLocalIp!");
         return OPRT_COM_ERROR;
    }

    memset(&ifr, 0, sizeof(ifr));
    strncpy(ifr.ifr_name, NET_DEV, sizeof(ifr.ifr_name) - 1);

    if(ioctl(sock,SIOCGIFHWADDR,&ifr) < 0) {
        PR_ERR("ioctl error errno");
        close(sock);
        return FALSE;
    }
    memcpy(mac->mac,ifr.ifr_hwaddr.sa_data,sizeof(mac->mac));

    PR_DEBUG("WIFI Get MAC %02X-%02X-%02X-%02X-%02X-%02X\r\n",
             mac->mac[0],mac->mac[1],mac->mac[2],mac->mac[3],mac->mac[4],mac->mac[5]);
    close(sock);

    return OPRT_OK;
}

// 无需实现
OPERATE_RET hwl_bnw_set_mac(IN CONST NW_MAC_S *mac)
{
    if(NULL == mac) {
        return OPRT_INVALID_PARM;
    }
    PR_DEBUG("WIFI Set MAC\r\n");

    return OPRT_OK;
}

