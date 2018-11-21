/*********************************************************************************
  *Copyright(C),2015-2020, 涂鸦科技 www.tuya.comm
  *FileName: tuya_ipc_media_utils.c
  *
  * 文件描述：
  * 1. 音视频工具API实现
  *
  * 本文件代码为工具代码，用户可以不用关心
  * 请勿随意修改该文件中的任何内容，如需修改请联系涂鸦产品经理！！
  *
**********************************************************************************/

#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/statfs.h>  

#include "tuya_ipc_p2p.h"
#include "tuya_ipc_api.h"
#include "tuya_ipc_echo_show.h"
#include "tuya_ipc_chromecast.h"
#include "tuya_ipc_media_utils.h"
#include "tuya_ipc_media_handler.h"
#include "tuya_ipc_stream_storage.h"
#include "tuya_ipc_cloud_storage.h"
#include "tuya_ring_buffer.h"

#define PR_ERR(fmt, ...)    printf("Err:"fmt"\r\n", ##__VA_ARGS__)
#define PR_DEBUG(fmt, ...)  printf("Dbg:"fmt"\r\n", ##__VA_ARGS__)
//#define PR_TRACE(fmt, ...)  printf("Trace:"fmt"\r\n", ##__VA_ARGS__)
#define PR_TRACE(fmt, ...)

//单日最大事件个数，超过该值，在回放时会认为存储有异常，无法播放
//该值设置太大会影响查询效率，反应到APP端耗时太长影响体验
#define MAX_EVENT_NUM_PER_DAY   (500)


STATIC IPC_MEDIA_INFO_S s_media_info = {0};
VOID IPC_APP_Refresh_Media_Info(VOID)
{
    IPC_APP_Register_Media_Info_CB(&s_media_info);

    PR_DEBUG("channel_enable:%d %d %d", s_media_info.channel_enable[0], s_media_info.channel_enable[1], s_media_info.channel_enable[2]);

    PR_DEBUG("fps:%u", s_media_info.video_fps[E_CHANNEL_VIDEO_MAIN]);
    PR_DEBUG("gop:%u", s_media_info.video_gop[E_CHANNEL_VIDEO_MAIN]);
    PR_DEBUG("bitrate:%u kbps", s_media_info.video_bitrate[E_CHANNEL_VIDEO_MAIN]);
    PR_DEBUG("video_main_width:%u", s_media_info.video_width[E_CHANNEL_VIDEO_MAIN]);
    PR_DEBUG("video_main_height:%u", s_media_info.video_height[E_CHANNEL_VIDEO_MAIN]);
    PR_DEBUG("video_freq:%u", s_media_info.video_freq[E_CHANNEL_VIDEO_MAIN]);
    PR_DEBUG("video_codec:%d", s_media_info.video_codec[E_CHANNEL_VIDEO_MAIN]);

    PR_DEBUG("audio_codec:%d", s_media_info.audio_codec[E_CHANNEL_AUDIO]);
    PR_DEBUG("audio_sample:%d", s_media_info.audio_sample[E_CHANNEL_AUDIO]);
    PR_DEBUG("audio_databits:%d", s_media_info.audio_databits[E_CHANNEL_AUDIO]);
    PR_DEBUG("audio_channel:%d", s_media_info.audio_channel[E_CHANNEL_AUDIO]);
}

/*
---------------------------------------------------------------------------------
Stream_Storage相关代码起始位置
---------------------------------------------------------------------------------
*/

typedef struct
{
    CHANNEL_E channel;
}STORAGE_THREAD_ARG;

#define DISK_CLEAN_THRESH           100   //触发删除文件的磁盘剩余空间阈值 MB
#define DISK_CHECK_FREQ             10    //磁盘空间检查时间间隔，秒
void *thread_disk_clean(void *arg)
{
    struct statfs space_stat;
    UINT64 freeBytes;
    INT ret;
    CHAR *base_path = (CHAR *)arg;

    PR_DEBUG("start thread to check free space, sd path: %s",base_path);
    memset((void*)&space_stat, 0, sizeof(struct statfs));
    while (TRUE)
    {
        ret = statfs(base_path, &space_stat);
        if(ret != 0)
        {
            PR_ERR("check free space fails. %d", ret);
            sleep(DISK_CHECK_FREQ);
            continue;
        }
        freeBytes = ((UINT64)space_stat.f_bavail * (UINT64)space_stat.f_bsize) >> 20;
        PR_DEBUG("curr free %s: %llu", base_path, freeBytes);
        if(freeBytes < DISK_CLEAN_THRESH)
        {
            PR_DEBUG("curr free %s: %u < %llu Delete.",
                     base_path, freeBytes, DISK_CLEAN_THRESH);

            if(E_DELETE_EVENT == tuya_ipc_ss_delete_oldest_event()) //有成功删除的情况，不sleep继续查看空间是否足够
            {
                continue;
            }
        }
        sleep(DISK_CHECK_FREQ);
    }

    pthread_exit(0);
}

OPERATE_RET TUYA_APP_Init_Stream_Storage(IN CONST CHAR *p_sd_base_path)
{
    STATIC BOOL s_stream_storage_inited = FALSE;
    if(s_stream_storage_inited == TRUE)
    {
        PR_DEBUG("The Stream Storage Is Already Inited");
        return OPRT_OK;
    }

    PR_DEBUG("Init Stream_Storage SD:%s", p_sd_base_path);
    OPERATE_RET ret = tuya_ipc_ss_init(p_sd_base_path, &s_media_info, MAX_EVENT_NUM_PER_DAY);
    if(ret != OPRT_OK)
    {
        PR_ERR("Init Main Video Stream_Storage Fail. %d", ret);
        return OPRT_COM_ERROR;
    }

    pthread_t disk_clean_thread;
    STATIC CHAR base_dir[128];
    strncpy(base_dir,p_sd_base_path,128);
    pthread_create(&disk_clean_thread, NULL, thread_disk_clean, (VOID *)base_dir);
    pthread_detach(disk_clean_thread);

    //开启连续存储
    tuya_ipc_ss_set_write_mode(SS_WRITE_MODE_ALL);
    //或者触发自定义事件存储
    //tuya_ipc_ss_set_write_mode(SS_WRITE_MODE_EVENT);

    return OPRT_OK;
}

/*
---------------------------------------------------------------------------------
Stream_Storage相关代码结束位置
---------------------------------------------------------------------------------
*/

/*
---------------------------------------------------------------------------------
RingBuffer相关代码起始位置
---------------------------------------------------------------------------------
*/

OPERATE_RET TUYA_APP_Init_Ring_Buffer(VOID)
{
    STATIC BOOL s_ring_buffer_inited = FALSE;
    if(s_ring_buffer_inited == TRUE)
    {
        PR_DEBUG("The Ring Buffer Is Already Inited");
        return OPRT_OK;
    }

    CHANNEL_E channel;
    OPERATE_RET ret;
    for( channel = E_CHANNEL_VIDEO_MAIN; channel < E_CHANNEL_MAX; channel++ )
    {
        PR_DEBUG("init ring buffer Channel:%d Enable:%d", channel, s_media_info.channel_enable[channel]);
        if(s_media_info.channel_enable[channel] == TRUE)
        {
            if(channel == E_CHANNEL_AUDIO)
            {
                PR_DEBUG("audio_sample %d, audio_databits %d, audio_fps %d",s_media_info.audio_sample[E_CHANNEL_AUDIO],s_media_info.audio_databits[E_CHANNEL_AUDIO],s_media_info.audio_fps[E_CHANNEL_AUDIO]);
                ret = tuya_ipc_ring_buffer_init(channel, s_media_info.audio_sample[E_CHANNEL_AUDIO]*s_media_info.audio_databits[E_CHANNEL_AUDIO]/1024,s_media_info.audio_fps[E_CHANNEL_AUDIO],NULL);
            }
            else
            {
                PR_DEBUG("video_bitrate %d, video_fps %d",s_media_info.video_bitrate[channel],s_media_info.video_fps[channel]);
                ret = tuya_ipc_ring_buffer_init(channel, s_media_info.video_bitrate[channel],s_media_info.video_fps[channel],NULL);
            }
            if(ret != 0)
            {
                PR_ERR("init ring buffer fails. %d %d", channel, ret);
                return OPRT_MALLOC_FAILED;
            }
            PR_DEBUG("init ring buffer success. channel:%d", channel);
        }
    }

    s_ring_buffer_inited = TRUE;

    return OPRT_OK;
}

OPERATE_RET TUYA_APP_Put_Frame(IN CONST CHANNEL_E channel, IN CONST MEDIA_FRAME_S *p_frame)
{
    PR_TRACE("Put Frame. Channel:%d type:%d size:%u pts:%llu ts:%llu",
             channel, p_frame->type, p_frame->size, p_frame->pts, p_frame->timestamp);

    OPERATE_RET ret = tuya_ipc_ring_buffer_append_data(channel,p_frame->p_buf, p_frame->size,p_frame->type,p_frame->pts);

    if(ret != OPRT_OK)
    {
        PR_ERR("Put Frame Fail.%d Channel:%d type:%d size:%u pts:%llu ts:%llu",ret,
                 channel, p_frame->type, p_frame->size, p_frame->pts, p_frame->timestamp);
    }
    return ret;
}

OPERATE_RET TUYA_APP_Get_Frame_Backwards(IN CONST CHANNEL_E channel,
                                                  IN CONST USER_INDEX_E user_index,
                                                  IN CONST UINT backward_frame_num,
                                                  INOUT MEDIA_FRAME_S *p_frame)
{
    if(p_frame == NULL)
    {
        PR_ERR("input is null");
        return OPRT_INVALID_PARM;
    }

    Ring_Buffer_Node_S *node;
    if(channel == E_CHANNEL_VIDEO_MAIN || channel == E_CHANNEL_VIDEO_SUB)
        node = tuya_ipc_ring_buffer_get_pre_video_frame(channel,user_index,backward_frame_num);
    else
        node = tuya_ipc_ring_buffer_get_pre_audio_frame(channel,user_index,backward_frame_num);
    if(node != NULL)
    {
        p_frame->p_buf = node->rawData;
        p_frame->size = node->size;
        p_frame->timestamp = node->timestamp;
        p_frame->type = node->type;
        p_frame->pts = node->pts;
        return OPRT_OK;
    }
    else
    {
        PR_ERR("Fail to re-locate for user %d backward %d frames, channel %d",user_index,backward_frame_num,channel);
        return OPRT_COM_ERROR;
    }
}

OPERATE_RET TUYA_APP_Get_Frame(IN CONST CHANNEL_E channel, IN CONST USER_INDEX_E user_index, IN CONST BOOL isRetry, IN CONST BOOL ifBlock, INOUT MEDIA_FRAME_S *p_frame)
{
    if(p_frame == NULL)
    {
        PR_ERR("input is null");
        return OPRT_INVALID_PARM;
    }
    PR_TRACE("Get Frame Called. channel:%d user:%d retry:%d", channel, user_index, isRetry);

    Ring_Buffer_Node_S *node = NULL;
    while(node == NULL)
    {
        if(channel == E_CHANNEL_VIDEO_MAIN || channel == E_CHANNEL_VIDEO_SUB)
        {
            node = tuya_ipc_ring_buffer_get_video_frame(channel,user_index,isRetry);
        }
        else if(channel == E_CHANNEL_AUDIO)
        {
            node = tuya_ipc_ring_buffer_get_audio_frame(channel,user_index,isRetry);
        }
        if(NULL == node)
        {
            if(ifBlock)
            {
                usleep(10*1000);
            }
            else
            {
                return OPRT_NO_MORE_DATA;
            }
        }
    }
    p_frame->p_buf = node->rawData;
    p_frame->size = node->size;
    p_frame->timestamp = node->timestamp;
    p_frame->type = node->type;
    p_frame->pts = node->pts;

    PR_TRACE("Get Frame Success. channel:%d user:%d retry:%d size:%u ts:%ull type:%d pts:%llu",
             channel, user_index, isRetry, p_frame->size, p_frame->timestamp, p_frame->type, p_frame->pts);
    return OPRT_OK;
}

/*
---------------------------------------------------------------------------------
RingBuffer相关代码结束位置
---------------------------------------------------------------------------------
*/

/*
---------------------------------------------------------------------------------
TUTK P2P相关代码起始位置
---------------------------------------------------------------------------------
*/

typedef struct
{
    BOOL enabled;
    TRANSFER_VIDEO_CLARITY_TYPE_E live_clarity;
    UINT max_users;
    TUYA_CODEC_ID p2p_audio_codec;
}TUYA_APP_P2P_MGR;

STATIC TUYA_APP_P2P_MGR s_p2p_mgr = {0};

STATIC VOID __TUYA_APP_media_frame_TO_trans_video(IN CONST MEDIA_FRAME_S *p_in, INOUT TRANSFER_VIDEO_FRAME_S *p_out)
{
    p_out->video_codec = s_media_info.video_codec[E_CHANNEL_VIDEO_MAIN];
    p_out->video_frame_type = p_in->type == E_VIDEO_PB_FRAME ? TY_VIDEO_FRAME_PBFRAME:TY_VIDEO_FRAME_IFRAME;
    p_out->p_video_buf = p_in->p_buf;
    p_out->buf_len = p_in->size;
    p_out->pts = p_in->pts;
    p_out->timestamp = p_in->timestamp;
}

STATIC VOID __TUYA_APP_media_frame_TO_trans_audio(IN CONST MEDIA_FRAME_S *p_in, INOUT TRANSFER_AUDIO_FRAME_S *p_out)
{  
    p_out->audio_codec = s_media_info.audio_codec[E_CHANNEL_AUDIO];
    p_out->audio_sample = s_media_info.audio_sample[E_CHANNEL_AUDIO];
    p_out->audio_databits = s_media_info.audio_databits[E_CHANNEL_AUDIO];
    p_out->audio_channel = s_media_info.audio_channel[E_CHANNEL_AUDIO];
    p_out->p_audio_buf = p_in->p_buf;
    p_out->buf_len = p_in->size;
    p_out->pts = p_in->pts;
    p_out->timestamp = p_in->timestamp;
}

STATIC VOID __TUYA_APP_ss_pb_event_cb(IN UINT pb_idx, IN SS_PB_EVENT_E pb_event, IN PVOID args)
{
    PR_DEBUG("ss pb rev event: %u %d", pb_idx, pb_event);
    if(pb_event == SS_PB_FINISH)
    {
        tuya_ipc_playback_send_finish(pb_idx);
    }
}

STATIC VOID __TUYA_APP_ss_pb_get_video_cb(IN UINT pb_idx, IN CONST MEDIA_FRAME_S *p_frame)
{
    TRANSFER_VIDEO_FRAME_S video_frame = {0};
    __TUYA_APP_media_frame_TO_trans_video(p_frame, &video_frame);
    tuya_ipc_playback_send_video_frame(pb_idx, &video_frame);
}

STATIC VOID __TUYA_APP_ss_pb_get_audio_cb(IN UINT pb_idx, IN CONST MEDIA_FRAME_S *p_frame)
{
    TRANSFER_AUDIO_FRAME_S audio_frame = {0};
    __TUYA_APP_media_frame_TO_trans_audio(p_frame, &audio_frame);
    tuya_ipc_playback_send_audio_frame(pb_idx, &audio_frame);
}

STATIC VOID __depereated_online_cb(IN TRANSFER_ONLINE_E status)
{

}

/* 传输事件回调 */
STATIC VOID __TUYA_APP_p2p_event_cb(IN CONST TRANSFER_EVENT_E event, IN CONST PVOID args)
{
    PR_DEBUG("p2p rev event cb=[%d] ", event);
    switch (event)
    {
        case TRANS_LIVE_VIDEO_START:
        {
            C2C_TRANS_CTRL_VIDEO_START * parm = (C2C_TRANS_CTRL_VIDEO_START *)args;
            PR_DEBUG("chn[%u] video start",parm->channel);
            break;
        }
        case TRANS_LIVE_VIDEO_STOP:
        {
            C2C_TRANS_CTRL_VIDEO_STOP * parm = (C2C_TRANS_CTRL_VIDEO_STOP *)args;
            PR_DEBUG("chn[%u] video stop",parm->channel);
            break;
        }
        case TRANS_LIVE_AUDIO_START:
        {
            C2C_TRANS_CTRL_AUDIO_START * parm = (C2C_TRANS_CTRL_AUDIO_START *)args;
            PR_DEBUG("chn[%u] audio start",parm->channel);
            break;
        }
        case TRANS_LIVE_AUDIO_STOP:
        {
            C2C_TRANS_CTRL_AUDIO_STOP * parm = (C2C_TRANS_CTRL_AUDIO_STOP *)args;
            PR_DEBUG("chn[%u] audio stop",parm->channel);
            break;
        }
        case TRANS_SPEAKER_START:
        {
            PR_DEBUG("enbale audio speaker");
            TUYA_APP_Enable_Speaker_CB(TRUE);
            break;
        }
        case TRANS_SPEAKER_STOP:
        {
            PR_DEBUG("disable audio speaker");
            TUYA_APP_Enable_Speaker_CB(FALSE);
            break;
        }
        case TRANS_LIVE_LOAD_ADJUST:
        {
            C2C_TRANS_LIVE_LOAD_PARAM_S *quality = (C2C_TRANS_LIVE_LOAD_PARAM_S *)args;
            PR_DEBUG("live quality %d -> %d", quality->curr_load_level, quality->new_load_level);
            break;
        }
		case TRANS_PLAYBACK_LOAD_ADJUST:
        {
            C2C_TRANS_PB_LOAD_PARAM_S *quality= (C2C_TRANS_PB_LOAD_PARAM_S *)args;
            PR_DEBUG("pb idx:%d quality %d -> %d", quality->client_index, quality->curr_load_level, quality->new_load_level);
            break;
        }
        case TRANS_ABILITY_QUERY:
        {
            C2C_TRANS_QUERY_FIXED_ABI_REQ * pAbiReq;
            pAbiReq = (C2C_TRANS_QUERY_FIXED_ABI_REQ *)args;
            pAbiReq->ability_mask = TY_CMD_QUERY_IPC_FIXED_ABILITY_TYPE_VIDEO |
                                    TY_CMD_QUERY_IPC_FIXED_ABILITY_TYPE_SPEAKER |
                                    TY_CMD_QUERY_IPC_FIXED_ABILITY_TYPE_MIC;          
            break;
        }
        case TRANS_PLAYBACK_QUERY_MONTH_SIMPLIFY:
        {
            C2C_TRANS_QUERY_PB_MONTH_REQ *p = (C2C_TRANS_QUERY_PB_MONTH_REQ *)args;
            PR_DEBUG("pb query by month: %d-%d", p->year, p->month);

            OPERATE_RET ret = tuya_ipc_pb_query_by_month(p->year, p->month, &(p->day));
            if (OPRT_OK != ret)
            {
                PR_ERR("pb query by month: %d-%d ret:%d", p->year, p->month, ret);
            }

            break;
        }
        case TRANS_PLAYBACK_QUERY_DAY_TS:
        {
            C2C_TRANS_QUERY_PB_DAY_RESP *pquery = (C2C_TRANS_QUERY_PB_DAY_RESP *)args;
            PR_DEBUG("pb_ts query by day: idx[%d]%d-%d-%d", pquery->channel,pquery->year, pquery->month, pquery->day);
            SS_QUERY_DAY_TS_ARR_S *p_day_ts = NULL;
            OPERATE_RET ret = tuya_ipc_pb_query_by_day(pquery->channel,pquery->year, pquery->month, pquery->day, &p_day_ts);
            if (OPRT_OK != ret)
            {
                PR_ERR("pb_ts query by day: %d-%d-%d Fail", pquery->channel,pquery->year, pquery->month, pquery->day);
                break;
            }
            if (p_day_ts){
                printf("%s %d count = %d\n",__FUNCTION__,__LINE__,p_day_ts->file_count);
                PLAY_BACK_ALARM_INFO_ARR * pResult = (PLAY_BACK_ALARM_INFO_ARR *)malloc(sizeof(PLAY_BACK_ALARM_INFO_ARR) + p_day_ts->file_count*sizeof(PLAY_BACK_ALARM_FRAGMENT));
                if (NULL == pResult){
                    printf("%s %d malloc failed \n",__FUNCTION__,__LINE__);
                    free(p_day_ts);
                    pquery->alarm_arr = NULL;
                    return;
                }

                INT i;
                pResult->file_count = p_day_ts->file_count;
                for (i = 0; i < p_day_ts->file_count; i++){
                    pResult->file_arr[i].time_sect.start_timestamp = p_day_ts->file_arr[i].start_timestamp;
                    pResult->file_arr[i].time_sect.end_timestamp = p_day_ts->file_arr[i].end_timestamp;
                }
                pquery->alarm_arr = pResult;
                free(p_day_ts);

            }else{
               pquery->alarm_arr = NULL; 
            }
            break;
        }
        case TRANS_PLAYBACK_START_TS:
        {
            /* 开始回放时client会带上开始时间点，这里简单起见，只进行了日志打印 */
            C2C_TRANS_CTRL_PB_START *pParam = (C2C_TRANS_CTRL_PB_START *)args;
            PR_DEBUG("PB StartTS idx:%d %u [%u %u]", pParam->channel, pParam->playTime, pParam->time_sect.start_timestamp, pParam->time_sect.end_timestamp);

            SS_FILE_TIME_TS_S pb_file_info;
            int ret;
            memset(&pb_file_info, 0x00, sizeof(SS_FILE_TIME_TS_S));
            memcpy(&pb_file_info, &pParam->time_sect, sizeof(SS_FILE_TIME_TS_S));
            
            ret = tuya_ipc_ss_pb_start(pParam->channel, __TUYA_APP_ss_pb_event_cb, __TUYA_APP_ss_pb_get_video_cb, __TUYA_APP_ss_pb_get_audio_cb);
            if (0 != ret){
                printf("%s %d pb_start failed\n",__FUNCTION__,__LINE__);
                tuya_ipc_playback_send_finish(pParam->channel);
            }else{
                if (0 != tuya_ipc_ss_pb_seek(pParam->channel, &pb_file_info, pParam->playTime)){
                    printf("%s %d pb_seek failed\n",__FUNCTION__,__LINE__);
                    tuya_ipc_playback_send_finish(pParam->channel);
                }
            }

            break;
        }
        case TRANS_PLAYBACK_PAUSE:
        {
            C2C_TRANS_CTRL_PB_PAUSE *pParam = (C2C_TRANS_CTRL_PB_PAUSE *)args;
            PR_DEBUG("PB Pause idx:%d", pParam->channel);

            tuya_ipc_ss_pb_set_status(pParam->channel, SS_PB_PAUSE);
            break;
        }
        case TRANS_PLAYBACK_RESUME:
        {
            C2C_TRANS_CTRL_PB_RESUME *pParam = (C2C_TRANS_CTRL_PB_RESUME *)args;
            PR_DEBUG("PB Resume idx:%d", pParam->channel);

            tuya_ipc_ss_pb_set_status(pParam->channel, SS_PB_RESUME);
            break;
        }
        case TRANS_PLAYBACK_MUTE:
        {
            C2C_TRANS_CTRL_PB_MUTE *pParam = (C2C_TRANS_CTRL_PB_MUTE *)args;
            PR_DEBUG("PB idx:%d mute", pParam->channel);

            tuya_ipc_ss_pb_set_status(pParam->channel, SS_PB_MUTE);
            break;
        }
        case TRANS_PLAYBACK_UNMUTE:
        {
            C2C_TRANS_CTRL_PB_UNMUTE *pParam = (C2C_TRANS_CTRL_PB_UNMUTE *)args;
            PR_DEBUG("PB idx:%d unmute", pParam->channel);

            tuya_ipc_ss_pb_set_status(pParam->channel, SS_PB_UN_MUTE);
            break;
        }
        case TRANS_PLAYBACK_STOP:
        {
            C2C_TRANS_CTRL_PB_STOP *pParam = (C2C_TRANS_CTRL_PB_STOP *)args;
            PR_DEBUG("PB Stop idx:%d", pParam->channel);

            tuya_ipc_ss_pb_stop(pParam->channel);
            break;
        }
        case TRANS_LIVE_VIDEO_CLARITY_SET:
        {
            C2C_TRANS_LIVE_CLARITY_PARAM_S *pParam = (C2C_TRANS_LIVE_CLARITY_PARAM_S *)args;
            PR_DEBUG("set clarity:%d", pParam->clarity);
            if((pParam->clarity == TY_VIDEO_CLARITY_STANDARD)||(pParam->clarity == TY_VIDEO_CLARITY_HIGH))
            {
                PR_DEBUG("set clarity:%d OK", pParam->clarity);
                s_p2p_mgr.live_clarity = pParam->clarity;
            }
            break;
        }
        case TRANS_LIVE_VIDEO_CLARITY_QUERY:
        {
            C2C_TRANS_LIVE_CLARITY_PARAM_S *pParam = (C2C_TRANS_LIVE_CLARITY_PARAM_S *)args;
            pParam->clarity = s_p2p_mgr.live_clarity;
            PR_DEBUG("query larity:%d", pParam->clarity);
            break;
        }
        default:
            break;
    }
}

STATIC VOID __TUYA_APP_rev_audio_cb(IN CONST TRANSFER_AUDIO_FRAME_S *p_audio_frame, IN CONST UINT time_stamp, IN CONST UINT frame_no)
{
    MEDIA_FRAME_S audio_frame = {0};
    audio_frame.p_buf = p_audio_frame->p_audio_buf;
    audio_frame.size = p_audio_frame->buf_len;

    PR_TRACE("Rev Audio. size:%u audio_codec:%d audio_sample:%d audio_databits:%d audio_channel:%d",p_audio_frame->buf_len,
             p_audio_frame->audio_codec, p_audio_frame->audio_sample, p_audio_frame->audio_databits, p_audio_frame->audio_channel);

    TUYA_APP_Rev_Audio_CB( &audio_frame, TUYA_AUDIO_SAMPLE_8K, TUYA_AUDIO_DATABITS_16, TUYA_AUDIO_CHANNEL_MONO);
}

OPERATE_RET TUYA_APP_Enable_P2PTransfer(IN UINT max_users)
{
    if(s_p2p_mgr.enabled == TRUE)
    {
        PR_DEBUG("The Tutk P2P Is Already Inited");
        return OPRT_OK;
    }

    PR_DEBUG("Init Tutk With Max Users:%u", max_users);

    s_p2p_mgr.enabled = TRUE;
    s_p2p_mgr.max_users = max_users;

    s_p2p_mgr.p2p_audio_codec = s_media_info.audio_codec[E_CHANNEL_AUDIO];

    TUYA_IPC_TRANSFER_VAR_S p2p_var = {0};
    p2p_var.online_cb = __depereated_online_cb;
    p2p_var.on_rev_audio_cb = __TUYA_APP_rev_audio_cb;
    p2p_var.rev_audio_codec = s_media_info.audio_codec[E_CHANNEL_AUDIO];
    p2p_var.audio_sample = s_media_info.audio_sample[E_CHANNEL_AUDIO];
    p2p_var.audio_databits = s_media_info.audio_databits[E_CHANNEL_AUDIO];
    p2p_var.audio_channel = s_media_info.audio_channel[E_CHANNEL_AUDIO];
    p2p_var.on_event_cb = __TUYA_APP_p2p_event_cb;
    p2p_var.live_quality = TRANS_LIVE_QUALITY_MAX;
    p2p_var.max_client_num = max_users;
    memcpy(&p2p_var.AVInfo,&s_media_info,sizeof(IPC_MEDIA_INFO_S));
    tuya_ipc_tranfser_init(&p2p_var);

    return OPRT_OK;
}

/*
---------------------------------------------------------------------------------
TUTK P2P相关代码结束位置
---------------------------------------------------------------------------------
*/



/*
---------------------------------------------------------------------------------
EchoShow相关代码起始位置
---------------------------------------------------------------------------------
*/
#if ENABLE_ECHO_SHOW == 1

INT TUYA_APP_Echoshow_Start(PVOID context, PVOID priv_data)
{
    printf("echoshow start...\n");

    return 0;
}

INT TUYA_APP_Echoshow_Stop(PVOID context, PVOID priv_data)
{
    printf("echoshow stop...\n");

    return 0;
}

INT TUYA_APP_Chromecast_Start(PVOID context, PVOID priv_data)
{
    printf("chromecast start...\n");

    return 0;
}

INT TUYA_APP_Chromecast_Stop(PVOID context, PVOID priv_data)
{
    printf("chromecast stop...\n");

    return 0;
}

OPERATE_RET TUYA_APP_Enable_EchoShow_Chromecast(VOID)
{
     STATIC BOOL s_echoshow_inited = FALSE;
     if(s_echoshow_inited == TRUE)
     {
         PR_DEBUG("The EchoShow Is Already Inited");
         return OPRT_OK;
     }

    PR_DEBUG("Init EchoShow");

    TUYA_ECHOSHOW_PARAM_S es_param = {0};

    es_param.pminfo = &s_media_info;
    es_param.cbk.pcontext = NULL;
    es_param.cbk.start = TUYA_APP_Echoshow_Start;
    es_param.cbk.stop = TUYA_APP_Echoshow_Stop;

    tuya_ipc_echoshow_init(&es_param);

    TUYA_CHROMECAST_PARAM_S param = {0};

    param.pminfo = &s_media_info;
    param.cbk.pcontext = NULL;
    param.cbk.start = TUYA_APP_Chromecast_Start;
    param.cbk.stop = TUYA_APP_Chromecast_Stop;

    tuya_ipc_chromecast_init(&param);

    s_echoshow_inited = TRUE;

    return OPRT_OK;
}
#endif
/*
---------------------------------------------------------------------------------
EchoShow相关代码结束位置
---------------------------------------------------------------------------------
*/

/*
---------------------------------------------------------------------------------
云储存相关代码起始位置
云存储由内部管理线程获取存储订单的状态、数据的上传等。
当用户开通的是事件存储类型时，通过API tuya_ipc_cloud_storage_event_start tuya_ipc_cloud_storage_event_stop
开始/结束一段存储。
---------------------------------------------------------------------------------
*/

/* 云存储必须对数据进行加密以满足安全要求。
目前涂鸦采用AES CBC模式方式。用软件实现无法满足实时性要求，
需要在这里提供硬件加密的实现。
*/
OPERATE_RET AES_CBC_init(VOID)
{
    //需要在这里添加加密需要的初始化操作
    //如果没有，直接return OPRT_OK
    return OPRT_OK;
}

/* 该临时接口使用CPU进行加密，在IPC SOC上运行延时不能满足要求 */
extern OPERATE_RET OpensslAES_CBC128_encrypt(IN CONST BYTE *pdata_in,  IN CONST UINT data_len,
                                OUT BYTE *pdata_out,  OUT UINT *pdata_out_len,
                                IN CONST BYTE *pkey, IN BYTE *piv);

/* 
pdata_in： 加密源数据，不能更改，否则会影响其他功能
data_len： 加密源数据大小
pdata_out：加密后数据存放地址，已经申请好空间，无需free
pdata_out_len： 加密后数据大小
pkey： 加密密钥key，16字节
piv： 加密向量iv，16字节
*/
OPERATE_RET AES_CBC_encrypt(IN BYTE *pdata_in,  IN UINT data_len,
                                 INOUT BYTE *pdata_out,  OUT UINT *pdata_out_len,
                                 IN BYTE *pkey, IN BYTE *piv)
{
    //注意不能更改pdata_in里的原始数据
    //如果加密接口会更改原始数据，请先copy pdata_in到额外的工作buffer
    return OpensslAES_CBC128_encrypt(pdata_in,data_len,pdata_out,pdata_out_len,pkey,piv);
}

OPERATE_RET AES_CBC_destory(VOID)
{
    //需要在这里添加程序结束时需要的去初始化操作
    //如果没有，直接return OPRT_OK
    return OPRT_OK;
}

OPERATE_RET TUYA_APP_Enable_CloudStorage(VOID)
{
    AES_HW_CBC_FUNC aes_func;
    aes_func.init = AES_CBC_init;
    aes_func.encrypt = AES_CBC_encrypt;
    aes_func.destory = AES_CBC_destory;
    tuya_ipc_cloud_storage_init(&s_media_info, &aes_func);
    return OPRT_OK;
}

/*
---------------------------------------------------------------------------------
云储存相关代码结束位置
---------------------------------------------------------------------------------
*/


OPERATE_RET TUYA_APP_Trigger_Event_Storage(VOID)
{
    PR_DEBUG("Event Triggerd");
    tuya_ipc_ss_trigger_event(20);
}

