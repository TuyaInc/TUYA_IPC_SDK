/*********************************************************************************
  *Copyright(C),2017, 涂鸦科技 www.tuya.comm
  *FileName:    tuya_ipc_p2p.h
**********************************************************************************/

#ifndef __TUYA_IPC_P2P_H__
#define __TUYA_IPC_P2P_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "tuya_cloud_types.h"
#include "tuya_cloud_error_code.h"
#include "tuya_ipc_media.h"

/**
 * \brief IPC传输模块回调信息类型定义
 * \struct TRANSFER_EVENT_E
 */
typedef enum
{
    TRANS_LIVE_VIDEO_START,         /**< 直播开始视频，参数为C2C_TRANS_CTRL_VIDEO_START*/
    TRANS_LIVE_VIDEO_STOP,         /**< 直播结束视频，参数为C2C_TRANS_CTRL_VIDEO_STOP*/
    TRANS_LIVE_AUDIO_START,         /**< 直播开始音频，参数为C2C_TRANS_CTRL_AUDIO_START*/
    TRANS_LIVE_AUDIO_STOP,         /**< 直播结束音频，参数为C2C_TRANS_CTRL_AUDIO_STOP*/
    TRANS_LIVE_VIDEO_CLARITY_SET, /**< 设置视频直播清晰度 ，参数为*/
    TRANS_LIVE_VIDEO_CLARITY_QUERY, /**< 查询视频直播清晰度 ，参数为*/
    TRANS_LIVE_LOAD_ADJUST, /**< 直播负载变更 ，参数为*/
    TRANS_PLAYBACK_LOAD_ADJUST, /**< 开始回放 ，参数为*/
    TRANS_PLAYBACK_QUERY_MONTH_SIMPLIFY, /* 按月查询本地视频信息，参数为  */
    TRANS_PLAYBACK_QUERY_DAY_TS, /* 按天查询本地视频信息，参数为  */

    TRANS_PLAYBACK_START_TS, /* 开始回放视频，参数为  */
    TRANS_PLAYBACK_PAUSE, /**< 暂停回放视频，参数为  */
    TRANS_PLAYBACK_RESUME, /**< 继续回放视频，参数为  */
    TRANS_PLAYBACK_MUTE, /**< 静音，参数为  */
    TRANS_PLAYBACK_UNMUTE, /**< 取消静音，参数为  */
    TRANS_PLAYBACK_STOP, /**< 停止回放视频，参数为  */ 

    TRANS_SPEAKER_START, /**< 开始对讲，无参数 */
    TRANS_SPEAKER_STOP,  /**< 停止对讲，无参数 */
    TRANS_ABILITY_QUERY,/**< 能力查询 C2C_TRANS_QUERY_FIXED_ABI_REQ*/
}TRANSFER_EVENT_E;

/**
 * \brief 设备在线状态
 * \enum TRANSFER_ONLINE_E
 */
typedef enum
{
    TY_DEVICE_OFFLINE,
    TY_DEVICE_ONLINE,
}TRANSFER_ONLINE_E;
    
// 能力集类型
typedef enum{
    TY_CMD_QUERY_IPC_FIXED_ABILITY_TYPE_VIDEO = 0x1,      // 设备端是否支持video
    TY_CMD_QUERY_IPC_FIXED_ABILITY_TYPE_SPEAKER = 0x2,    // 设备端是否支持扬声器
    TY_CMD_QUERY_IPC_FIXED_ABILITY_TYPE_MIC = 0x4,        // 设备端是否支持麦克风
}TY_CMD_QUERY_IPC_FIXED_ABILITY_TYPE;


// 能力集查询 C2C_CMD_QUERY_FIXED_ABILITY
// request, response
typedef struct tagC2CCmdQueryFixedAbility{
    unsigned int channel;
    unsigned int ability_mask;//能力结果按位赋值
}C2C_TRANS_QUERY_FIXED_ABI_REQ, C2C_TRANS_QUERY_FIXED_ABI_RESP;
/**
 * \brief 视频直播清晰度类型
 * \enum TRANSFER_VIDEO_CLARITY_TYPE_E
 */
typedef enum
{
    TY_VIDEO_CLARITY_STANDARD = 0, /**< 标清 */
    TY_VIDEO_CLARITY_HIGH,     /**< 高清 */
}TRANSFER_VIDEO_CLARITY_TYPE_E;

/**
 * \brief 直播模式下申请修改或者查询清晰度回调参数结构体
 * \struct C2C_TRANS_LIVE_CLARITY_PARAM_S
 */
typedef struct
{
    TRANSFER_VIDEO_CLARITY_TYPE_E clarity; /**< 视频清晰度 */
    VOID *pReserved;
}C2C_TRANS_LIVE_CLARITY_PARAM_S;

//预览相关操作结构体
typedef struct tagC2C_TRANS_CTRL_LIVE_VIDEO{
    unsigned int channel;
}C2C_TRANS_CTRL_VIDEO_START,C2C_TRANS_CTRL_VIDEO_STOP;

typedef struct tagC2C_TRANS_CTRL_LIVE_AUDIO{
    unsigned int channel;   
}C2C_TRANS_CTRL_AUDIO_START,C2C_TRANS_CTRL_AUDIO_STOP;


typedef struct
{
    UINT start_timestamp; /**< 回放文件开始时间戳（以秒为单位） */
    UINT end_timestamp;   /**< 回放文件结束时间戳（以秒为单位） */
} PLAYBACK_TIME_S;

// 回放片元
typedef struct tagPLAY_BACK_ALARM_FRAGMENT{
    unsigned int type;//智能事件类型，后续再定义
	PLAYBACK_TIME_S time_sect;
}PLAY_BACK_ALARM_FRAGMENT;

// 回放片元数组
typedef struct{
    unsigned int file_count;                            // 当天回放文件数量
    PLAY_BACK_ALARM_FRAGMENT file_arr[0];                  // 回放文件片段数组
}PLAY_BACK_ALARM_INFO_ARR;

// 按天查询 request, response
typedef struct{
    unsigned int channel;
    unsigned int year;                                  // 要查询的年份
    unsigned int month;                                 // 要查询的月份
    unsigned int day;                                   // 要查询的天数    
}C2C_TRANS_QUERY_PB_DAY_REQ;

typedef struct{
    unsigned int channel;
    unsigned int year;                                  // 要查询的年份
    unsigned int month;                                 // 要查询的月份
    unsigned int day;                                   // 要查询的天数    
    PLAY_BACK_ALARM_INFO_ARR * alarm_arr;                    // 用户返回的查询结果
}C2C_TRANS_QUERY_PB_DAY_RESP;

/**
UINT一共有32位，每1位表示对应天数是否有数据，最右边一位表示第0天。
比如 day = 26496 = B0110 0111 1000 0000
那么表示第7,8,9,10,13,14天有回放数据。
 */
// 按月查询有回放数据的天 request, response
typedef struct tagC2CCmdQueryPlaybackInfoByMonth{
	unsigned int channel;
    unsigned int year;                                  // 要查询的年份
    unsigned int month;                                 // 要查询的月份
    unsigned int day;                                  // 有回放数据的天
}C2C_TRANS_QUERY_PB_MONTH_REQ, C2C_TRANS_QUERY_PB_MONTH_RESP;


// request
//回放相关操作结构体
typedef struct tagC2C_TRANS_CTRL_PB_START{
    unsigned int channel;
    PLAYBACK_TIME_S time_sect;   
    UINT playTime;  /**< 实际回放开始时间戳（以秒为单位） */
}C2C_TRANS_CTRL_PB_START;

typedef struct tagC2C_TRANS_CTRL_PB_STOP{
    unsigned int channel;   //回放通道号
}C2C_TRANS_CTRL_PB_STOP;

typedef struct tagC2C_TRANS_CTRL_PB_PAUSE{
    unsigned int channel;
}C2C_TRANS_CTRL_PB_PAUSE,C2C_TRANS_CTRL_PB_RESUME;

typedef struct tagC2C_TRANS_CTRL_PB_MUTE{
    unsigned int channel;
}C2C_TRANS_CTRL_PB_MUTE,C2C_TRANS_CTRL_PB_UNMUTE;

/**
 * \brief 回放模式下网络负载变更通知回调参数结构体
 * \note 当负载变更时，IPC传输模块会有回调信息通知用户，用户可以根据负载变更状况调整视频帧直播帧率等。
 * \struct EVENT_PLAYBACK_LOAD_PARAM_S
 */
typedef struct
{
    INT client_index; /**< 通道号 */
    INT curr_load_level; /**< 0:best 5:worst */
    INT new_load_level; /**< 0:best 5:worst */

    VOID *pReserved;
}C2C_TRANS_PB_LOAD_PARAM_S;

typedef struct
{
    INT client_index; /**< 通道号 */
    INT curr_load_level; /**< 0:best 5:worst */
    INT new_load_level; /**< 0:best 5:worst */

    VOID *pReserved;
}C2C_TRANS_LIVE_LOAD_PARAM_S;

//END

/**
 * \brief IPC传输音频帧结构体定义
 * \struct TRANSFER_AUDIO_FRAME_S
 */
typedef struct
{
    TUYA_CODEC_ID audio_codec;
    TUYA_AUDIO_SAMPLE_E audio_sample;
    TUYA_AUDIO_DATABITS_E audio_databits;
    TUYA_AUDIO_CHANNEL_E audio_channel;
    BYTE *p_audio_buf; /**< 音频帧buffer */
    UINT buf_len;  /**< 音频帧buffer长度 */
    UINT64 pts;     //音频采集显示时间戳
    UINT64 timestamp;// Timestamp of the frame, in milliseconds 
    VOID *p_reserved;
}TRANSFER_AUDIO_FRAME_S;

/**
 * \brief 视频帧类型
 * \enum TRANSFER_VIDEO_FRAME_TYPE_E
 */
typedef enum
{
    TY_VIDEO_FRAME_PBFRAME,     /**< P/B frame */
    TY_VIDEO_FRAME_IFRAME,     /**< I frame */
}TRANSFER_VIDEO_FRAME_TYPE_E;

/**
 * \brief IPC传输视频帧结构体定义
 * \struct TRANSFER_VIDEO_FRAME_S
 */
typedef struct
{
    TUYA_CODEC_ID video_codec;
    TRANSFER_VIDEO_FRAME_TYPE_E video_frame_type;
    BYTE *p_video_buf; /**< 视频帧buffer */
    UINT buf_len;  /**< 视频帧buffer长度 */
    UINT64 pts; //音视频采集显示时间戳
    UINT64 timestamp;   // Timestamp of the frame, in milliseconds
    VOID *p_reserved;
}TRANSFER_VIDEO_FRAME_S;


/**
 * \typedef TRANSFER_EVENT_CB
 * \brief IPC传输模块回调函数定义
 * \param[in] event 回调信息定义
 * \param[in] args 回调信息附带的参数
 */
typedef VOID (*TRANSFER_EVENT_CB)(IN CONST TRANSFER_EVENT_E event, IN CONST PVOID args);

/**
 * \typedef TRANSFER_REV_AUDIO_CB
 * \brief IPC对讲模式声音回调函数定义
 * \param [in] p_audio_frame APP侧发送的音频帧
 * \param [in] time_stamp 音频帧采集的时间戳
 * \param [in] frame_no 音频帧序号
 */
typedef VOID (*TRANSFER_REV_AUDIO_CB)(IN CONST TRANSFER_AUDIO_FRAME_S *p_audio_frame, IN CONST UINT frame_no);

/**
 * \typedef TRANSFER_ONLINE_CB
 * \brief 设备在线状态回调函数定义
 * \param[in] status 设备在线状态
 */
typedef VOID (*TRANSFER_ONLINE_CB)(IN TRANSFER_ONLINE_E status);

/**
 * \brief 直播模式下的直播视频质量
 * \enum TRANS_LIVE_QUALITY_E
 */
typedef enum
{
    TRANS_LIVE_QUALITY_MAX = 0,     /**< ex. 640*480, 15fps, 320kbps (or 1280x720, 5fps, 320kbps) */
    TRANS_LIVE_QUALITY_HIGH,        /**< ex. 640*480, 10fps, 256kbps */
    TRANS_LIVE_QUALITY_MIDDLE,      /**< ex. 320*240, 15fps, 256kbps */
    TRANS_LIVE_QUALITY_LOW,         /**< ex. 320*240, 10fps, 128kbps */
    TRANS_LIVE_QUALITY_MIN,         /**< ex. 160*120, 10fps, 64kbps */
}TRANS_LIVE_QUALITY_E;

/**
 * \brief 直播模式下默认清晰度（多路码流格式下）
 * \enum TRANS_DEFAULT_QUALITY_E
 */
typedef enum
{
    TRANS_DEFAULT_STANDARD = 0,     /** 默认发送标清*/
    TRANS_DEFAULT_HIGH,        /**< 默认发送高清*/
}TRANS_DEFAULT_QUALITY_E;

/**
 * \brief P2P传输基本信息结构体
 * \struct TUYA_IPC_TRANSFER_VAR_S
 */
typedef struct
{
    TRANSFER_ONLINE_CB online_cb;/**< 设备P2P在线状态回调 */
    TRANSFER_REV_AUDIO_CB on_rev_audio_cb; /**< 对讲模式下获取来自APP端的声音回调函数定义 */
    TUYA_CODEC_ID rev_audio_codec; /**< 对讲模式下允许获取来自APP端的声音编码格式 */
    TUYA_AUDIO_SAMPLE_E audio_sample;/**< 对讲模式下允许获取来自APP端的声音编码格式 */
    TUYA_AUDIO_DATABITS_E audio_databits;/**< 对讲模式下允许获取来自APP端的声音编码格式 */
    TUYA_AUDIO_CHANNEL_E audio_channel;/**< 对讲模式下允许获取来自APP端的声音编码格式 */
    TRANSFER_EVENT_CB on_event_cb; /**< 涂鸦音视频传输模块事件回调响应 */
    TRANS_LIVE_QUALITY_E live_quality; /**< 直播时视频质量 */
    INT max_client_num;/**< 最大支持客户端数量 */
    IPC_MEDIA_INFO_S AVInfo;
    TRANS_DEFAULT_QUALITY_E defLiveMode;  /**< 多路码流可选择默认播放清晰度，单路码流可不考虑 */  
    VOID *p_reserved;
}TUYA_IPC_TRANSFER_VAR_S;

/**
 * \fn OPERATE_RET tuya_ipc_tranfser_init(IN CONST TUYA_IPC_TRANSFER_VAR_S *p_var)
 * \brief 启动涂鸦IPC传输模块
 * \note IPC传输模块需要在IPC模块已经激活后才可以启动。
 * \param[in] p_var 运行环境配置结构体
 * \return OPERATE_RET
 */
OPERATE_RET tuya_ipc_tranfser_init(IN CONST TUYA_IPC_TRANSFER_VAR_S *p_var);

/**
 * \fn OPERATE_RET tuya_ipc_tranfser_close()
 * \brief 启动涂鸦现开启的所有链接
 * \note 
 * \param[in]
 * \return OPERATE_RET
 */
OPERATE_RET tuya_ipc_tranfser_close(VOID);

/**
 * \fn OPERATE_RET tuya_ipc_tranfser_quit(VOID)
 * \brief 停止涂鸦IPC传输模块
 * \return OPERATE_RET
 */
OPERATE_RET tuya_ipc_tranfser_quit(VOID);



/**
 * \fn OPERATE_RET tuya_ipc_playback_send_video_frame(IN CONST UINT client, IN CONST TRANSFER_VIDEO_FRAME_S *p_video_frame)
 * \brief IPC传输模块发送视频回放帧
 * \param[in] client cliend id
 * \param[in] p_video_frame 视频回放帧
 * \return OPERATE_RET
 */
OPERATE_RET tuya_ipc_playback_send_video_frame(IN CONST UINT client, IN CONST TRANSFER_VIDEO_FRAME_S *p_video_frame);

/**
 * \fn OPERATE_RET tuya_ipc_playback_send_audio_frame(IN CONST UINT client, IN CONST TRANSFER_AUDIO_FRAME_S *p_audio_frame)
 * \brief IPC传输模块发送音频回放帧
 * \param[in] client cliend id
 * \param[in] p_audio_frame 音频回放帧
 * \return OPERATE_RET
 */
OPERATE_RET tuya_ipc_playback_send_audio_frame(IN CONST UINT client, IN CONST TRANSFER_AUDIO_FRAME_S *p_audio_frame);

/**
 * \fn OPERATE_RET tuya_ipc_playback_send_finish(IN CONST UINT client)
 * \brief 本地视频文件播放完成后告知client
 * \param[in] client cliend id
 * \return OPERATE_RET
 */
OPERATE_RET tuya_ipc_playback_send_finish(IN CONST UINT client);


/**
 * \fn OPERATE_RET tuya_ipc_malloc_time_s(IN CONST UINT num, OUT PLAYBACK_TIME_S **p_p_time_arr)
 * \brief 获取查询的指针数组内存
 * \param[in] num 要获取的数量
 * \param[out] p_p_time_arr 内存指针
 * \return OPERATE_RET
*/
OPERATE_RET tuya_ipc_malloc_time_s(IN CONST UINT num, OUT PLAY_BACK_ALARM_INFO_ARR **p_p_time_arr);

/**
 * \brief 设备连接状态信息结构体
 * \struct TUYA_IPC_TRANSFER_VAR_S
 */
typedef struct
{
    UCHAR p2p_mode; /**< 0: P2P mode, 1: Relay mode, 2: LAN mode, 255: Not connected. */
    UCHAR local_nat_type; /**< The local NAT type, 0: Unknown type, 1: Type 1, 2: Type 2, 3: Type 3, 10: TCP only */
    UCHAR remote_nat_type; /**< The remote NAT type, 0: Unknown type, 1: Type 1, 2: Type 2, 3: Type 3, 10: TCP only */
    UCHAR relay_type; /**< 0: Not Relay, 1: UDP Relay, 2: TCP Relay */

    VOID *p_reserved;
}CLIENT_CONNECT_INFO_S;

/**
 * \fn OPERATE_RET tuya_ipc_get_client_conn_info(OUT UINT *p_client_num, OUT CLIENT_CONNECT_INFO_S **p_p_conn_info)
 * \brief IPC传输模块获取连接client的状态
 * \param[out] p_client_num 有效client的数目
 * \param[out] p_p_conn_info 指针数组
 * \return OPERATE_RET
 */
OPERATE_RET tuya_ipc_get_client_conn_info(OUT UINT *p_client_num, OUT CLIENT_CONNECT_INFO_S **p_p_conn_info);

/**
 * \fn OPERATE_RET tuya_ipc_free_client_conn_info(IN CLIENT_CONNECT_INFO_S *p_conn_info)
 * \brief 释放 tuya_ipc_get_client_conn_info 返回的指针
 * \param[in] p_conn_info 要释放的指针
 * \return OPERATE_RET
 */
OPERATE_RET tuya_ipc_free_client_conn_info(IN CLIENT_CONNECT_INFO_S *p_conn_info);


#ifdef __cplusplus
}
#endif

#endif
