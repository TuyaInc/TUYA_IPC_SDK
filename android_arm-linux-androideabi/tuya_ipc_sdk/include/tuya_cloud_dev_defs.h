/*********************************************************************************
  *Copyright(C),2017, 涂鸦科技 www.tuya.comm
  *FileName:    tuya_cloud_dev_defs.h
**********************************************************************************/

#ifndef TUYA_CLOUD_DEV_CTL_H
#define TUYA_CLOUD_DEV_CTL_H

#ifdef __cplusplus
extern "C" {
#endif

#include "cJSON.h"
#include "tuya_cloud_types.h"
#include "tuya_cloud_error_code.h"

/**
 * \brief 数据来源枚举
 * \enum DP_CMD_TYPE_E
 */
typedef enum
{
    DP_CMD_LAN = 0,/**< 数据来自局域网 */
    DP_CMD_MQ,/**< 数据来自涂鸦云 */
    DP_CMD_TIMER,/**< 数据来自本地定时任务 */
}DP_CMD_TYPE_E;

/**
 * \brief 网关状态枚举
 * \enum GW_STATUS_E
 */
typedef enum
{
    GW_UNREGISTERED = 0,   /**< 网关尚未注册 */
    GW_REGISTERED,         /**< 网关尚未激活 */
    GW_ACTIVED,            /**< 网关已经激活 */
    GW_MQTT_ONLINE,        /**< 网关连接上MQTT */
    GW_MQTT_OFFLINE,       /**< 网关从MQTT离线 */
}GW_STATUS_E;

/**
 * \brief OTA状态枚举
 * \enum UPGRADE_PROGRESS_E
 */
typedef enum
{
    UPGRADE_FINISH = 0,/**< OTA升级结束 */
    UPGRADE_ERROR,/**< OTA升级失败 */
}UPGRADE_PROGRESS_E;


#define FW_URL_LEN 255 /**< 固件文件URL长度 */
#define FW_MD5_LEN 32  /**< 固件文件md5值长度 */
#define SW_VER_LEN 10  /**< 固件版本号长度 */
/**
 * \brief OTA结构体定义
 * \struct FW_UG_S
 */
typedef struct {
    CHAR fw_url[FW_URL_LEN+1];  /**< 固件文件URL */
    CHAR fw_md5[FW_MD5_LEN+1];  /**< 固件文件md5值 */
    CHAR sw_ver[SW_VER_LEN+1];  /**< 固件版本号 */
    UINT file_size;             /**< 固件文件大小（单位字节） */
}FW_UG_S;


/**
 * \typedef typedef VOID (*GW_STATUS_CHANGED_CB)(IN CONST GW_STATUS_E status, IN CONST PVOID p_reserved)
 * \brief 网关状态变化通知回调函数定义
 * \param[in] status 最新状态
 * \param[in] p_reserved 目前为NULL
 */
typedef VOID (*GW_STATUS_CHANGED_CB)(IN CONST GW_STATUS_E status, IN CONST PVOID p_reserved);

/**
 * \typedef typedef VOID (*GW_DEV_CMD_CB)(IN DP_CMD_TYPE_E cmd_tp,IN CONST CHAR *dev_id,IN cJSON *dps)
 * \brief dp数据变更回调函数定义
 * \param[in] cmd_tp 数据下发通道
 * \param[in] dev_id deviceid
 * \param[in] dps JSON格式的结构化数据
 */
typedef VOID (*GW_DEV_CMD_CB)(IN DP_CMD_TYPE_E cmd_tp,IN CONST CHAR *dev_id,IN cJSON *dps);

/**
 * \typedef typedef VOID (*GW_DEV_QUERY_CB)(IN DP_CMD_TYPE_E cmd_tp,IN CONST CHAR *dev_id,IN cJSON *dps_arr);
 * \brief dp数据查询回调函数定义
 * \param[in] cmd_tp 数据查询通道
 * \param[in] dev_id deviceid
 * \param[in] dps_arr JSON格式的结构化数据
 */
typedef VOID (*GW_DEV_QUERY_CB)(IN DP_CMD_TYPE_E cmd_tp,IN CONST CHAR *dev_id,IN cJSON *dps_arr);

/**
 * \typedef typedef VOID (*GW_UG_INFORM_CB)(IN CONST FW_UG_S *fw)
 * \brief 网关模式下获取固件升级信息回调函数定义
 * \param[in] fw 固件升级信息结构体
 */
typedef VOID (*GW_UG_INFORM_CB)(IN CONST FW_UG_S *fw);


/**
 * \typedef typedef VOID (*DEV_UG_INFORM_CB)(IN CONST CHAR *devid,IN CONST FW_UG_S *fw)
 * \brief 设备模式下获取固件升级信息回调函数定义
 * \param[in] devid device_id
 * \param[in] fw 固件升级信息结构体
 * \deprecated IPC下该函数定义无用
 */
typedef VOID (*DEV_UG_INFORM_CB)(IN CONST CHAR *devid,IN CONST FW_UG_S *fw);

/**
 * \typedef typedef OPERATE_RET (*GET_FILE_DATA_CB)(IN CONST CHAR *fw_url, IN CONST UINT total_len,IN CONST UINT offset,\
 *                                      IN CONST BYTE *data,IN CONST UINT len,OUT UINT *remain_len, IN PVOID pri_data)
 * \brief 固件文件下载回调函数定义
 * \param[in] fw_url 固件文件URL
 * \param[in] total_len 固件文件大小（单位字节）
 * \param[in] offset 当前回调函数文件下载偏移量
 * \param[in] data 当前回调函数已经下载内容
 * \param[in] len data的长度
 * \param[in] *remain_len 通知涂鸦云当前回调函数本次处理内容长度 (*remain_len<=len)，本参数用于解决本地存储时可能出现的字节对齐问题。
 * \param[in] pri_data 注册回调函数时设置的额外参数
 * \return OPERATE_RET
 *               OPRT_OK:本次回调已经成功执行，可以继续下载
 *               other:本次回调执行失败
 */
typedef OPERATE_RET (*GET_FILE_DATA_CB)(IN CONST CHAR *fw_url, IN CONST UINT total_len,IN CONST UINT offset,\
                                        IN CONST BYTE *data,IN CONST UINT len,OUT UINT *remain_len, IN PVOID pri_data);

/**
 * \typedef typedef VOID (*UPGRADE_NOTIFY_CB)(IN CONST CHAR *fw_url, IN CONST UPGRADE_PROGRESS_E progress, IN PVOID pri_data)
 * \brief 固件下载结束通知回调函数定义
 * \param[in] fw_url:固件文件URL
 * \param[in] progress:固件下载状态
 * \param[in] UPGRADE_FINISH:固件下载结束
 * \param[in] UPGRADE_ERROR:固件下载失败
 * \param[in] pri_data:注册回调函数时设置的额外参数
 */
typedef VOID (*UPGRADE_NOTIFY_CB)(IN CONST CHAR *fw_url, IN CONST UPGRADE_PROGRESS_E progress, IN PVOID pri_data);

/**
 * \typedef typedef VOID (*GW_RST_INFORM_CB)(INT from)
 * \brief 网关模式下重置网关回调函数定义
 * \param[in] from: 0:本地重置 1:APP移除
 */
typedef VOID (*GW_RST_INFORM_CB)(INT from);

/**
 * \typedef typedef VOID (*GW_ECHO_SHOW_CB)(IN CONST CHAR* url)
 * \brief ECHO SHOW 服务回调回调函数定义
 * \param[in] url
 */
typedef VOID (*GW_ECHO_SHOW_CB)(IN CONST CHAR* url);

/**
 * \typedef  typedef VOID (*GW_CLOUD_STORAGE_CB)(IN CONST CHAR* action, IN CONST CHAR* store_mode);
  * \brief 云存储功能开启服务回调回调函数定义
 * \param[in] action: store/abort
 * \param[in] store_mode: event/continue
 */
typedef VOID (*GW_CLOUD_STORAGE_CB)(IN CONST CHAR* action, IN CONST CHAR* store_mode);

/**
 * \typedef typedef VOID (*GW_CHROMECAST_CB)(IN CONST CHAR* url)
 * \brief CHROMECAST 服务回调函数定义
 * \param[in] url
 */
typedef VOID (*GW_CHROMECAST_CB)(IN CHAR* url);


/**
 * \typedef  typedef VOID (*GW_GET_SSID_PASSWD_TOKEN_CB)(VOID);
  * \brief 获取到ap的ssid，password和token通知函数
 */
typedef VOID (*GW_GET_SSID_PASSWD_TOKEN_CB)(VOID);


/**
 * \typedef typedef VOID (*GW_RESTART_INFORM_CB)(VOID);
 * \brief 网关模式下重启网关回调函数定义
 */
typedef VOID (*GW_RESTART_INFORM_CB)(VOID);


#ifdef __cplusplus
}
#endif

#endif // TUYA_CLOUD_DEV_CTL_H
