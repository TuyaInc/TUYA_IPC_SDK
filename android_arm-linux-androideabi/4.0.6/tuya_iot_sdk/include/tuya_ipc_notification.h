/*********************************************************************************
  *Copyright(C),2017, 涂鸦科技 www.tuya.comm
  *FileName:    tuya_ipc_notification.h
**********************************************************************************/

#ifndef TUYA_IPC_NOTIFICATION_H
#define TUYA_IPC_NOTIFICATION_H

#include "tuya_cloud_types.h"
#include "tuya_cloud_error_code.h"

#define NOTIFICATION_MOTION_DETECTION 115

/**
 * \brief 通知的内容主体上传类型
 * \enum NOTIFICATION_CONTENT_TYPE_E
 */
typedef enum{
    NOTIFICATION_CONTENT_MP4 = 0, /* 视频，.mp4文件 */
    NOTIFICATION_CONTENT_JPEG, /* 图片，.jpeg文件 */
    NOTIFICATION_CONTENT_PNG, /* 图片，.png文件 */
    NOTIFICATION_CONTENT_MAX,
} NOTIFICATION_CONTENT_TYPE_E;

INT_T tuya_ipc_notification_message_malloc(IN INT_T count, OUT VOID **message);
INT_T tuya_ipc_notification_message_free(IN VOID *message);

/**
 * \fn OPERATE_RET tuya_ipc_notification_content_upload_from_buffer(IN CHAR_T *name, IN NOTIFICATION_CONTENT_TYPE_E type,
 *          IN CHAR_T *data, IN INT_T data_len, OUT VOID *message)
 * \brief 通知信息的内容主体上传至涂鸦云
 * \param[in] name 图片/视频名称
 * \param[in] type 图片/视频类型
 * \param[in] data 数据指针
 * \param[in] data_len 数据长度
 * \param[out] message 通知信息的消息存储指针
 * \return INT_T OPERATE_RET
 * \warning 阻塞型函数，建议线程内使用，多线程安全
 */
OPERATE_RET tuya_ipc_notification_content_upload_from_buffer(IN CHAR_T *name, IN NOTIFICATION_CONTENT_TYPE_E type,
                                        IN CHAR_T *data, IN INT_T data_len, OUT VOID *message);

/**
 * \fn OPERATE_RET tuya_ipc_notification_content_upload_from_file(IN CHAR_T *name, IN NOTIFICATION_CONTENT_TYPE_E type,
                                        OUT VOID *message)
 * \brief 通知信息的内容主体上传至涂鸦云
 * \param[in] name 图片/视频路径及名称
 * \param[in] type 图片/视频类型
 * \param[out] message 通知信息的消息存储指针
 * \return INT_T OPERATE_RET
 * \warning 阻塞型函数，建议线程内使用，多线程安全
 */
OPERATE_RET tuya_ipc_notification_content_upload_from_file(IN CHAR_T *name, IN NOTIFICATION_CONTENT_TYPE_E type,
                                        OUT VOID *message);

/**
 * \fn OPERATE_RET tuya_ipc_notification_message_upload(IN CONST BYTE_T notification, IN VOID *message, IN UINT_T time_out)
 * \brief 根据推送的类型，以及消息内容，会自动生成消息推送给APP
 * \param[in] notification 推送类型
 * \param[in] message 消息指针
 * \param[in] time_out 超时时间
 * \return OPERATE_RET
 */
OPERATE_RET tuya_ipc_notification_message_upload(IN CONST BYTE_T notification, IN VOID *message, IN UINT_T time_out);

/**
 * \fn OPERATE_RET tuya_ipc_snapshot_message_upload(IN CONST BYTE_T snapshot, IN UINT_T time_out)
 * \brief 门铃按下时推送截图信息给APP
 * \param[in] snapshot 推送类型
 * \param[in] message 消息指针
 * \param[in] time_out 超时时间
 * \return OPERATE_RET
 */
OPERATE_RET tuya_ipc_snapshot_message_upload(IN CONST BYTE_T snapshot, IN VOID *message, IN UINT_T time_out);





OPERATE_RET notification_content_upload_from_buffer(IN CHAR_T *name, IN CHAR_T* content_type,
                                    IN CHAR_T *data, IN INT_T data_len, OUT VOID *message);

OPERATE_RET notification_content_upload_from_file(IN CHAR_T *name, IN CHAR_T* content_type, OUT VOID *message);

OPERATE_RET notification_message_upload(IN CONST BYTE_T notification, IN VOID *message, IN UINT_T time_out);

OPERATE_RET notification_snapshot_message_upload(IN CONST BYTE_T snapshot, IN VOID *message, IN UINT_T time_out);

#endif//TUYA_IPC_NOTIFICATION_H

