#ifndef _TUYA_IPC_MEDIA_H_
#define _TUYA_IPC_MEDIA_H_


#ifdef __cplusplus
extern "C" {
#endif

#include "tuya_cloud_types.h"

#define MAX_MEDIA_FRAME_SIZE    (200*1024)
#define MAX_VIDEO_CHANNEL       2

typedef enum
{
    E_CHANNEL_VIDEO_MAIN = 0,    
    E_CHANNEL_VIDEO_SUB,
    E_CHANNEL_AUDIO,
    E_CHANNEL_MAX
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
    TUYA_VIDEO_BITRATE_1M    = 1024,
    TUYA_VIDEO_BITRATE_2M    = 2048,
    TUYA_VIDEO_BITRATE_4M    = 4096,
    TUYA_VIDEO_BITRATE_8M    = 8192,
    TUYA_VIDEO_BITRATE_MAX   = 0xFFFF
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
    BOOL_T channel_enable[E_CHANNEL_MAX];

    UINT_T video_fps[MAX_VIDEO_CHANNEL];
    UINT_T video_gop[MAX_VIDEO_CHANNEL];
    TUYA_VIDEO_BITRATE_E video_bitrate[MAX_VIDEO_CHANNEL];
    UINT_T video_width[MAX_VIDEO_CHANNEL];
    UINT_T video_height[MAX_VIDEO_CHANNEL];
    UINT_T video_freq[MAX_VIDEO_CHANNEL];
    TUYA_CODEC_ID video_codec[MAX_VIDEO_CHANNEL];

    TUYA_CODEC_ID audio_codec;
    UINT_T audio_fps;
    TUYA_AUDIO_SAMPLE_E audio_sample;
    TUYA_AUDIO_DATABITS_E audio_databits;
    TUYA_AUDIO_CHANNEL_E audio_channel;
}IPC_MEDIA_INFO_S;

typedef struct
{
    UINT_T    type;
    UINT_T    size;
    UINT64_T  timestamp;
    UINT64_T  pts;
}STORAGE_FRAME_HEAD_S;

typedef struct
{
    MEDIA_FRAME_TYPE_E type;
    BYTE_T    *p_buf;
    UINT_T    size;
    UINT64_T  pts;
    UINT64_T  timestamp;
}MEDIA_FRAME_S;



#ifdef __cplusplus
}
#endif

#endif /*_TUYA_IPC_MEDIA_H_*/
