#include "tuya_ipc_api.h"
#include "tuya_ipc_dp_handler.h"
#include "tuya_ipc_mgr_utils.h"
#include "tuya_ipc_media_utils.h"
#include "tuya_ipc_cloud_storage.h"


#include <sys/time.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <libgen.h>
STATIC CHAR s_raw_path[128] = {0};

/*
 * 示例代码采用文件读写方式来模拟音视频请求，文件在rawfiles.tar.gz中
 */
#define AUDIO_FRAME_SIZE 640
#define AUDIO_FPS 25

#define VIDEO_BUF_SIZE	(1024 * 400)

static int live_clarity = 0;/* 0为标清 */

/*
使用读写文件的方式来模拟音频输出。
*/
void *thread_live_audio(void *arg)
{
    char fullpath[128] = {0};
    sprintf(fullpath, "%s/rawfiles/jupiter_8k_16bit_mono.raw", s_raw_path);

    FILE *aFp = fopen(fullpath, "rb");
    if(aFp == NULL)
    {
        printf("can't read live audio files\n");
        pthread_exit(0);
    }
    char audioBuf[AUDIO_FRAME_SIZE];
    MEDIA_FRAME_S pcm_frame = {0};
    pcm_frame.type = E_AUDIO_FRAME;

    while(1)
    {
        int size = fread(audioBuf, 1, AUDIO_FRAME_SIZE, aFp);
        if(size < AUDIO_FRAME_SIZE)
        {
            rewind(aFp);
            continue;
        }
        pcm_frame.size = size;
        pcm_frame.p_buf = audioBuf;

        TUYA_APP_Put_Frame(E_CHANNEL_AUDIO,&pcm_frame);

        int frameRate = AUDIO_FPS;
        int sleepTick = 1000000/frameRate;
        usleep(sleepTick);
    }

    pthread_exit(0);
}

/* 使用读写文件的方式来模拟直播视频。*/
void *thread_live_video(void *arg)
{
    char raw_fullpath[128] = {0};
    char info_fullpath[128] = {0};

    sprintf(raw_fullpath, "%s/rawfiles/video_multi/beethoven_240.multi/frames.bin", s_raw_path);
    sprintf(info_fullpath, "%s/rawfiles/video_multi/beethoven_240.multi/frames.info", s_raw_path);

    FILE *streamBin_fp = fopen(raw_fullpath, "rb");
    FILE *streamInfo_fp = fopen(info_fullpath, "rb");
    if((streamBin_fp == NULL)||(streamInfo_fp == NULL))
    {
        printf("can't read live video files\n");
        pthread_exit(0);
    }

    char line[128] = {0}, *read = NULL;
    INT fps = 30;
    read = fgets(line, sizeof(line), streamInfo_fp);
    sscanf(line, "FPS %d\n", &fps);

    char videoBuf[VIDEO_BUF_SIZE];

    MEDIA_FRAME_S h264_frame = {0};
    while(1)
    {
        read = fgets(line, sizeof(line), streamInfo_fp);
        if(read == NULL)
        {
            rewind(streamBin_fp);
            rewind(streamInfo_fp);
            read = fgets(line, sizeof(line), streamInfo_fp);

            continue;
        }

        char frame_type[2] = {0};
        int frame_pos = 0, frame_size = 0, nRet = 0;
        sscanf(line, "%c %d %d\n", frame_type, &frame_pos, &frame_size);

        fseek(streamBin_fp, frame_pos*sizeof(char), SEEK_SET);
        nRet = fread(videoBuf, 1, frame_size, streamBin_fp);
        if(nRet < frame_size)
        {
            rewind(streamBin_fp);
            rewind(streamInfo_fp);
            read = fgets(line, sizeof(line), streamInfo_fp);
            continue;
        }

        //注意：部分编码器出I帧时SPS/PPS/SEI/IDR分开上传，需要合并为一个连续帧后传入，且不能删除NALU的分割符
        h264_frame.type = (strcmp(frame_type, "I") == 0 ? E_VIDEO_I_FRAME: E_VIDEO_PB_FRAME);
        h264_frame.p_buf = videoBuf;
        h264_frame.size = nRet;
        h264_frame.pts = 0;

        /* 将高清视频数据送入SDK */
        TUYA_APP_Put_Frame(E_CHANNEL_VIDEO_MAIN, &h264_frame);
        /* 将标清视频数据送入SDK */
        TUYA_APP_Put_Frame(E_CHANNEL_VIDEO_SUB, &h264_frame);

        int frameRate = fps;
        int sleepTick = 1000000/frameRate;
        usleep(sleepTick);
    }

    pthread_exit(0);
}

VOID usage(CHAR *app_name)
{
    printf("%s -m mode -t token -r raw path -h\n", app_name);
    printf("\t m: 0-WIFI_INIT_AUTO 1-WIFI_INIT_AP 2-WIFI_INIT_DEBUG, refer to WIFI_INIT_MODE_E\n"
        "\t t: token get form qrcode info\n"
        "\t r: raw source file path\n"
        "\t h: help info\n");

    return;
}

#ifdef __HuaweiLite__
int app_main(int argc, char *argv[])
#else
int main(int argc, char *argv[])
#endif

{
    INT res = -1;
    CHAR token[30] = {0};
    WIFI_INIT_MODE_E mode = WIFI_INIT_AUTO;

#ifdef __HuaweiLite__
    if(argc != 2)
    {
        printf("%s <token>\n", argv[0]);
        return -1;
    }
    mode = WIFI_INIT_DEBUG; //demo的模式设置为debug，所以运行这个main流程前开发者需要先保证设备已经能联网。
    strcpy(s_raw_path, "/tmp"); //demo需要用到的测试音视频文件夹所在目录
    strcpy(token, argv[1]); //从APP二维码或广播包中扫取的token字段值
#else
    while((res = getopt(argc, argv, "?m:t:s:r:h")) != -1) 
    {
        switch(res) {
        case 'm':
            mode = atoi(optarg);
            break;

        case 't':
            strcpy(token, optarg);
            break;

        case 'r':
            strcpy(s_raw_path, optarg);
            break;

        case 'h':
        default:
            usage(argv[0]);
            return -1;
        }
    }
#endif
    /* 启动SDK */
    IPC_APP_Init_SDK(mode, token);

    /* demo使用文件模拟的方式模拟外部音视频输入，实际数据的获取需要由开发者实现 */
#ifdef __HuaweiLite__
    TSK_INIT_PARAM_S stappTask;
    int taskid = -1;
    memset(&stappTask, 0, sizeof(TSK_INIT_PARAM_S));
    stappTask.pfnTaskEntry = (TSK_ENTRY_FUNC)thread_live_video;
    stappTask.uwStackSize  = 0x80000;
    stappTask.pcName = "live_video";
    stappTask.usTaskPrio = 10;
    stappTask.uwResved   = LOS_TASK_STATUS_DETACHED;
    LOS_TaskCreate((UINT32 *)&taskid, &stappTask);

    stappTask.pfnTaskEntry = (TSK_ENTRY_FUNC)thread_live_audio;
    stappTask.pcName = "live_video";
    LOS_TaskCreate((UINT32 *)&taskid, &stappTask);
 #else   
    pthread_t h264_output_thread;
    pthread_create(&h264_output_thread, NULL, thread_live_video, NULL);
    pthread_detach(h264_output_thread);

    pthread_t pcm_output_thread;
    pthread_create(&pcm_output_thread, NULL, thread_live_audio, NULL);
    pthread_detach(pcm_output_thread);
#endif
    /* 判断SDK是否连接到MQTT */
    while(IPC_APP_Get_Mqtt_Status() != 1)
    {
        sleep(1);
    }
    /*至少在联网之后进行一次系统时间同步*/
    IPC_APP_Sync_Utc_Time();
    
    /* 开启本地存储，因为涉及从服务端同步时间，建议放在联网之后 */
    TUYA_APP_Init_Stream_Storage(IPC_APP_Get_SD_PATH());
    
    /* 开启P2P通道 */
    TUYA_APP_Enable_P2PTransfer(IPC_APP_Get_Max_P2P_User());
    
    /* 当MQTT连接成功后，上传本地所有状态 */
    IPC_APP_upload_all_status();

    /* 使能云存储功能 */
    TUYA_APP_Enable_CloudStorage();

    /* 手动模拟相关功能 */
    char test_input[64] = {0};
    while(1)
    {
        scanf("%s",test_input);
        /* 对于云存储是事件存储类型的订单，用命令模拟事件的开始 */
        if(0 == strcmp(test_input,"start"))
        {
            printf("simulation: event start\n");
            tuya_ipc_ss_start_event(); //本地存储为事件模式时，开始存储
            ClOUD_STORAGE_TYPE_E mode = tuya_ipc_cloud_storage_get_store_mode();
            if(mode != ClOUD_STORAGE_TYPE_EVENT && mode != ClOUD_STORAGE_TYPE_CONTINUE)
            {
                printf("current cloud storage mode %d, not supported",mode);
                continue;
            }
            //这里使用文件模拟，实际可以使用从芯片编码器获取的JPEG 地址和size
            FILE*fp = fopen("snap.jpg","r+");
            if(NULL == fp)
            {
                printf("fail to open snap.jpg\n");
                continue;
            }
            fseek(fp,0,SEEK_END);
            int snapshot_size = ftell(fp);
            CHAR *snapshot_buf = (CHAR *)malloc(snapshot_size);
            fseek(fp,0,SEEK_SET);
            fread(snapshot_buf,snapshot_size,1,fp);
            fclose(fp);
            tuya_ipc_cloud_storage_event_start(snapshot_buf,snapshot_size,EVENT_TYPE_MOTION_DETECT);
            free(snapshot_buf);
        }
        /* 对于云存储是事件存储类型的订单，用命令模拟事件的结束 */
        else if(0 == strcmp(test_input,"stop"))
        {
            printf("simulation: event stop\n");
            tuya_ipc_ss_stop_event(); //结束本地存储
            ClOUD_STORAGE_TYPE_E mode = tuya_ipc_cloud_storage_get_store_mode();
            if(mode != ClOUD_STORAGE_TYPE_EVENT && mode != ClOUD_STORAGE_TYPE_CONTINUE)
            {
                printf("current cloud storage mode %d, not supported",mode);
                continue;
            }
            tuya_ipc_cloud_storage_event_stop();
        }
        /* 用命令模拟移动侦测事件触发本地存储和APP消息上报 */
        else if(0 == strcmp(test_input,"motion"))
        {
            printf("simulation: motion detect\n");
            IPC_APP_set_alarm_function_onoff(1);

            //这里使用文件模拟，实际可以使用从芯片编码器获取的JPEG 地址和size
            char snapfile[64];
            snprintf(snapfile,64,"%s/rawfiles/tuya_logo.jpg",s_raw_path);
            FILE*fp = fopen(snapfile,"r+");
            if(NULL == fp)
            {
                printf("fail to open snap.jpg\n");
                continue;
            }
            fseek(fp,0,SEEK_END);
            int snapshot_size = ftell(fp);
            CHAR *snapshot_buf = (CHAR *)malloc(snapshot_size);
            fseek(fp,0,SEEK_SET);
            fread(snapshot_buf,snapshot_size,1,fp);
            fclose(fp);
            /* 事件模式录像下，发生事件时，触发SD卡本地录制，该接口非阻塞 */
            /* 该接口固定触发20秒的本地存储。如果要自定义开始/结束存储，参考前面start/stop中的tuya_ipc_ss_start_event API */
            TUYA_APP_Trigger_Event_Storage();
            /* 往APP推送侦测消息以及当前快照图片。快照图片的获取需要由开发者实现 */
            IPC_APP_Send_Motion_Alarm_From_Buffer(snapshot_buf,snapshot_size,NOTIFICATION_CONTENT_JPEG);
            free(snapshot_buf);
        }
        /* 用命令模拟回复出厂操作 */
        /* tuya_ipc_reset接口值只负责执行重置注册信息的功能，其他设备端的重置工作需要开发者实现 */
        else if(0 == strcmp(test_input,"reset"))
        {
            printf("simulation: reset\n");
            tuya_ipc_reset();
            break;
        }
        /* 用命令模拟：获得设备的激活状态 */
        else if(0 == strcmp(test_input,"status"))
        {
            IPC_REGISTER_STATUS status = tuya_ipc_get_register_status();
            printf("current register status %d[0:unregistered 1:registered 2:activated]\n",status);
        }
        /* 用命令模拟：门铃上报 */
        else if(0 == strcmp(test_input,"bell"))
        {
            //step 1: 上报门铃呼叫的消息
            tuya_ipc_door_bell_press();
            //step 2: 上传门铃快照图片
            IPC_APP_Send_DoorBell_Snap("snap.jpg", NOTIFICATION_CONTENT_JPEG);
        }
        usleep(100*1000);
    }

    return 0;
}

