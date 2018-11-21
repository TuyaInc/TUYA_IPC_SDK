/*********************************************************************************
  *Copyright(C),2015-2020, 涂鸦科技 www.tuya.comm
  *FileName: tuya_ipc_media_handler.c
  *
  * 文件描述：
  * 1. 音视频函数API实现
  *
  * 开发者工作：
  * 1. 设置本地音视频属性。
  * 2. 实现对讲模式所需要的API。
  *
**********************************************************************************/
#include <string.h>
#include <stdio.h>

#include "tuya_ipc_media_handler.h"


/* 设置本地音视频属性 */
VOID IPC_APP_Register_Media_Info_CB(INOUT IPC_MEDIA_INFO_S *p_media_info)
{
    if(p_media_info == NULL)
    {
        return;
    }
    memset(p_media_info, 0 , sizeof(IPC_MEDIA_INFO_S));

    /* 高清主码流，视频配置 */
    /* 注意1： 如果设备主码流支持多种视频流配置，这里请把各项设置为允许配置的上限值 */
    /* 注意2： E_CHANNEL_VIDEO_MAIN必须存在，是SDK内部存储业务的数据源。对于只有编码器只出一路流的情况，请关闭E_CHANNEL_VIDEO_SUB */
    p_media_info->channel_enable[E_CHANNEL_VIDEO_MAIN] = TRUE;    /* 是否开启本地高清视频流 */
    p_media_info->video_fps[E_CHANNEL_VIDEO_MAIN] = 30;  /* FPS */
    p_media_info->video_gop[E_CHANNEL_VIDEO_MAIN] = 60;  /* GOP */
    p_media_info->video_bitrate[E_CHANNEL_VIDEO_MAIN] = TUYA_VIDEO_BITRATE_1M; /* 码率上限 */
    p_media_info->video_width[E_CHANNEL_VIDEO_MAIN] = 640; /* 单帧分辨率 宽 */
    p_media_info->video_height[E_CHANNEL_VIDEO_MAIN] = 360;/* 单帧分辨率 高 */
    p_media_info->video_freq[E_CHANNEL_VIDEO_MAIN] = 90000; /* 时钟频率 */
    p_media_info->video_codec[E_CHANNEL_VIDEO_MAIN] = TUYA_CODEC_VIDEO_H264; /* 编码格式 */

    /* 标清子码流，视频配置 */
    /* 请注意如果设备子码流支持多种视频流配置，这里请把各项设置为允许配置的上限值 */
    p_media_info->channel_enable[E_CHANNEL_VIDEO_SUB] = TRUE;     /* 是否开启本地标清视频流 */
    p_media_info->video_fps[E_CHANNEL_VIDEO_SUB] = 30;  /* FPS */
    p_media_info->video_gop[E_CHANNEL_VIDEO_SUB] = 60;  /* GOP */
    p_media_info->video_bitrate[E_CHANNEL_VIDEO_SUB] = TUYA_VIDEO_BITRATE_512K; /* 码率上限 */
    p_media_info->video_width[E_CHANNEL_VIDEO_SUB] = 640; /* 单帧分辨率 宽 */
    p_media_info->video_height[E_CHANNEL_VIDEO_SUB] = 360;/* 单帧分辨率 高 */
    p_media_info->video_freq[E_CHANNEL_VIDEO_SUB] = 90000; /* 时钟频率 */
    p_media_info->video_codec[E_CHANNEL_VIDEO_SUB] = TUYA_CODEC_VIDEO_H264; /* 编码格式 */

    /* 音频码流配置. 注意：SDK内部P2P预览、云存储、本地存储使用的都是E_CHANNEL_AUDIO的数据 */
    p_media_info->channel_enable[E_CHANNEL_AUDIO] = TRUE;         /* 是否开启本地声音采集 */
    p_media_info->audio_codec[E_CHANNEL_AUDIO] = TUYA_CODEC_AUDIO_PCM;/* 编码格式 */
    p_media_info->audio_sample[E_CHANNEL_AUDIO] = TUYA_AUDIO_SAMPLE_8K;/* 采样率 */
    p_media_info->audio_databits[E_CHANNEL_AUDIO] = TUYA_AUDIO_DATABITS_16;/* 位宽 */
    p_media_info->audio_channel[E_CHANNEL_AUDIO] = TUYA_AUDIO_CHANNEL_MONO;/* 信道 */
    p_media_info->audio_fps[E_CHANNEL_AUDIO] = 25;/* 每秒分片数 */

    /* 有更多音视频流的情况，设置对应channel_enable为TRUE，并设置响应的参数 */
}

/* 对讲模式声音回调，开启关闭扬声器硬件 */
VOID TUYA_APP_Enable_Speaker_CB(BOOL enabled)
{
    printf("enable speaker %d \r\n", enabled);
    //TODO
    /* 开发者需要实现开启关闭扬声器硬件操作，如果IPC硬件特性无需显式开启，该函数留空即可 */
}

/* 对讲模式声音回调，开启关闭扬声器硬件 */
VOID TUYA_APP_Rev_Audio_CB(IN CONST MEDIA_FRAME_S *p_audio_frame,
                           TUYA_AUDIO_SAMPLE_E audio_sample,
                           TUYA_AUDIO_DATABITS_E audio_databits,
                           TUYA_AUDIO_CHANNEL_E audio_channel)
{
    printf("rev audio cb len:%u sample:%d db:%d channel:%d\r\n", p_audio_frame->size, audio_sample, audio_databits, audio_channel);
    //PCM-Format 8K 16Bit MONO
    //TODO
    /* 开发者需要实现扬声器播放声音操作 */

}


