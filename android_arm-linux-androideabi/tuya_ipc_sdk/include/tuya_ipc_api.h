/*********************************************************************************
  *Copyright(C),2017, 涂鸦科技 www.tuya.comm
  *FileName:    tuya_ipc_api.h
**********************************************************************************/

#ifndef _TUYA_IPC_API_H
#define _TUYA_IPC_API_H

#ifdef __cplusplus
extern "C" {
#endif

#include "tuya_cloud_types.h"
#include "tuya_cloud_wifi_defs.h"
#include "tuya_cloud_dev_defs.h"
#include "tuya_cloud_error_code.h"


typedef VOID (*TUYA_CMD_DISPATCH)(IN INT cmd,IN VOID *param);
typedef VOID (*TUYA_QUERY_DISPATCH)(IN INT cmd,IN VOID *param);

#define IPC_STORAGE_PATH_LEN 64
#define IPC_PRODUCT_KEY_LEN 16
#define IPC_UUID_LEN 25
#define IPC_AUTH_KEY_LEN 32
#define IPC_SW_VER_LEN 10
#define IPC_SERIAL_NUM_LEN 32
#define	IPC_FACTORY_ID_LEN 4
#define IPC_P2P_ID_LEN 25

typedef enum 
{
    E_IPC_UNREGISTERED = 0, //未注册激活状态
    E_IPC_REGISTERED = 1,   //中间态
    E_IPC_ACTIVEATED = 2    
//已经激活绑定状态，以此为准
}IPC_REGISTER_STATUS;

/**
 * \brief IPC环境变量结构体
 * \struct TUYA_IPC_ENV_VAR_S
 */
typedef struct
{
    CHAR storage_path[IPC_STORAGE_PATH_LEN + 1];/**< 涂鸦SDK内部配置文件存放路径，要求可读写，掉电不丢失 */

    CHAR product_key[IPC_PRODUCT_KEY_LEN + 1]; /**< product key */
    CHAR uuid[IPC_UUID_LEN + 1]; /**< 指定涂鸦SDK使用的UUID,如果为空，那么使用配置文件中预先写入的值 */
    CHAR auth_key[IPC_AUTH_KEY_LEN + 1]; /**< 指定涂鸦SDK使用的auth_key,如果为空，那么使用配置文件中预先写入的值 */
    CHAR p2p_id[IPC_P2P_ID_LEN + 1]; /**< 指定涂鸦SDK使用的p2p id ,如果用户本地没有，传入空值即可*/

    CHAR dev_sw_version[IPC_SW_VER_LEN + 1]; /**< 设备固件版本号 */
    CHAR dev_serial_num[IPC_SERIAL_NUM_LEN + 1]; /**< 设备唯一硬件标示号码 */

    TUYA_CMD_DISPATCH cmd_dispatch; /**< 涂鸦命令分发函数 */
    TUYA_QUERY_DISPATCH query_dispatch; /**< 涂鸦查询分发函数 */

    GW_STATUS_CHANGED_CB status_changed_cb; /**< 设备状态变更回调函数，可以为NULL */
    GW_UG_INFORM_CB gw_ug_cb; /**< IPC模式下获取固件升级信息回调函数，可以为NULL */
    GW_RST_INFORM_CB gw_rst_cb;/**< IPC模式下重置IPC回调函数，可以为NULL */
    GW_RESTART_INFORM_CB gw_restart_cb;/**< IPC模式下重启IPC进程回调函数，可以为NULL */

    BOOL mem_save_mode;/**< 低内存模式 */

    VOID *p_reserved;
}TUYA_IPC_ENV_VAR_S;

#define NOTIFICATION_MOTION_DETECTION 115

/**
 * \brief 通知的内容主体上传类型
 * \enum NOTIFICATION_CONTENT_TYPE_E
 */
typedef enum{
	NOTIFICATION_CONTENT_JPEG, /* 图片，.jpeg文件 */
	NOTIFICATION_CONTENT_PNG, /* 图片，.png文件 */
	NOTIFICATION_CONTENT_MAX,
} NOTIFICATION_CONTENT_TYPE_E;


/**
 * \brief 直接联网的输入类型，如二维码配网
 * \enum TUYA_IPC_DIRECT_CONNECT_TYPE_E
 */
typedef enum{
	TUYA_IPC_DIRECT_CONNECT_QRCODE = 0, 
	TUYA_IPC_DIRECT_CONNECT_MAX,
} TUYA_IPC_DIRECT_CONNECT_TYPE_E;



/**
 * \fn CHAR *tuya_ipc_get_sdk_info(VOID)
 * \brief 返回涂鸦SDK的编译信息
 * \return CHAR *
 */
CHAR *tuya_ipc_get_sdk_info(VOID);

/**
 * \fn OPERATE_RET tuya_ipc_init_sdk(IN CONST TUYA_IPC_ENV_VAR_S *p_var)
 * \brief 校验并且初始化涂鸦SDK正常工作所需的运行环境
 * \param[in] p_var 运行环境配置结构体指针
 * \return OPERATE_RET
 */
OPERATE_RET tuya_ipc_init_sdk(IN CONST TUYA_IPC_ENV_VAR_S *p_var);

/**
 * \fn OPERATE_RET tuya_ipc_start_sdk(IN CONST TUYA_WIFI_DEV_INFO_S *p_wifi_info)
 * \brief 以WIFI_DEVICE方式启动涂鸦SDK
 * \param[in] p_wifi_info WIFI_DEVICE方式配置信息结构体指针
 * \return OPERATE_RET
 */
OPERATE_RET tuya_ipc_start_sdk(IN CONST TUYA_WIFI_DEV_INFO_S *p_wifi_info);

/**
 * \fn OPERATE_RET tuya_ipc_upgrade_sdk(     IN CONST FW_UG_S *fw,\
 *                                     IN CONST GET_FILE_DATA_CB get_file_cb,\
 *                                     IN CONST UPGRADE_NOTIFY_CB upgrd_nofity_cb,\
 *                                     IN PVOID pri_data)
 * \brief 升级本地固件
 * \param[in] fw 固件文件URL
 * \param[in] get_file_cb 固件文件下载回调函数
 * \param[in] upgrd_nofity_cb 固件下载结束通知回调函数
 * \param[in] pri_data 当回调函数进行回调时，可作为额外参数
 * \return OPERATE_RET
 */
OPERATE_RET tuya_ipc_upgrade_sdk(     IN CONST FW_UG_S *fw,\
                                      IN CONST GET_FILE_DATA_CB get_file_cb,\
                                      IN CONST UPGRADE_NOTIFY_CB upgrd_nofity_cb,\
                                      IN PVOID pri_data);

/**
 * \fn OPERATE_RET tuya_ipc_reset(VOID)
 * \brief 重置IPC注册信息
 * \return OPERATE_RET
 */
OPERATE_RET tuya_ipc_reset(VOID);

/**
 * \fn OPERATE_RET tuya_ipc_get_register_status(VOID)
 * \brief 获取IPC注册激活的状态
 * \return OPERATE_RET
 */
IPC_REGISTER_STATUS tuya_ipc_get_register_status(VOID);

/**
 * \fn OPERATE_RET tuya_ipc_get_service_time(OUT time_t *time_utc, OUT INT *time_zone)
 * \brief 获取与APP相同的服务器时间
 * \param[out] time_utc utc时间
 * \param[out] time_zone 时区，以秒数返回
 * \return OPERATE_RET
 */
OPERATE_RET tuya_ipc_get_service_time(OUT TIME_T *time_utc, OUT INT *time_zone);

/**
 * \fn OPERATE_RET tuya_ipc_check_in_dls(IN TIME_T time_utc, OUT BOOL * pIsDls)
 * \brief 校验是否处于夏令时
 * \param[in] time_utc utc时间
 * \param[out] pIsDls 是否处于夏令时 TRUE:处于夏令时 FALSE:不处于夏令时
 * \return OPERATE_RET
 */
OPERATE_RET tuya_ipc_check_in_dls(IN TIME_T time_utc, OUT BOOL * pIsDls);


/**
 * \fn OPERATE_RET tuya_ipc_get_utc_time(OUT time_t *time_utc)
 * \brief 获取utc时间
 * \param[out] time_utc utc时间，以秒数返回
 * \return OPERATE_RET
 */
OPERATE_RET tuya_ipc_get_utc_time(OUT TIME_T *time_utc);

/**
 * \fn OPERATE_RET tuya_ipc_dp_report(IN CONST CHAR *p_json_data, IN CONST BOOL force)
 * \brief 涂鸦消息上报
 * \param[in] p_json_data 消息内容指针
 * \param[in] force 是否强制发送
 * \return OPERATE_RET
 */
OPERATE_RET tuya_ipc_dp_report(IN CONST CHAR *p_json_data, IN CONST BOOL force);

/**
 * \fn OPERATE_RET tuya_ipc_dp_report_sync(IN CONST CHAR *p_json_data, IN CONST BOOL force, IN CONST UINT timeout)
 * \brief 涂鸦消息上报
 * \param[in] p_json_data 消息内容指针
 * \param[in] force 是否强制发送
 * \param[in] timeout 超时时间(s)
 * \return OPERATE_RET
 */
OPERATE_RET tuya_ipc_dp_report_sync(IN CONST CHAR *p_json_data, IN CONST BOOL force, IN CONST UINT timeout);

/**
 * \fn INT tuya_ipc_notification_message_malloc(IN INT count, OUT VOID **message)
 * \brief 分配通知信息的消息
 * \param[in] count 消息所需要包含的视频/图片数量
 * \param[in] message 消息指针
 * \return INT MALLOC的空间大小
 */
INT tuya_ipc_notification_message_malloc(IN INT count, OUT VOID **message);

/**
 * \fn OPERATE_RET tuya_ipc_notification_message_free(IN VOID *message)
 * \brief 释放通知信息的消息
 * \param[in] message 消息指针
 * \return OPERATE_RET
 */
OPERATE_RET tuya_ipc_notification_message_free(IN VOID *message);

/**
 * \fn OPERATE_RET tuya_ipc_notification_content_upload_from_buffer(IN CHAR *name, IN NOTIFICATION_CONTENT_TYPE_E type,
 *          IN CHAR *data, IN INT data_len, OUT VOID *message)
 * \brief 通知信息的内容主体上传至涂鸦云
 * \param[in] type 图片/视频类型
 * \param[in] data 数据指针
 * \param[in] data_len 数据长度
 * \param[out] message 通知信息的消息存储指针
 * \return INT OPERATE_RET
 * \warning 阻塞型函数，建议线程内使用，多线程安全
 */
OPERATE_RET tuya_ipc_notification_content_upload_from_buffer(IN NOTIFICATION_CONTENT_TYPE_E type,
                                        IN CHAR *data, IN INT data_len, OUT VOID *message);

/**
 * \fn OPERATE_RET tuya_ipc_notification_content_upload_from_file(IN CHAR *name, IN NOTIFICATION_CONTENT_TYPE_E type,
                                        OUT VOID *message)
 * \brief 通知信息的内容主体上传至涂鸦云
 * \param[in] name 图片/视频路径及名称
 * \param[in] type 图片/视频类型
 * \param[out] message 通知信息的消息存储指针
 * \return INT OPERATE_RET
 * \warning 阻塞型函数，建议线程内使用，多线程安全
 */
OPERATE_RET tuya_ipc_notification_content_upload_from_file(IN CHAR *name, IN NOTIFICATION_CONTENT_TYPE_E type,
                                        OUT VOID *message);

/**
 * \fn OPERATE_RET tuya_ipc_notification_message_upload(IN CONST BYTE notification, IN VOID *message, IN UINT time_out)
 * \brief 根据推送的类型，以及消息内容，会自动生成消息推送给APP
 * \param[in] notification 推送类型
 * \param[in] message 消息指针
 * \param[in] time_out 超时时间
 * \return OPERATE_RET
 */
OPERATE_RET tuya_ipc_notification_message_upload(IN CONST BYTE notification, IN VOID *message, IN UINT time_out);

/**
 * \fn OPERATE_RET tuya_ipc_snapshot_message_upload(IN CONST BYTE snapshot, IN UINT time_out)
 * \brief 门铃按下时推送截图信息给APP
 * \param[in] snapshot 推送类型
 * \param[in] message 消息指针
 * \param[in] time_out 超时时间
 * \return OPERATE_RET
 */
OPERATE_RET tuya_ipc_snapshot_message_upload(IN CONST BYTE snapshot, IN VOID *message, IN UINT time_out);

/**
 * \fn OPERATE_RET tuya_ipc_get_wakeup_data(INOUT BYTE *wakeup_data_arr, INOUT UINT *p_len)
 * \param[in out] wakeup_data_arr 推送给设备的休眠唤醒的数据
 * \param[in out] p_len 推送给设备的休眠唤醒的数据长度
 * \return OPERATE_RET
 */
OPERATE_RET tuya_ipc_get_wakeup_data(INOUT BYTE *wakeup_data_arr, INOUT UINT *p_len);

/**
 * \fn OPERATE_RET tuya_ipc_get_heartbeat_data(INOUT BYTE *heartbeat_data_arr, INOUT UINT *p_len)
 * \param[in out] heartbeat_data_arr 休眠时设备心跳数据
 * \param[in out] p_len 休眠时设备心跳数据长度
 * \return OPERATE_RET
 */
OPERATE_RET tuya_ipc_get_heartbeat_data(INOUT BYTE *heartbeat_data_arr, INOUT UINT *p_len);

/**
 * \fn OPERATE_RET tuya_ipc_direct_connect(IN CONST CHAR *p_str, IN CONST TUYA_IPC_DIRECT_CONNECT_TYPE_E source) 
 * \brief 提供给客户直接联网操作，比如在二维码，声波配网等
 * \param[in] p_str  {"p":"20112012pw1","s":"GAOKE_57093C","t":"AYkbx3mD298GJi"} p表示密码，s表示ssid，t表示token
 * \param[in] source 目前仅支持二维码 TUYA_IPC_DIRECT_CONNECT_QRCODE
 * \return OPERATE_RET
 */
OPERATE_RET tuya_ipc_direct_connect(IN CONST CHAR *p_str, IN CONST TUYA_IPC_DIRECT_CONNECT_TYPE_E source);

/**
 * \fn OPERATE_RET tuya_ipc_set_net_info_nofify_cb(IN CONST GW_GET_SSID_PASSWD_TOKEN_CB cb)
 * \brief 设置通知客户获取配网信息的回调函数
 * \return OPERATE_RET
 */
OPERATE_RET tuya_ipc_set_net_info_nofify_cb(IN CONST GW_GET_SSID_PASSWD_TOKEN_CB nofity_cb);

/**
 * \fn OPERATE_RET tuya_ipc_door_bell_press(VOID)
 * \brief 
 * \return OPERATE_RET
 */
OPERATE_RET tuya_ipc_door_bell_press(VOID);

/**
 * \fn OPERATE_RET tuya_ipc_book_wakeup_topic(VOID)
 * \brief 执行订阅唤醒事件
 * \return OPERATE_RET
 */
OPERATE_RET tuya_ipc_book_wakeup_topic(VOID);

/**
 * \fn INT tuya_ipc_get_mqtt_socket_fd(VOID)
 * \brief 获取mqqt的socket fd
 * \return INT
 */
INT tuya_ipc_get_mqtt_socket_fd(VOID);

/**
 * \fn OPERATE_RET tuya_ipc_set_log_attr(IN CONST INT log_level, CHAR *filename)
 * \brief 设置日志打印级别
 * \param[in] log_level 0-5,数值越大，日志打印越丰富。
 * \param[in] filename 是否打印到文件
 * \return OPERATE_RET
 */
OPERATE_RET tuya_ipc_set_log_attr(IN CONST INT log_level, CHAR *filename);

/**
 * \fn OPERATE_RET tuya_ipc_send_custom_mqtt_msg(IN CONST UINT protocol, IN CONST CHAR *p_data)
 * \brief 发送自定义mqtt消息
 * \return OPERATE_RET
 */
OPERATE_RET tuya_ipc_send_custom_mqtt_msg(IN CONST UINT protocol, IN CONST CHAR *p_data);

/**
 * \fn OPERATE_RET tuya_ipc_set_service_time(IN TIME_T new_time_utc)
 * \brief 强制设置SDK的时间
 * \param[in] new_time_utc utc时间
 * \return OPERATE_RET
 */
OPERATE_RET tuya_ipc_set_service_time(IN TIME_T new_time_utc);


/**
 * \fn OPERATE_RET tuya_ipc_report_living_msg(uint32_t error_code, int force, int timeout)
 * \brief 向app端通知直播信息
 * \param[in] error_code 错误码
 * \param[in] force
 * \param[in] timeout  超时时间
 * \return OPERATE_RET
 */
OPERATE_RET tuya_ipc_report_living_msg(IN UINT error_code, IN UINT force, IN UINT timeout);

/**
 * \fn OPERATE_RET tuya_ipc_get_free_ram(IN ULONG *free)
 * \brief 查询系统剩余内存
 * \param[in] 内存容量结果，单位KB
 * \return OPERATE_RET
 */
OPERATE_RET tuya_ipc_get_free_ram(IN ULONG *free);

#ifdef __cplusplus
}
#endif

#endif  /*_TUYA_IPC_API_H*/

