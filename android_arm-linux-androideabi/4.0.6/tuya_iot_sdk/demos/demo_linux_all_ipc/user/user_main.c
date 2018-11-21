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
STATIC CHAR_T s_raw_path[128] = {0};

static void *sync_time_func(void *arg)
{
    while(1)
    {
        sleep(60);

        OPERATE_RET ret = OPRT_OK;

        /* 调用本API同步时间。如果返回OK，说明时间同步成功 */
        ret = IPC_APP_Sync_Utc_Time();
        if(ret != OPRT_OK)
        {
            continue;
        }
        break;
    }

    pthread_exit(0);
}
static void *motion_detect_func(VOID)
{
    UINT_T curr_time = time(NULL);
    CHAR_T tmp_cmd[128] = {0};

    /* 这里使用文件模拟的方式模拟移动侦测截图 */
    memset(tmp_cmd, 0, 128);
    snprintf(tmp_cmd, 128, "cp -rf %s/rawfiles/tuya_logo.jpg jpg_%u.jpg", s_raw_path, curr_time);
    system(tmp_cmd);
    memset(tmp_cmd, 0, 128);
    snprintf(tmp_cmd, 128, "jpg_%u.jpg", curr_time);

    /* 事件模式录像下，发生事件时，触发SD卡本地录制，该接口非阻塞 */
    TUYA_APP_Trigger_Event_Storage();
    /* 当移动侦测事件发生后，调用该API推送报警图片到APP。请注意这里文件名称需要保证不重名，比较好的实践是使用时间戳命名 */
    IPC_APP_Send_Motion_Alarm(tmp_cmd, NOTIFICATION_CONTENT_JPEG);

    memset(tmp_cmd, 0, 128);
    snprintf(tmp_cmd, 128, "rm -rf jpg_%u.jpg", curr_time);
    system(tmp_cmd);
}

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
    INT_T fps = 30;
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

VOID usage(CHAR_T *app_name)
{
    printf("%s -m mode -t token -r raw path -h\n", (CHAR_T *)basename(app_name));
    printf("\t m: 0-WIFI_INIT_AUTO 1-WIFI_INIT_AP 2-WIFI_INIT_DEBUG, refer to WIFI_INIT_MODE_E\n"
        "\t t: token get form qrcode info\n"
        "\t r: raw source file path\n"
        "\t h: help info\n");

    return;
}

INT_T main(INT_T argc, CHAR_T** argv)
{
    INT_T res = -1;
    CHAR_T token[30] = {0};
    WIFI_INIT_MODE_E mode = WIFI_INIT_AUTO;

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

    /* SDK使用文件模拟的方式模拟外部音视频输入 */
    pthread_t h264_output_thread;
    pthread_create(&h264_output_thread, NULL, thread_live_video, NULL);
    pthread_detach(h264_output_thread);

    pthread_t pcm_output_thread;
    pthread_create(&pcm_output_thread, NULL, thread_live_audio, NULL);
    pthread_detach(pcm_output_thread);

    /* 启动SDK */
    IPC_APP_Init_SDK(mode, token);

    /* 判断SDK是否连接到MQTT */
    while(IPC_APP_Get_Mqtt_Status() != 1)
    {
        sleep(1);
    }
    TUYA_APP_Enable_P2PTransfer( IPC_APP_Get_Max_P2P_User() );
    /* 当MQTT连接成功后，上传本地所有状态 */
    IPC_APP_upload_all_status();

    /* 使能云存储功能 */
    TUYA_APP_Enable_CloudStorage();

    /* 开启线层，同步服务器和本地时间 */
    pthread_t sync_time_thread;
    pthread_create(&sync_time_thread, NULL, sync_time_func, NULL);
    pthread_detach(sync_time_thread);

    /* 手动模拟相关功能 */
    char test_input[64] = {0};
    while(1)
    {
        scanf("%s",test_input);
        /* 对于云存储是事件存储类型的订单，用命令模拟事件的开始 */
        if(0 == strcmp(test_input,"start"))
        {
            printf("simulation: event start\n");
            ClOUD_STORAGE_TYPE_E mode = tuya_ipc_cloud_storage_get_store_mode();
            if(mode != ClOUD_STORAGE_TYPE_EVENT)
            {
                printf("current cloud storage mode %d, not event type",mode);
                continue;
            }
            FILE*fp = fopen("snap.jpg","r+");
            if(NULL == fp)
            {
                printf("fail to open snap.jpg\n");
                continue;
            }
            fseek(fp,0,SEEK_END);
            int snapshot_size = ftell(fp);
            CHAR_T *snapshot_buf = (CHAR_T *)malloc(snapshot_size);
            fseek(fp,0,SEEK_SET);
            fread(snapshot_buf,snapshot_size,1,fp);
            fclose(fp);
            tuya_ipc_cloud_storage_event_start(snapshot_buf,snapshot_size,EVENT_TYPE_MOTION_DETECT);
        }
        /* 对于云存储是事件存储类型的订单，用命令模拟事件的结束 */
        else if(0 == strcmp(test_input,"stop"))
        {
            printf("simulation: event stop\n");
            ClOUD_STORAGE_TYPE_E mode = tuya_ipc_cloud_storage_get_store_mode();
            if(mode != ClOUD_STORAGE_TYPE_EVENT)
            {
                printf("current cloud storage mode %d, not event type",mode);
                continue;
            }
            tuya_ipc_cloud_storage_event_stop();
        }
        /* 用命令模拟移动侦测事件触发本地存储和APP消息上报 */
        else if(0 == strcmp(test_input,"motion"))
        {
            printf("simulation: motion detect\n");
            IPC_APP_set_alarm_function_onoff(1);
            motion_detect_func();
        }
        /* 用命令模拟回复出厂操作 */
        else if(0 == strcmp(test_input,"reset"))
        {
            printf("simulation: reset\n");
            tuya_ipc_reset();

            break;
        }
        usleep(10000);
    }

    return 0;
}

