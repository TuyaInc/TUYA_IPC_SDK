/*********************************************************************************
  *Copyright(C),2015-2020, 涂鸦科技 www.tuya.comm
  *FileName:    tuya_ipc_media_utils.h
  *
  * 请勿随意修改该文件中的任何内容，如需修改请联系涂鸦产品经理！！
  * 文件描述：
  * 1. 音视频工具API定义
**********************************************************************************/


#ifndef _TUYA_IPC_MEDIA_UTILS_H
#define _TUYA_IPC_MEDIA_UTILS_H

#include "tuya_ring_buffer.h"
#include "tuya_cloud_types.h"


#ifdef __cplusplus
extern "C" {
#endif

#define PR_ERR(fmt, ...)    printf("Err:"fmt"\r\n", ##__VA_ARGS__)
#define PR_DEBUG(fmt, ...)  printf("Dbg:"fmt"\r\n", ##__VA_ARGS__)
//#define PR_TRACE(fmt, ...)  printf("Trace:"fmt"\r\n", ##__VA_ARGS__)
#define PR_TRACE(fmt, ...)

/* 工具API */
VOID IPC_APP_Refresh_Media_Info(VOID);
/* 工具API */
OPERATE_RET TUYA_APP_Init_Ring_Buffer(VOID);
/* 工具API */
OPERATE_RET TUYA_APP_Init_Stream_Storage(IN CONST CHAR_T *p_sd_base_path);
/* 将采集到的音视频裸数据送入SDK */
OPERATE_RET TUYA_APP_Put_Frame(IN CONST CHANNEL_E channel, IN CONST MEDIA_FRAME_S *p_frame);
/* 工具API */
OPERATE_RET TUYA_APP_Get_Frame(IN CONST CHANNEL_E channel, IN CONST USER_INDEX_E user_index, IN CONST BOOL_T isRetry, IN CONST BOOL_T ifBlock, INOUT MEDIA_FRAME_S *p_frame);
/* 工具API */
OPERATE_RET TUYA_APP_Enable_P2PTransfer(IN UINT_T max_users);
/* 工具API */
OPERATE_RET TUYA_APP_Enable_EchoShow_Chromecast(VOID);

/* 事件模式录像下，发生事件时，通知SDK录制 */
OPERATE_RET TUYA_APP_Trigger_Event_Storage(VOID);

/* 工具API */
OPERATE_RET TUYA_APP_Enable_CloudStorage(VOID);

#ifdef __cplusplus
}
#endif

#endif  /* _TUYA_IPC_MEDIA_UTILS_H */
