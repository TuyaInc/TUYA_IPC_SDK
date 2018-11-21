/*********************************************************************************
  *Copyright(C),2015-2020, 涂鸦科技 www.tuya.comm
  *FileName: tuya_ipc_media_handler.h
  *
  * 文件描述：
  * 1. 音视频函数API定义，具体内容请参考.c文件中的代码注释
**********************************************************************************/

#ifndef _TUYA_IPC_MEDIA_HANDLER_H
#define _TUYA_IPC_MEDIA_HANDLER_H

#include "tuya_cloud_types.h"
#include "tuya_ipc_p2p.h"
#include "tuya_ipc_media.h"
#include "tuya_ipc_media_utils.h"

#ifdef __cplusplus
extern "C" {
#endif

VOID IPC_APP_Register_Media_Info_CB(INOUT IPC_MEDIA_INFO_S *p_media_info);

VOID TUYA_APP_Enable_Speaker_CB(BOOL enabled);

VOID TUYA_APP_Rev_Audio_CB(IN CONST MEDIA_FRAME_S *p_audio_frame,
                           TUYA_AUDIO_SAMPLE_E audio_sample,
                           TUYA_AUDIO_DATABITS_E audio_databits,
                           TUYA_AUDIO_CHANNEL_E audio_channel);

#ifdef __cplusplus
}
#endif

#endif  /* _TUYA_IPC_MEDIA_HANDLER_H */
