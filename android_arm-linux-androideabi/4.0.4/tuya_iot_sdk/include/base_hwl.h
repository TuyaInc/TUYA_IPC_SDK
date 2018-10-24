#ifndef _BASE_HWL_H
#define _BASE_HWL_H

#include "tuya_cloud_types.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct
{
    CHAR_T ip[16];    /* WIFI的IP地址 xxx.xxx.xxx.xxx */
    CHAR_T mask[16];  /* WIFI的IP子网掩码 xxx.xxx.xxx.xxx */
    CHAR_T gw[16];    /* WIFI的IP网关 xxx.xxx.xxx.xxx */
}BASE_IP_S;

typedef struct
{
    BYTE_T mac[6]; /* WIFI的MAC地址 */
}BASE_MAC_S;

OPERATE_RET hwl_bnw_get_ip(OUT BASE_IP_S *ip);

BOOL_T hwl_bnw_station_conn(VOID);

OPERATE_RET hwl_bnw_set_station_connect(IN CONST CHAR_T *ssid,IN CONST CHAR_T *passwd);

BOOL_T hwl_bnw_need_wifi_cfg(VOID);

OPERATE_RET hwl_bnw_station_get_conn_ap_rssi(OUT SCHAR_T *rssi);

OPERATE_RET hwl_bnw_get_mac(OUT BASE_MAC_S *mac);

OPERATE_RET hwl_bnw_set_mac(IN CONST BASE_MAC_S *mac);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif



