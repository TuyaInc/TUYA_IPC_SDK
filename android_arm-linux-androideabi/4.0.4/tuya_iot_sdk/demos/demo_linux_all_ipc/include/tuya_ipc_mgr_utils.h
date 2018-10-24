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

typedef enum {
    WIFI_INIT_AUTO,
    WIFI_INIT_AP,
    WIFI_INIT_DEBUG,
}WIFI_INIT_MODE_E;


typedef struct
{
    CHAR_T storage_path[IPC_STORAGE_PATH_LEN + 1];/**< 涂鸦SDK内部配置文件存放路径，要求可读写，掉电不丢失 */
    CHAR_T upgrade_file_path[IPC_STORAGE_PATH_LEN + 1];/**< OTA文件保存位置 */
    CHAR_T sd_base_path[IPC_STORAGE_PATH_LEN + 1];/**< SD卡目录位置 */

    CHAR_T product_key[IPC_PRODUCT_KEY_LEN + 1]; /**< product key */
    CHAR_T uuid[IPC_UUID_LEN + 1]; /**< 指定涂鸦SDK使用的UUID,如果为空，那么使用配置文件中预先写入的值 */
    CHAR_T auth_key[IPC_AUTH_KEY_LEN + 1]; /**< 指定涂鸦SDK使用的auth_key,如果为空，那么使用配置文件中预先写入的值 */
    CHAR_T p2p_id[IPC_P2P_ID_LEN + 1]; /**< 指定涂鸦SDK使用的p2p id */
    CHAR_T dev_sw_version[IPC_SW_VER_LEN + 1]; /**< 设备固件版本号 */
    UINT_T max_p2p_user;/**< p2p最大数目 */
}IPC_MGR_INFO_S;

/* 更新本地时间 */
OPERATE_RET IPC_APP_Sync_Utc_Time(VOID);

/* 获取最大p2p连接数目 */
UINT_T IPC_APP_Get_Max_P2P_User(VOID);

/* 获取MQTT连接状态 */
INT_T IPC_APP_Get_Mqtt_Status(VOID);

/* SDK初始化 */
OPERATE_RET IPC_APP_Init_SDK(WIFI_INIT_MODE_E init_mode, CHAR_T *p_token);

/* 本地重置IPC */
OPERATE_RET tuya_ipc_reset(VOID);


/*
获取与APP相同的服务器时间
time_utc utc时间
time_zone 时区，以秒数返回
*/
OPERATE_RET tuya_ipc_get_service_time(OUT TIME_T *time_utc, OUT INT_T *time_zone);

/* 强制设置SDK的时间 */
OPERATE_RET tuya_ipc_set_service_time(IN TIME_T new_time_utc);



OPERATE_RET hwl_entropy_init(VOID);
OPERATE_RET hwl_entropy_calc(unsigned char *output, size_t output_len);
OPERATE_RET hwl_entropy_release(VOID);

#ifdef __cplusplus
}
#endif

#endif  /*_TUYA_IPC_MGR_UTILS_H*/
