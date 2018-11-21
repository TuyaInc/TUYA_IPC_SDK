#ifndef _TUYA_IPC_MEDIA_H_
#define _TUYA_IPC_MEDIA_H_


#ifdef __cplusplus
extern "C" {
#endif

#include "tuya_cloud_types.h"

#define MAX_MEDIA_FRAME_SIZE    (200*1024)

typedef enum
{
    E_CHANNEL_VIDEO_MAIN = 0,    
    E_CHANNEL_VIDEO_SUB,
    E_CHANNEL_VIDEO_3RD,
    E_CHANNEL_VIDEO_4TH,
    E_CHANNEL_VIDEO_MAX = 8,    //最大支持视频码流路数
    E_CHANNEL_AUDIO,
    E_CHANNEL_AUDIO_2RD,
    E_CHANNEL_AUDIO_3RD,
    E_CHANNEL_AUDIO_4TH,
    E_CHANNEL_MAX = 16
}CHANNEL_E;

typedef enum
{
    E_VIDEO_PB_FRAME = 0,
    E_VIDEO_I_FRAME,
    E_VIDEO_TS_FRAME,
    E_AUDIO_FRAME,
    E_MEDIA_FRAME_TYPE_MAX
}MEDIA_FRAME_TYPE_E;

//该枚举值涉及与服务端、APP端共用，后续不允许改变已有枚举的值
typedef enum
{
    TUYA_CODEC_VIDEO_MPEG4 = 0,
    TUYA_CODEC_VIDEO_H263,
    TUYA_CODEC_VIDEO_H264,
    TUYA_CODEC_VIDEO_MJPEG,
    TUYA_CODEC_VIDEO_H265,
    TUYA_CODEC_VIDEO_MAX = 99,
    
    TUYA_CODEC_AUDIO_ADPCM,
    TUYA_CODEC_AUDIO_PCM,
    TUYA_CODEC_AUDIO_AAC_RAW,
    TUYA_CODEC_AUDIO_AAC_ADTS,
    TUYA_CODEC_AUDIO_AAC_LATM,
    TUYA_CODEC_AUDIO_G711U,
    TUYA_CODEC_AUDIO_G711A,
    TUYA_CODEC_AUDIO_G726,
    TUYA_CODEC_AUDIO_SPEEX,
    TUYA_CODEC_AUDIO_MP3,
    TUYA_CODEC_AUDIO_MAX = 199,
    TUYA_CODEC_INVALID
}TUYA_CODEC_ID;

typedef enum
{
    TUYA_AUDIO_SAMPLE_8K     = 8000,
    TUYA_AUDIO_SAMPLE_11K    = 11000,
    TUYA_AUDIO_SAMPLE_12K    = 12000,
    TUYA_AUDIO_SAMPLE_16K    = 16000,
    TUYA_AUDIO_SAMPLE_22K    = 22000,
    TUYA_AUDIO_SAMPLE_24K    = 24000,
    TUYA_AUDIO_SAMPLE_32K    = 32000,
    TUYA_AUDIO_SAMPLE_44K    = 44000,
    TUYA_AUDIO_SAMPLE_48K    = 48000,
    TUYA_AUDIO_SAMPLE_MAX    = 0xFFFFFFFF
}TUYA_AUDIO_SAMPLE_E;

typedef enum
{
    TUYA_VIDEO_BITRATE_64K   = 64,
    TUYA_VIDEO_BITRATE_128K  = 128,
    TUYA_VIDEO_BITRATE_256K  = 256,
    TUYA_VIDEO_BITRATE_512K  = 512,
    TUYA_VIDEO_BITRATE_768K  = 768,
    TUYA_VIDEO_BITRATE_1M    = 1024,
    TUYA_VIDEO_BITRATE_1_5M  = 1536,
    TUYA_VIDEO_BITRATE_2M    = 2048,
    TUYA_VIDEO_BITRATE_4M    = 4096,
    TUYA_VIDEO_BITRATE_8M    = 8192,
    TUYA_VIDEO_BITRATE_MAX   = 8192
}TUYA_VIDEO_BITRATE_E;           //Kbps

typedef enum
{
    TUYA_AUDIO_DATABITS_8 = 8,
    TUYA_AUDIO_DATABITS_16 = 16,
    TUYA_AUDIO_DATABITS_MAX = 0xFF
}TUYA_AUDIO_DATABITS_E;

typedef enum
{
    TUYA_AUDIO_CHANNEL_MONO,
    TUYA_AUDIO_CHANNEL_STERO,
}TUYA_AUDIO_CHANNEL_E;

typedef struct
{
    BOOL channel_enable[E_CHANNEL_MAX];

    UINT video_fps[E_CHANNEL_VIDEO_MAX];
    UINT video_gop[E_CHANNEL_VIDEO_MAX];
    TUYA_VIDEO_BITRATE_E video_bitrate[E_CHANNEL_VIDEO_MAX];
    UINT video_width[E_CHANNEL_VIDEO_MAX];
    UINT video_height[E_CHANNEL_VIDEO_MAX];
    UINT video_freq[E_CHANNEL_VIDEO_MAX];
    TUYA_CODEC_ID video_codec[E_CHANNEL_VIDEO_MAX];

    TUYA_CODEC_ID audio_codec[E_CHANNEL_MAX];
    UINT audio_fps[E_CHANNEL_MAX];
    TUYA_AUDIO_SAMPLE_E audio_sample[E_CHANNEL_MAX];
    TUYA_AUDIO_DATABITS_E audio_databits[E_CHANNEL_MAX];
    TUYA_AUDIO_CHANNEL_E audio_channel[E_CHANNEL_MAX];
}IPC_MEDIA_INFO_S;

typedef struct
{
    UINT    type;
    UINT    size;
    UINT64  timestamp;
    UINT64  pts;
}STORAGE_FRAME_HEAD_S;

typedef struct
{
    MEDIA_FRAME_TYPE_E type;
    BYTE    *p_buf;
    UINT    size;
    UINT64  pts;
    UINT64  timestamp;
}MEDIA_FRAME_S;



#ifdef __cplusplus
}
#endif

#endif /*_TUYA_IPC_MEDIA_H_*/
