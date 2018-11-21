
#ifndef _TUYA_RING_BUFFER_
#define _TUYA_RING_BUFFER_

#ifdef __cplusplus
extern "C" {
#endif


#include "tuya_cloud_types.h"
#include "tuya_ipc_media.h"
#include "tuya_cloud_error_code.h"
#if ENABLE_MONITOR_WORK
#include "ty_monitor.h"
#define RING_BUF_CHECK_PERIOD 2
#define RING_BUF_CHECK_ERR_LIMIT 20
#endif

//如果没有必要，建议不要修改该值
#define MAX_BUFFER_SECONDS     10

typedef VOID (*FUNC_REQUEST_I_FRAME)(INT channel);

typedef enum
{
    E_USER_STREAM_STORAGE = 0,
    E_USER_COULD_STORAGE = 1,
    E_USER_ECHO_SHOW = 2,
    E_USER_CHROMECAST = 3,
    E_USER_P2P_USER = 4, //支持多路P2P，可以采用 E_USER_P2P_USER + p2p index的方式区分user
    E_USER_NUM_MAX = 16
}USER_INDEX_E;

typedef struct 
{
    UINT index;
    MEDIA_FRAME_TYPE_E type;
    UCHAR *rawData;
    UINT size;
    UINT64 pts;
    UINT64 timestamp;
    UINT seqNo;
    UCHAR *extraData;
    UINT extraSize;
}Ring_Buffer_Node_S;

/* 初始化一个 ring buffer
channel: buffer ring支持多个，针对IPC可能会存在的主码流、子码流、三码流等场景，
bitrate: 本路码流的码率，Kbps。
fps: 每秒帧数。
requestIframeCB: 申请I帧的回调函数，用于新用户接入请求时加快I帧出图。为NULL则不会主动申请。
*/
OPERATE_RET tuya_ipc_ring_buffer_init(CHANNEL_E channel, UINT bitrate, UINT fps, FUNC_REQUEST_I_FRAME requestIframeCB);

#if 0
/* 销毁一个 ring buffer*/
OPERATE_RET tuya_ipc_ring_buffer_destory(CHANNEL_E channel);
#endif
/* 往ring buffer中追加新的数据
*/
OPERATE_RET tuya_ipc_ring_buffer_append_data(CHANNEL_E channel, UCHAR *addr, UINT size, MEDIA_FRAME_TYPE_E type, UINT64 pts);

/* 往ring buffer中追加额外数据。
用于产生的同一帧音视频数据需要保存不同的格式的情况。比如需要同时缓存PCM和AAC两种编码。
该数据保存在node的extraData、extraSize字段。
注意：必须接在tuya_ipc_ring_buffer_append_data之后配对使用，才能保证保存在同一帧节点之下。
*/
OPERATE_RET tuya_ipc_ring_buffer_append_extra_data(CHANNEL_E channel, UCHAR *dataAddr, UINT dataSize);

/* 从ringbuffer中取数据。 由userIndex区分同一份环形缓存的不同用户，各位维护取数据的状态。
当取数据落后写数据超过阈值时，取数据的接口会自动跳帧取到最新的I帧或最新的音频帧。
*/
Ring_Buffer_Node_S *tuya_ipc_ring_buffer_get_video_frame(CHANNEL_E channel, USER_INDEX_E userIndex, BOOL isRetry);
Ring_Buffer_Node_S *tuya_ipc_ring_buffer_get_audio_frame(CHANNEL_E channel, USER_INDEX_E userIndex, BOOL isRetry);

/* 从最新节点开始往回查找frameNum数量的帧，视频以找到I帧为准。
*/
Ring_Buffer_Node_S *tuya_ipc_ring_buffer_find_pre_video_by_frame(CHANNEL_E channel, UINT frameNum);
Ring_Buffer_Node_S *tuya_ipc_ring_buffer_find_pre_audio_by_frame(CHANNEL_E channel, UINT frameNum);

/* 从最新节点开始往回查找frameNum数量的帧，视频以找到I帧为准。并更新userIndex对应的user到该新的节点。
即获取到预录数据后，使用tuya_ipc_ring_buffer_get_video_frame/tuya_ipc_ring_buffer_get_audio_frame接口
会自动从预录点开始往后取数据。
*/
Ring_Buffer_Node_S *tuya_ipc_ring_buffer_get_pre_video_frame(CHANNEL_E channel, USER_INDEX_E userIndex, UINT frameNum);
Ring_Buffer_Node_S *tuya_ipc_ring_buffer_get_pre_audio_frame(CHANNEL_E channel, USER_INDEX_E userIndex, UINT frameNum);

/* 锚定userIndex用户到指定的node
*/
VOID tuya_ipc_ring_buffer_anchor_user_to_node(CHANNEL_E channel, USER_INDEX_E userIndex, Ring_Buffer_Node_S *node);

/* 从userIndex的当前位置开始，往后找指定type且seqNo更大的节点
*/
Ring_Buffer_Node_S *tuya_ipc_ring_buffer_get_next_target_frame(CHANNEL_E channel, USER_INDEX_E userIndex, UINT seqNo, MEDIA_FRAME_TYPE_E type);

/* 清除userIndex对应的ringbuffer用户状态，下一次get frame时会从最新的开始取用
*/
VOID tuya_ipc_ring_buffer_clean_user_state(CHANNEL_E channel, USER_INDEX_E userIndex);

#ifdef __cplusplus
}
#endif

#endif  /*_TUYA_RING_BUFFER_*/


