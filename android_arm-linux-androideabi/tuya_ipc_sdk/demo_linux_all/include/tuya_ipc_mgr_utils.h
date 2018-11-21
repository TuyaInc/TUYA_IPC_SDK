/*********************************************************************************
  *Copyright(C),2015-2020, 涂鸦科技 www.tuya.comm
  *FileName:    tuya_ipc_mgr_utils.h
  *
  * 请勿随意修改该文件中的任何内容，如需修改请联系涂鸦产品经理！！
  * 文件描述：
  * 1. 固件工具API定义
**********************************************************************************/


#ifndef _TUYA_IPC_MGR_UTILS_H
#define _TUYA_IPC_MGR_UTILS_H

#ifdef __cplusplus
extern "C" {
#endif

#include "tuya_ipc_api.h"

typedef struct
{
    CHAR storage_path[IPC_STORAGE_PATH_LEN + 1];/**< 涂鸦SDK内部配置文件存放路径，要求可读写，掉电不丢失 */
    CHAR upgrade_file_path[IPC_STORAGE_PATH_LEN + 1];/**< OTA文件保存位置 */
    CHAR sd_base_path[IPC_STORAGE_PATH_LEN + 1];/**< SD卡目录位置 */

    CHAR product_key[IPC_PRODUCT_KEY_LEN + 1]; /**< product key */
    CHAR uuid[IPC_UUID_LEN + 1]; /**< 指定涂鸦SDK使用的UUID,如果为空，那么使用配置文件中预先写入的值 */
    CHAR auth_key[IPC_AUTH_KEY_LEN + 1]; /**< 指定涂鸦SDK使用的auth_key,如果为空，那么使用配置文件中预先写入的值 */
    CHAR p2p_id[IPC_P2P_ID_LEN + 1]; /**< 指定涂鸦SDK使用的p2p id */
    CHAR dev_sw_version[IPC_SW_VER_LEN + 1]; /**< 设备固件版本号 */
    UINT max_p2p_user;/**< p2p最大数目 */
}IPC_MGR_INFO_S;

/* 更新本地时间 */
OPERATE_RET IPC_APP_Sync_Utc_Time(VOID);

/* 获取本地存储SD卡路径 */
CHAR *IPC_APP_Get_SD_PATH(VOID);

/* 获取最大p2p连接数目 */
UINT IPC_APP_Get_Max_P2P_User(VOID);

/* 获取MQTT连接状态 */
INT IPC_APP_Get_Mqtt_Status(VOID);

/* SDK初始化 */
OPERATE_RET IPC_APP_Init_SDK(WIFI_INIT_MODE_E init_mode, CHAR *p_token);


#ifdef __cplusplus
}
#endif

#endif  /*_TUYA_IPC_MGR_UTILS_H*/
