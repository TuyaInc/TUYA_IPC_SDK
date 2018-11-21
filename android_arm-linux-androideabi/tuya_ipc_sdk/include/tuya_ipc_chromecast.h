#ifndef __TUYA_IPC_CHROMECAST_H__
#define __TUYA_IPC_CHROMECAST_H__

#include <stdint.h>

#include "tuya_cloud_types.h"
#include "tuya_ipc_media.h"


#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


typedef enum TUYA_STREAM_TYPE_ {
	TUYA_STREAM_TYPE_AUDIO,
	TUYA_STREAM_TYPE_VIDEO,
} TUYA_STREAM_TYPE_E;


typedef struct TUYA_STREAM_FRAME_ {
    uint64_t    			utcms;          ///< utc时间
    uint64_t    			pts;            ///< pts时间
    uint32_t    			seq;            ///< 序列号
    TUYA_STREAM_TYPE_E    	type;           ///< 数据流类型，表征是音频还是视频
    TUYA_CODEC_ID			codec;
    int32_t     			length;         ///< 数据长度
    void*       			pbuff;          ///< 数据指针
} TUYA_STREAM_FRAME_S;


typedef INT (*TUYA_CHROMECAST_START)(PVOID context, PVOID priv_data);
typedef INT (*TUYA_CHROMECAST_STOP)(PVOID context, PVOID priv_data);
typedef INT (*TUYA_CHROMECAST_GET_FRAME)(PVOID context, TUYA_STREAM_TYPE_E type, TUYA_STREAM_FRAME_S* pframe);
typedef INT (*TUYA_CHROMECAST_GET_FRAME_RELEASE)(PVOID context, TUYA_STREAM_TYPE_E type, TUYA_STREAM_FRAME_S* pframe);


typedef struct {
	PVOID						        pcontext;
	TUYA_CHROMECAST_START 		        start;
	TUYA_CHROMECAST_STOP		        stop;
	TUYA_CHROMECAST_GET_FRAME 	        get_frame;
    TUYA_CHROMECAST_GET_FRAME_RELEASE   get_frame_release;
} TUYA_CHROMECAST_CALLBACK;


typedef enum TUYA_STREAM_SOURCE_ {
	TUYA_STREAM_SOURCE_RINGBUFFER,
	TUYA_STREAM_SOURCE_CUSTOMER,
} TUYA_STREAM_SOURCE_E;


typedef enum TUYA_STREAM_TRANSMIT_MODE_ {
	TUYA_STREAM_TRANSMIT_MODE_ASYNC,
	TUYA_STREAM_TRANSMIT_MODE_SYNC,
} TUYA_STREAM_TRANSMIT_MODE_E;


typedef struct {
	CHANNEL_E					audio_channel;
	CHANNEL_E					video_channel;
	TUYA_STREAM_SOURCE_E 		src;
	TUYA_STREAM_TRANSMIT_MODE_E	mode;
	IPC_MEDIA_INFO_S* 			pminfo;
	TUYA_CHROMECAST_CALLBACK    cbk;
} TUYA_CHROMECAST_PARAM_S;


OPERATE_RET tuya_ipc_chromecast_init(TUYA_CHROMECAST_PARAM_S* param);


OPERATE_RET tuya_ipc_chromecast_deinit(VOID);


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif