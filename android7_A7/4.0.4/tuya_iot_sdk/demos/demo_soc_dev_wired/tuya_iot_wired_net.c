#include "base_hwl.h"
#include <string.h>

// 获取有线网卡的ip地址
OPERATE_RET hwl_bnw_get_ip(OUT BASE_IP_S *ip)
{
    // UserTODO
    strcpy(ip->ip, "192.168.1.218");
    return OPRT_OK;
}

// 硬件是否连接外网
BOOL_T hwl_bnw_station_conn(VOID)
{
    // UserTODO
    return TRUE;
}

// 若硬件形态为wifi+有线模式，而且用户需要连接外部WIFI，那么需要实现连接WIFI回调函数，sdk内部会自动调用
OPERATE_RET hwl_bnw_set_station_connect(IN CONST CHAR_T *ssid,IN CONST CHAR_T *passwd)
{
    // UserTODO
    return OPRT_COM_ERROR;
}

// 若硬件形态为wifi+有线模式，而且用户需要连接外部WIFI，则返回TRUE，否者返回FALSE
BOOL_T hwl_bnw_need_wifi_cfg(VOID)
{
    // UserTODO
    return FALSE;
}

// 若硬件形态为wifi+有线模式，而且用户需要连接外部WIFI，则返回WIFI实际信号强度，否者返回99
OPERATE_RET hwl_bnw_station_get_conn_ap_rssi(OUT SCHAR_T *rssi)
{
    // UserTODO
    *rssi = 99;
    return OPRT_OK;
}

// 获取有线网卡的MAC地址
OPERATE_RET hwl_bnw_get_mac(OUT BASE_MAC_S *mac)
{
    // UserTODO

    mac->mac[0] = 0xc8;
    mac->mac[1] = 0x5b;
    mac->mac[2] = 0x76;
    mac->mac[3] = 0x4d;
    mac->mac[4] = 0x75;
    mac->mac[5] = 0xcd;

    return OPRT_OK;
}

// 当前无需实现
OPERATE_RET hwl_bnw_set_mac(IN CONST BASE_MAC_S *mac)
{
    return OPRT_OK;
}
