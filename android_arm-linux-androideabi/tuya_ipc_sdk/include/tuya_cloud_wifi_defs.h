/*********************************************************************************
  *Copyright(C),2017, 涂鸦科技 www.tuya.comm
  *FileName:    tuya_cloud_wifi_defs.h
**********************************************************************************/


#ifndef TUYA_CLOUD_WIFI_FUNC_H
#define TUYA_CLOUD_WIFI_FUNC_H

#ifdef __cplusplus
extern "C" {
#endif

#include "tuya_cloud_types.h"
#include "tuya_cloud_error_code.h"


#define WIFI_SSID_LEN 32   /**< WIFI名称长度 */
#define WIFI_PASSWD_LEN 64 /**< WIFI密码长度 */
#define WIFI_TOKEN_LEN 16  /**< WIFI令牌长度，用于和TUYA APP校验 */

/**
 * \brief WIFI信息结构体
 * \struct AP_IF_S
 */
typedef struct {
    BYTE channel; /**< AP信道号 */
    SCHAR rssi;   /**< AP信号强度 */
    BYTE bssid[6]; /**< AP BSSID */
    BYTE ssid[WIFI_SSID_LEN+1]; /**< AP名称 */
    BYTE s_len; /**< AP名称长度 */
}AP_IF_S;

/**
 * \typedef typedef VOID (*SNIFFER_CALLBACK)(IN CONST BYTE *buf,IN CONST USHORT len)
 * \brief WIFI空口抓包后TUYA_SDK分析函数入口定义
 * \param[in] buf WIFI空口抓到的包，这里的buf需要包含802.11协议头
 * \param[in] len 包长度
 * \return VOID
 */
typedef VOID (*SNIFFER_CALLBACK)(IN CONST BYTE *buf,IN CONST USHORT len);

/**
 * \brief WIFI工作模式
 * \struct WF_IF_E
 */
typedef enum {
    WF_STATION = 0, /**< WIFI在STATION模式 */
    WF_AP, /**< WIFI在AP模式 */
}WF_IF_E;

/**
 * \brief WIFI信息结构体
 * \struct NW_IP_S
 */
typedef struct {
    CHAR ip[16]; /**< WIFI的IP地址 xxx.xxx.xxx.xxx */
    CHAR mask[16]; /**< WIFI的IP子网掩码 xxx.xxx.xxx.xxx */
    CHAR gw[16]; /**< WIFI的IP网关 xxx.xxx.xxx.xxx */
}NW_IP_S;

/**
 * \brief MAC信息结构体
 * \struct NW_MAC_S
 */
typedef struct {
    BYTE mac[6]; /**< WIFI的MAC地址 */
}NW_MAC_S;

/**
 * \brief WIFI工作模式
 * \enum WF_WK_MD_E
 */
typedef enum {
    WWM_LOWPOWER = 0, /**< WIFI工作在低功耗模式 */
    WWM_SNIFFER, /**< WIFI工作在Sniffer模式 */
    WWM_STATION, /**< WIFI工作在STATION模式 */
    WWM_SOFTAP, /**< WIFI工作在AP模式 */
    WWM_STATIONAP, /**< WIFI工作在双频模式 */
}WF_WK_MD_E;

/**
 * \brief WIFI工作在AP模式下的加密方式
 * \enum WF_AP_AUTH_MODE_E
 */
typedef enum {
    WAAM_OPEN = 0,
    WAAM_WEP,
    WAAM_WPA_PSK,
    WAAM_WPA2_PSK,
    WAAM_WPA_WPA2_PSK,
}WF_AP_AUTH_MODE_E;

/**
 * \brief WIFI配置为AP模式
 * \struct WF_AP_CFG_IF_S
 */
typedef struct {
    BYTE ssid[WIFI_SSID_LEN+1]; /**< SSID */
    BYTE s_len;  /**< SSID的长度 */
    BYTE passwd[WIFI_PASSWD_LEN+1]; /**< PASSWD */
    BYTE p_len; /**< PASSWD的长度 */
    BYTE chan; /**< AP信道，目前设置为0 */
    WF_AP_AUTH_MODE_E md; /**< 加密方式 */
    BYTE ssid_hidden; /**< 是否隐藏SSID，目前设置为0 */
    BYTE max_conn; /**< AP最大连接数，目前设置为0 */
    USHORT ms_interval; /**< 广播间隔，目前设置为0 */
}WF_AP_CFG_IF_S;

/**
 * \brief WIFI工作在STATION模式下的工作状态
 * \enum WF_STATION_STAT_E
 */
typedef enum {
    WSS_IDLE = 0,
    WSS_CONNECTING,
    WSS_PASSWD_WRONG,
    WSS_NO_AP_FOUND,
    WSS_CONN_FAIL,
    WSS_CONN_SUCCESS,
    WSS_GOT_IP,
}WF_STATION_STAT_E;

/**
 * \typedef typedef OPERATE_RET (*WF_ALL_AP_SCAN_CB)(OUT AP_IF_S **ap_ary,OUT UINT *num)
 * \brief WIFI搜索当前AP列表 回调函数定义
 * \param[in] *ap_ary 搜索到的AP数组
 * \param[in] *num 搜索到的AP数量
 * \return OPERATE_RET
 */
typedef OPERATE_RET (*WF_ALL_AP_SCAN_CB)(OUT AP_IF_S **ap_ary,OUT UINT *num);

/**
 * \typedef typedef OPERATE_RET (*WF_ASSIGN_AP_SCAN_CB)(IN CONST BYTE *ssid,OUT AP_IF_S **ap_ary)
 * \brief 指定BSSID下，WIFI搜索当前AP列表 回调函数定义
 * \param[in] ssid 指定的BSSID
 * \param[out] *ap_ary 搜索到的BSSID匹配的AP数组
 * \param[out] *num 搜索到的AP数量
 * \return OPERATE_RET
 */
typedef OPERATE_RET (*WF_ASSIGN_AP_SCAN_CB)(IN CONST BYTE *ssid,OUT AP_IF_S **ap_ary);

/**
 * \typedef typedef OPERATE_RET (*WF_RELEASE_AP_CB)(IN AP_IF_S **ap_ary)
 * \brief 回收函数WF_ALL_AP_SCAN_CB和函数WF_ASSIGN_AP_SCAN_CB返回的AP数组内存 的回调函数定义
 * \param[in] ap_ary 要释放的内存
 * \return OPERATE_RET
 */
typedef OPERATE_RET (*WF_RELEASE_AP_CB)(IN AP_IF_S *ap_ary);

/**
 * \typedef typedef OPERATE_RET (*WF_SET_CUR_CHAN_CB)(IN CONST BYTE chan)
 * \brief 设置WIFI当前工作信道 回调函数定义
 * \param[in] chan 要设置的工作信道
 * \return OPERATE_RET
 */
typedef OPERATE_RET (*WF_SET_CUR_CHAN_CB)(IN CONST BYTE chan);

/**
 * \typedef typedef OPERATE_RET (*WF_GET_CUR_CHAN_CB)(OUT BYTE *chan)
 * \brief 返回WIFI当前工作信道 回调函数定义
 * \param[out] chan 返回的工作信道
 * \return OPERATE_RET
 */
typedef OPERATE_RET (*WF_GET_CUR_CHAN_CB)(OUT BYTE *chan);

/**
 * \typedef typedef OPERATE_RET (*WF_SNIFFER_SET_CB)(IN CONST BOOL en,IN CONST SNIFFER_CALLBACK cb)
 * \brief 使能/去使能WIFI的空口抓包功能 回调函数定义
 * \param[in] en 使能/去使能
 * \param[in] cb 当使能空口抓包时，把抓到的包送到cb中
 * \return OPERATE_RET
 */
typedef OPERATE_RET (*WF_SNIFFER_SET_CB)(IN CONST BOOL en,IN CONST SNIFFER_CALLBACK cb);

/**
 * \typedef typedef OPERATE_RET (*WF_GET_IP_CB)(IN CONST WF_IF_E wf,OUT NW_IP_S *ip)
 * \brief 获取WIFI的IP地址 回调函数定义
 * \param[in] wf WIFI模式
 * \param[out] ip 获取到的WIFI的IP地址
 * \return OPERATE_RET
 */
typedef OPERATE_RET (*WF_GET_IP_CB)(IN CONST WF_IF_E wf,OUT NW_IP_S *ip);

/**
 * \typedef typedef OPERATE_RET (*WF_SET_IP_CB)(IN CONST WF_IF_E wf,IN CONST NW_IP_S *ip)
 * \brief 设置WIFI的IP地址 回调函数定义
 * \param[in] wf WIFI模式
 * \param[in] ip WIFI要设置的IP地址
 * \return OPERATE_RET
 */
typedef OPERATE_RET (*WF_SET_IP_CB)(IN CONST WF_IF_E wf,IN CONST NW_IP_S *ip);

/**
 * \typedef typedef OPERATE_RET (*WF_GET_MAC_CB)(IN CONST WF_IF_E wf,OUT NW_MAC_S *mac)
 * \brief 获取WIFI的MAC地址 回调函数定义
 * \param[in] wf WIFI模式
 * \param[out] mac 获取到的WIFI的MAC地址
 * \return OPERATE_RET
 */
typedef OPERATE_RET (*WF_GET_MAC_CB)(IN CONST WF_IF_E wf,OUT NW_MAC_S *mac);

/**
 * \typedef typedef OPERATE_RET (*WF_SET_MAC_CB)(IN CONST WF_IF_E wf,IN CONST NW_MAC_S *mac)
 * \brief 设置WIFI的MAC地址 回调函数定义
 * \param[in] wf WIFI模式
 * \param[in] mac WIFI要设置的MAC地址
 * \return OPERATE_RET
 */
typedef OPERATE_RET (*WF_SET_MAC_CB)(IN CONST WF_IF_E wf,IN CONST NW_MAC_S *mac);

/**
 * \typedef typedef OPERATE_RET (*WF_WK_MODE_SET_CB)(IN CONST WF_WK_MD_E mode)
 * \brief 设置WIFI的工作模式 回调函数定义
 * \param[in] mode WIFI工作模式
 * \return OPERATE_RET
 */
typedef OPERATE_RET (*WF_WK_MODE_SET_CB)(IN CONST WF_WK_MD_E mode);

/**
 * \typedef typedef OPERATE_RET (*WF_WK_MODE_GET_CB)(OUT WF_WK_MD_E *mode)
 * \brief 获取WIFI的工作模式 回调函数定义
 * \param[out] mode WIFI工作模式
 * \return OPERATE_RET
 */
typedef OPERATE_RET (*WF_WK_MODE_GET_CB)(OUT WF_WK_MD_E *mode);

/**
 * \typedef WF_STA_CONN_CB
 * \brief WIFI作为STA模式连接AP 回调函数定义
 * \param[in] ssid 要连接的AP的SSID
 * \param[in] passwd 要连接的AP的PASSWD
 * \return OPERATE_RET
 */
typedef OPERATE_RET (*WF_STA_CONN_CB)(IN CONST CHAR *ssid,IN CONST CHAR *passwd);

/**
 * \typedef typedef OPERATE_RET (*WF_STA_DISCONN_CB)(VOID)
 * \brief WIFI作为STA模式解除与AP的连接 回调函数定义
 * \return OPERATE_RET
 */
typedef OPERATE_RET (*WF_STA_DISCONN_CB)(VOID);

/**
 * \typedef typedef OPERATE_RET (*WF_STA_GET_CONN_AP_RSSI_CB)(OUT SCHAR *rssi)
 * \brief WIFI作为STA模式连接到AP上后，获取AP的信号强度 回调函数定义
 * \param[out] *rssi 信号强度
 * \return OPERATE_RET
 */
typedef OPERATE_RET (*WF_STA_GET_CONN_AP_RSSI_CB)(OUT SCHAR *rssi);

/**
 * \typedef typedef OPERATE_RET (*WF_STA_STAT_GET_CB)(OUT WF_STATION_STAT_E *stat)
 * \brief WIFI作为STA模式连接AP时，获取当前连接状态 回调函数定义
 * \param[out] *stat 当前连接状态
 * \return OPERATE_RET
 */
typedef OPERATE_RET (*WF_STA_STAT_GET_CB)(OUT WF_STATION_STAT_E *stat);

/**
 * \typedef typedef OPERATE_RET (*WF_AP_START_CB)(IN CONST WF_AP_CFG_IF_S *cfg)
 * \brief WIFI开启AP模式 回调函数定义
 * \param[in] cfg AP模式配置
 * \return OPERATE_RET
 */
typedef OPERATE_RET (*WF_AP_START_CB)(IN CONST WF_AP_CFG_IF_S *cfg);

/**
 * \typedef typedef OPERATE_RET (*WF_AP_STOP_CB)(VOID)
 * \brief WIFI关闭AP模式 回调函数定义
 * \return OPERATE_RET
 */
typedef OPERATE_RET (*WF_AP_STOP_CB)(VOID);

/**
 * \brief WIFI配置回调函数结构体
 * \struct WF_INTF_REG_S
 */
typedef struct {
    WF_ALL_AP_SCAN_CB wall_aps_cb; /**< WIFI搜索当前AP列表回调 */
    WF_ASSIGN_AP_SCAN_CB wass_aps_cb; /**< 指定BSSID下，WIFI搜索当前AP列表 */
    WF_RELEASE_AP_CB wrelease_ap_cb; /**< 回收函数WF_ALL_AP_SCAN_CB和函数WF_ASSIGN_AP_SCAN_CB返回的AP数组内存 */
    WF_SET_CUR_CHAN_CB wscc_cb; /**< 设置WIFI当前工作信道 */
    WF_GET_CUR_CHAN_CB wgcc_cb; /**< 返回WIFI当前工作信道 */
    WF_SNIFFER_SET_CB wss_cb; /**< 使能/去使能WIFI的空口抓包功能 */
    WF_GET_IP_CB wgi_cb; /**< 获取WIFI的IP地址 */
    WF_SET_IP_CB wsi_cb; /**< 设置WIFI的IP地址 */
    WF_GET_MAC_CB wgm_cb; /**< 获取WIFI的MAC地址 */
    WF_SET_MAC_CB wsm_cb; /**< 设置WIFI的MAC地址 */
    WF_WK_MODE_SET_CB wwms_cb; /**< 设置WIFI的工作模式 */
    WF_WK_MODE_GET_CB wwmg_cb; /**< 获取WIFI的工作模式 */
    WF_STA_CONN_CB wsc_cb; /**< WIFI作为STA模式连接AP */
    WF_STA_DISCONN_CB wsd_cb; /**< WIFI作为STA模式解除与AP的连接 */
    WF_STA_GET_CONN_AP_RSSI_CB wsgcar_cb; /**< WIFI作为STA模式连接到AP上后，获取AP的信号强度 */
    WF_STA_STAT_GET_CB wssg_cb; /**< WIFI作为STA模式连接AP时，获取当前连接状态 */
    WF_AP_START_CB wa_start_cb; /**< WIFI开启AP模式 */
    WF_AP_STOP_CB wa_stop_cb; /**< WIFI关闭AP模式 */
}WF_INTF_REG_S;

/**
 * \brief dev获取wifissid和passwd的方式
 * \enum WIFI_INIT_MODE_E
 */
typedef enum {
    WIFI_INIT_AUTO,
    WIFI_INIT_AP,
    WIFI_INIT_DEBUG,
}WIFI_INIT_MODE_E;

/**
 * \brief WIFI以自动方式启动所需信息结构体
 * \struct TUYA_WIFI_INIT_AUTO_S
 */
typedef struct
{
    VOID *p_reserved;
}TUYA_WIFI_INIT_AUTO_S;

/**
 * \brief WIFI以自建AP方式启动所需信息结构体
 * \struct TUYA_WIFI_INIT_AP_S
 */
typedef struct
{
    CHAR ap_ssid[WIFI_SSID_LEN+1]; /**< 指定涂鸦SDK使用的AP_SSID，而不是使用默认的AP_SSID，设置为全0 */
    CHAR ap_passwd[WIFI_PASSWD_LEN+1]; /**< 指定涂鸦SDK使用的AP_PASSWD，而不是使用默认的AP_PASSWD，设置为全0 */

    VOID *p_reserved;
}TUYA_WIFI_INIT_AP_S;

/**
 * \brief WIFI以调试模式启动所需信息结构体
 * \struct TUYA_WIFI_INIT_DEBUG_S
 */
typedef struct
{
    CHAR ssid[WIFI_SSID_LEN+1]; /**< 指定涂鸦SDK使用的AP_SSID，而不是使用默认的AP_SSID，设置为全0 */
    CHAR passwd[WIFI_PASSWD_LEN+1]; /**< 指定涂鸦SDK使用的AP_PASSWD，而不是使用默认的AP_PASSWD，设置为全0 */
    CHAR token[WIFI_TOKEN_LEN+1]; /**< 空无效 */

    VOID *p_reserved;
}TUYA_WIFI_INIT_DEBUG_S;

/**
 * \brief 启动所需信息结构体联合
 * \union TUYA_WIFI_INIT_U
 */
typedef union
{
    TUYA_WIFI_INIT_AUTO_S auto_cfg;
    TUYA_WIFI_INIT_AP_S ap_cfg;
    TUYA_WIFI_INIT_DEBUG_S debug_cfg;
}TUYA_WIFI_INIT_U;

/**
 * \brief 以WIFI_DEVICE方式启动SDK所需信息结构体
 * \struct TUYA_WIFI_DEV_INFO_S
 */
typedef struct
{
    WF_INTF_REG_S wf_infos; /**< 涂鸦SDK所需要的WIFI控制回调函数结构体 */

    WIFI_INIT_MODE_E wifi_init_mode;
    TUYA_WIFI_INIT_U wifi_init_cfg;

    VOID *p_reserved;
}TUYA_WIFI_DEV_INFO_S;

/**
 * \fn OPERATE_RET tuya_iot_start_wifi_sdk(IN CONST TUYA_WIFI_DEV_INFO_S *p_wifi_info)
 * \brief 以WIFI_DEVICE方式启动涂鸦SDK
 * \param[in] p_wifi_info 方式配置信息结构体
 * \return OPERATE_RET
 */
OPERATE_RET tuya_iot_start_wifi_sdk(IN CONST TUYA_WIFI_DEV_INFO_S *p_wifi_info);

/**
 * \fn OPERATE_RET tuya_iot_stop_wifi_sdk(VOID)
 * \brief 以WIFI_DEVICE方式停止涂鸦SDK
 * \return OPERATE_RET
 */
OPERATE_RET tuya_iot_stop_wifi_sdk(VOID);


#ifdef __cplusplus
}
#endif

#endif // TUYA_CLOUD_WIFI_FUNC_H
