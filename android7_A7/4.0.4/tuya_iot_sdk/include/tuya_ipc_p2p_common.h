/*********************************************************************************
  *Copyright(C),2017, 涂鸦科技 www.tuya.comm
  *FileName:    tuya_ipc_p2p_common.h
**********************************************************************************/

#ifndef __TUYA_IPC_P2P_COMMON_H__
#define __TUYA_IPC_P2P_COMMON_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "tuya_cloud_types.h"
#include "tuya_cloud_error_code.h"

//tutk
#define P2P_ID_LEN 25
#define P2P_NAME_LEN 8
#define P2P_PASSWD_LEN 8
#define P2P_GW_LOCAL_KEY_LEN 16 //securt lan dps key len

typedef struct
{
    CHAR_T p2p_id[P2P_ID_LEN + 1];
    CHAR_T p2p_name[P2P_NAME_LEN + 1];
    CHAR_T p2p_passwd[P2P_PASSWD_LEN + 1];
    CHAR_T gw_local_key[P2P_GW_LOCAL_KEY_LEN + 1];
    VOID *p_reserved;
}TUYA_IPC_P2P_AUTH_S;

//ppcs
#define SY_INIT_STR_LEN (128)
#define SY_CRC_KEY_LEN  (10)
#define SY_P2P_KEY_LEN  (20)
#define SY_SVR_KEY_LEN  (20)
#define SY_DID_LEN  (30)
#define SY_WAKEUP_SVR_LEN (30)
                    
/**
 * \brief 尚云认证结构体
 * \struct SY_AUTH_PARAM_S
 */
typedef struct
{
    CHAR_T init_str[SY_INIT_STR_LEN];
    CHAR_T crc_key[SY_CRC_KEY_LEN];
    CHAR_T p2p_key[SY_P2P_KEY_LEN];
    CHAR_T did[SY_DID_LEN];
    CHAR_T wakeup_svrkey[SY_SVR_KEY_LEN];

    CHAR_T wakeup_server1[SY_WAKEUP_SVR_LEN];
    CHAR_T wakeup_server2[SY_WAKEUP_SVR_LEN];
    CHAR_T wakeup_server3[SY_WAKEUP_SVR_LEN];
}P2P_AUTH_PARAM_S;

typedef enum{
    TY_AV_CODEC_VIDEO_UNKOWN = 0,
    TY_AV_CODEC_VIDEO_MPEG4 = 0x10,
    TY_AV_CODEC_VIDEO_H263 = 0x11,
    TY_AV_CODEC_VIDEO_H264 = 0x12,
    TY_AV_CODEC_VIDEO_MJPEG = 0x13,
    TY_AV_CODEC_VIDEO_H265 = 0x14,    
    
    TY_AV_CODEC_AUDIO_ADPCM = 0x80,
    TY_AV_CODEC_AUDIO_PCM = 0x81,
    TY_AV_CODEC_AUDIO_AAC_RAW = 0x82,
    TY_AV_CODEC_AUDIO_AAC_ADTS = 0x83,
    TY_AV_CODEC_AUDIO_AAC_LATM = 0x84,
    TY_AV_CODEC_AUDIO_G711U = 0x85,    //10
    TY_AV_CODEC_AUDIO_G711A = 0x86,
    TY_AV_CODEC_AUDIO_G726 = 0x87,
    TY_AV_CODEC_AUDIO_SPEEX = 0x88,
    TY_AV_CODEC_AUDIO_MP3 = 0x89,

    TY_AV_CODEC_MAX = 0xFF
}TY_AV_CODEC_ID;

/**
 * \brief 音频采样率
 * \enum TRANSFER_AUDIO_SAMPLE_E
 */
typedef enum
{
    TY_AUDIO_SAMPLE_8K,
    TY_AUDIO_SAMPLE_11K,
    TY_AUDIO_SAMPLE_12K,
    TY_AUDIO_SAMPLE_16K,
    TY_AUDIO_SAMPLE_22K,
    TY_AUDIO_SAMPLE_24K,
    TY_AUDIO_SAMPLE_32K,
    TY_AUDIO_SAMPLE_44K,
    TY_AUDIO_SAMPLE_48K,
    TY_AUDIO_SAMPLE_96K,
}TRANSFER_AUDIO_SAMPLE_E;

/**
 * \brief 音频位宽
 * \enum TRANSFER_AUDIO_DATABITS_E
 */
typedef enum
{
    TY_AUDIO_DATABITS_8,
    TY_AUDIO_DATABITS_16,
}TRANSFER_AUDIO_DATABITS_E;

/**
 * \brief 音频通道
 * \enum TRANSFER_AUDIO_CHANNEL_E
 */
typedef enum
{
    TY_AUDIO_CHANNEL_MONO,
    TY_AUDIO_CHANNEL_STERO,
}TRANSFER_AUDIO_CHANNEL_E;

OPERATE_RET tuya_ipc_p2p_get_auth_param(INOUT P2P_AUTH_PARAM_S *p_auth_param);
OPERATE_RET tuya_ipc_p2p_get_id(INOUT CHAR_T p2p_id[]);
OPERATE_RET tuya_ipc_check_p2p_auth_update();


#ifdef __cplusplus
}
#endif

#endif

