/*********************************************************************************
  *Copyright(C),2015-2020, 涂鸦科技 www.tuya.comm
  *FileName:    tuya_linux_wifi.h
  *
  * 请勿随意修改该文件中的任何内容，如需修改请联系涂鸦产品经理！！
  * 文件描述：
  * 1. WIFI操作API定义
**********************************************************************************/


#ifndef TUYA_LINUX_WIFI_H
#define TUYA_LINUX_WIFI_H

#include "tuya_ipc_api.h"
#include "wf_basic_intf.h"

#ifdef __cplusplus
extern "C" {
#endif


OPERATE_RET hwl_wf_all_ap_scan(OUT AP_IF_S **ap_ary,OUT UINT_T *num);
OPERATE_RET hwl_wf_assign_ap_scan(IN CONST CHAR_T *ssid,OUT AP_IF_S **ap);
OPERATE_RET hwl_wf_release_ap(IN AP_IF_S *ap);
OPERATE_RET hwl_wf_set_cur_channel(IN CONST BYTE_T chan);
OPERATE_RET hwl_wf_get_cur_channel(OUT BYTE_T *chan);
OPERATE_RET hwl_wf_sniffer_set(IN CONST BOOL_T en,IN CONST SNIFFER_CALLBACK cb);
OPERATE_RET hwl_wf_get_ip(IN CONST WF_IF_E wf,OUT NW_IP_S *ip);
OPERATE_RET hwl_wf_get_mac(IN CONST WF_IF_E wf,INOUT NW_MAC_S *mac);
OPERATE_RET hwl_wf_set_mac(IN CONST WF_IF_E wf,IN CONST NW_MAC_S *mac);
OPERATE_RET hwl_wf_wk_mode_set(IN CONST WF_WK_MD_E mode);
OPERATE_RET hwl_wf_wk_mode_get(OUT WF_WK_MD_E *mode);
OPERATE_RET hwl_wf_station_connect(IN CONST CHAR_T *ssid,IN CONST CHAR_T *passwd);
OPERATE_RET hwl_wf_station_disconnect(VOID);
OPERATE_RET hwl_wf_station_get_conn_ap_rssi(OUT SCHAR_T *rssi);
OPERATE_RET hwl_wf_station_stat_get(OUT WF_STATION_STAT_E *stat);
OPERATE_RET hwl_wf_ap_start(IN CONST WF_AP_CFG_IF_S *cfg);
OPERATE_RET hwl_wf_ap_stop(VOID);

#ifdef __cplusplus
}
#endif

#endif // TUYA_LINUX_WIFI_H
