
#ifndef _TUYA_RING_BUFFER_
#define _TUYA_RING_BUFFER_

#ifdef __cplusplus
extern "C" {
#endif


#include "tuya_cloud_types.h"
#include "tuya_ipc_media.h"
#include "tuya_cloud_error_code.h"

typedef VOID (*FUNC_REQUEST_I_FRAME)(VOID);

typedef enum
{
    E_USER_STREAM_STORAGE = 0,
    E_USER_COULD_STORAGE,
    E_USER_ECHO_SHOW,
    E_USER_CHROMECAST,
    E_USER_P2P_USER, //支持多路P2P，可以采用 E_USER_P2P_USER + p2p index的方式区分user
    E_USER_NUM_MAX = 30
}USER_INDEX_E;

typedef struct 
{
    UINT_T index;
    MEDIA_FRAME_TYPE_E type;
    UCHAR_T *rawData;
    UINT_T size;
    UINT64_T pts;
    UINT64_T timestamp;
    UINT_T seqNo;
    UCHAR_T *extraData;
    UINT_T extraSize;
}Ring_Buffer_Node_S;

/* 初始化一个 ring buffer
channel: buffer ring支持多个，针对IPC可能会存在的主码流、子码流、三码流等场景，
bitrate: 本路码流的码率，Kbps。
fps: 每秒帧数。
requestIframeCB: 申请I帧的回调函数，用于新用户接入请求时加快I帧出图。为NULL则不会主动申请。
*/
OPERATE_RET tuya_ipc_ring_buffer_init(CHANNEL_E channel, UINT_T bitrate, UINT_T fps, FUNC_REQUEST_I_FRAME requestIframeCB);

#if 0
/* 销毁一个 ring buffer*/
OPERATE_RET tuya_ipc_ring_buffer_destory(CHANNEL_E channel);
#endif
/* 往ring buffer中追加新的数据
*/
OPERATE_RET tuya_ipc_ring_buffer_append_data(CHANNEL_E channel, UCHAR_T *addr, UINT_T size, MEDIA_FRAME_TYPE_E type, UINT64_T pts);

/* 从ringbuffer中取数据。 由userIndex区分同一份环形缓存的不同用户，各位维护取数据的状态。
当取数据落后写数据超过阈值时，取数据的接口会自动跳帧取到最新的I帧或最新的音频帧。
*/
Ring_Buffer_Node_S *tuya_ipc_ring_buffer_get_video_frame(CHANNEL_E channel, USER_INDEX_E userIndex, BOOL_T isRetry);
Ring_Buffer_Node_S *tuya_ipc_ring_buffer_get_audio_frame(CHANNEL_E channel, USER_INDEX_E userIndex, BOOL_T isRetry);
Ring_Buffer_Node_S *tuya_ipc_ring_buffer_get_pre_video_frame(CHANNEL_E channel, USER_INDEX_E userIndex, UINT_T frameNum);
Ring_Buffer_Node_S *tuya_ipc_ring_buffer_get_pre_audio_frame(CHANNEL_E channel, USER_INDEX_E userIndex, UINT_T frameNum);

#ifdef __cplusplus
}
#endif

#endif  /*_TUYA_RING_BUFFER_*/


