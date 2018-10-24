/*********************************************************************************
  *Copyright(C),2017, 涂鸦科技 www.tuya.comm
  *FileName:    tuya_ipc_cloud_storage.h
**********************************************************************************/

#ifndef __TUYA_IPC_CLOUD_STORAGE_H__
#define __TUYA_IPC_CLOUD_STORAGE_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "tuya_cloud_types.h"
#include "tuya_cloud_error_code.h"
#include "tuya_ipc_media.h"

//上报事件时的快照大小上限
#define MAX_SNAPSHOT_BUFFER_SIZE            100*1024

/* 事件类型 */
typedef enum
{
    EVENT_TYPE_MOTION_DETECT,   /* 移动侦测触发的存储 */
    EVENT_TYPE_DOOR_BELL,       /* 门铃唤醒触发的存储 */
    EVENT_TYPE_INVALID
}ClOUD_STORAGE_EVENT_TYPE_E;

/* 云存储订单类型 */
typedef enum
{
    ClOUD_STORAGE_TYPE_CONTINUE,  /* 连续上传云存储数据，结束以订单结束时间为准 */
    ClOUD_STORAGE_TYPE_EVENT,     /* 事件区间上传云存储数据，开始、结束由自定义数据触发 */ 
    ClOUD_STORAGE_TYPE_INVALID    /* 无订单或其他异常状态 */
}ClOUD_STORAGE_TYPE_E;

typedef INT_T (*Tuya_CBC_AES128_Init)(VOID);
typedef INT_T (*Tuya_CBC_AES128_Encrypt)(IN BYTE_T *pdata_in,   //加密前数据，需要保证不会被修改
                                             IN UINT_T data_len,     //加密前数据大小
                                             IN BYTE_T *pdata_out,   //机密后数据目的地址，SDK内部申请了空间，不需要额外malloc
                                             OUT UINT_T *pdata_out_len,   //加密后数据大小
                                             IN BYTE_T *pkey,     //aes加密key
                                             IN BYTE_T *piv);     //aes cbc加密iv
typedef INT_T (*Tuya_CBC_AES128_Destroy)(VOID);

typedef struct
{
    Tuya_CBC_AES128_Init init;
    Tuya_CBC_AES128_Encrypt encrypt;
    Tuya_CBC_AES128_Destroy destory;
}AES_HW_CBC_FUNC;

/**
 * \fn OPERATE_RET tuya_ipc_cloud_storage_init  
 * \brief 云存储功能初始化
 * \param[in] media_setting 多媒体流配置信息。当为变码率或配置可能在运行中更改的情况，各个参数请传入上限值。
 * \return OPERATE_RET
 */
OPERATE_RET tuya_ipc_cloud_storage_init(IN IPC_MEDIA_INFO_S *media_setting, IN AES_HW_CBC_FUNC *aes_func);

/**
 * \fn OPERATE_RET tuya_ipc_cloud_storage_get_store_mode  
 * \brief 获取当前云存储订单类型。
 * \return ClOUD_STORAGE_TYPE_E
 */
ClOUD_STORAGE_TYPE_E tuya_ipc_cloud_storage_get_store_mode(VOID);

/**
 * \fn OPERATE_RET tuya_ipc_cloud_storage_event_start  
 * \brief 上报一个事件的开始
 * \param[in] snapshot_buffer snapshot_size 当前事件发生时的快照内容与大小。最大支持100K的图片。
 * \param[in] type 事件类型
 * \return OPERATE_RET
 */
OPERATE_RET tuya_ipc_cloud_storage_event_start(IN CHAR_T *snapshot_buffer, IN UINT_T snapshot_size, IN ClOUD_STORAGE_EVENT_TYPE_E type);

/**
 * \fn OPERATE_RET tuya_ipc_cloud_storage_event_start  
 * \brief 上报当前事件的结束。暂不支持多个事件起始-结束交叉上报。
 * \return OPERATE_RET
 */
OPERATE_RET tuya_ipc_cloud_storage_event_stop(VOID);

#ifdef __cplusplus
}
#endif

#endif
