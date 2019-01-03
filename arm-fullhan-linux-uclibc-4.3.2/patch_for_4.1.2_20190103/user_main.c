#include <sys/time.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include "tuya_ipc_dp_utils.h"
#include "tuya_ipc_common_demo.h"
#include "tuya_ipc_system_control_demo.h"
#include "tuya_ipc_media_demo.h"
#include "tuya_ipc_motion_detect_demo.h"

#define IPC_APP_STORAGE_PATH    "/tmp/"   //涂鸦SDK内部配置文件存放路径，要求可读写，掉电不丢失 
#define IPC_APP_UPGRADE_FILE    "/tmp/upgrade.file" //OTA升级时下载的固件存放位置
#define IPC_APP_SD_BASE_PATH    "/tmp/"      // SD卡挂载目录：本地存储根目录
#define IPC_APP_PID             "gpkguYNp7yI4k413"  //PID：涂鸦平台产品ID。此为demo专用，非正式。正式ID在涂鸦开发者平台创建产品生成。
#define IPC_APP_UUID            "tuyaOneUuidForOneDevice" //UUID：每个设备唯一的标识，一机一号。开发、生产时请向涂鸦申请。
#define IPC_APP_AUTHKEY         "tuyaOneAuthkeyForOneUUID" //AUTHKEY: 对应UUID的鉴权码，一机一码，与UUID配对。开发、生产时请向涂鸦申请。
#define IPC_APP_VERSION         "1.2.3"     //显示在涂鸦APP上的固件版本信息

IPC_MGR_INFO_S s_mgr_info = {0};
STATIC INT_T s_mqtt_status = 0;

CHAR_T s_raw_path[128] = {0};

STATIC VOID __IPC_APP_Get_Net_Status_cb(IN CONST BYTE_T stat)
{
    PR_DEBUG("Net status change to:%d", stat);
    switch(stat)
    {
        case STAT_CLOUD_CONN:        //for wifi ipc
        //case GB_STAT_CLOUD_CONN:     //for wired ipc
        {
            IPC_APP_Notify_LED_Sound_Status_CB(IPC_MQTT_ONLINE);
            PR_DEBUG("mqtt is online\r\n");
            s_mqtt_status = 1;
            break;
        }
        default:
        {
            break;
        }
    }
}

#if defined(WIFI_IPC) && (WIFI_IPC==1)
OPERATE_RET IPC_APP_Init_SDK(WIFI_INIT_MODE_E init_mode, CHAR_T *p_token)
#else
OPERATE_RET IPC_APP_Init_SDK(VOID)
#endif
{
    PR_DEBUG("SDK Version:%s\r\n", tuya_ipc_get_sdk_info());

    memset(&s_mgr_info, 0, sizeof(IPC_MGR_INFO_S));
    strcpy(s_mgr_info.storage_path, IPC_APP_STORAGE_PATH);
    strcpy(s_mgr_info.upgrade_file_path, IPC_APP_UPGRADE_FILE);
    strcpy(s_mgr_info.sd_base_path, IPC_APP_SD_BASE_PATH);
    strcpy(s_mgr_info.product_key, IPC_APP_PID);
    strcpy(s_mgr_info.uuid, IPC_APP_UUID);
    strcpy(s_mgr_info.auth_key, IPC_APP_AUTHKEY);
    strcpy(s_mgr_info.dev_sw_version, IPC_APP_VERSION);
    s_mgr_info.max_p2p_user = 4; //涂鸦P2P最大支持4路同时预览，开发者根据硬件资源情况可以考虑缩减
    PR_DEBUG("Init Value.storage_path %s", s_mgr_info.storage_path);
    PR_DEBUG("Init Value.upgrade_file_path %s", s_mgr_info.upgrade_file_path);
    PR_DEBUG("Init Value.sd_base_path %s", s_mgr_info.sd_base_path);
    PR_DEBUG("Init Value.product_key %s", s_mgr_info.product_key);
    PR_DEBUG("Init Value.uuid %s", s_mgr_info.uuid);
    PR_DEBUG("Init Value.auth_key %s", s_mgr_info.auth_key);
    PR_DEBUG("Init Value.p2p_id %s", s_mgr_info.p2p_id);
    PR_DEBUG("Init Value.dev_sw_version %s", s_mgr_info.dev_sw_version);
    PR_DEBUG("Init Value.max_p2p_user %u", s_mgr_info.max_p2p_user);

    IPC_APP_Set_Media_Info();
    TUYA_APP_Init_Ring_Buffer();

    IPC_APP_Notify_LED_Sound_Status_CB(IPC_BOOTUP_FINISH);

    TUYA_IPC_ENV_VAR_S env;

    memset(&env, 0, sizeof(TUYA_IPC_ENV_VAR_S));

    strcpy(env.storage_path, s_mgr_info.storage_path);
    strcpy(env.product_key,s_mgr_info.product_key);
    strcpy(env.uuid, s_mgr_info.uuid);
    strcpy(env.auth_key, s_mgr_info.auth_key);
    strcpy(env.dev_sw_version, s_mgr_info.dev_sw_version);
    strcpy(env.dev_serial_num, "tuya_ipc");

    env.dev_obj_dp_cb = IPC_APP_handle_dp_cmd_objs;
    env.dev_dp_query_cb = IPC_APP_handle_dp_query_objs;
    env.status_changed_cb = __IPC_APP_Get_Net_Status_cb;
    env.gw_ug_cb = IPC_APP_Upgrade_Inform_cb;
    env.gw_rst_cb = IPC_APP_Reset_System_CB;
    env.gw_restart_cb = IPC_APP_Restart_Process_CB;
    env.mem_save_mode = FALSE;
    tuya_ipc_init_sdk(&env);

#if defined(WIFI_IPC) && (WIFI_IPC==1)
    tuya_ipc_start_sdk(init_mode, p_token);
#else
	tuya_ipc_start_sdk(WIFI_INIT_NULL, NULL);
#endif
    return OPRT_OK;
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

#ifdef __HuaweiLite__
int app_main(int argc, char *argv[])
#else
int main(int argc, char *argv[])
#endif

{
    INT_T res = -1;
    CHAR_T token[30] = {0};
    WIFI_INIT_MODE_E mode = WIFI_INIT_AUTO;

    strcpy(s_raw_path, "/tmp"); //demo需要用到的测试音视频文件夹所在目录
#ifdef __HuaweiLite__
    if(argc != 2)
    {
        printf("%s <token>\n", argv[0]);
        return -1;
    }
    mode = WIFI_INIT_DEBUG; //demo的模式设置为debug，所以运行这个main流程前开发者需要先保证设备已经能联网。
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
#if defined(WIFI_IPC) && (WIFI_IPC==1)
    IPC_APP_Init_SDK(mode, token);
#else
    //有线机型SDK内部已经实现了与涂鸦APP的token交互
    IPC_APP_Init_SDK();
#endif

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
    while(s_mqtt_status != 1)
    {
        sleep(1);
    }
    /*至少在联网之后进行一次系统时间同步*/
    IPC_APP_Sync_Utc_Time();
    
    /* 开启本地存储，因为涉及从服务端同步时间，建议放在联网之后 */
    TUYA_APP_Init_Stream_Storage(s_mgr_info.sd_base_path);
    
    /* 开启P2P通道 */
    TUYA_APP_Enable_P2PTransfer(s_mgr_info.max_p2p_user);
    
    /* 当MQTT连接成功后，上传本地所有配置项(DP)状态 */
    IPC_APP_upload_all_status();

    /* 使能云存储功能 */
    TUYA_APP_Enable_CloudStorage();

    /* 使能推流服务 */
    TUYA_APP_Enable_EchoShow_Chromecast();

    /* 开启移动侦测任务，通过侦测结果触发对应的告警上报/本地存储/云存储等任务 */
#ifdef __HuaweiLite__
    stappTask.pfnTaskEntry = (TSK_ENTRY_FUNC)thread_md_proc;
    stappTask.pcName = "motion_detect";
    LOS_TaskCreate((UINT32 *)&taskid, &stappTask);
#else
    pthread_t motion_detect_thread;
    pthread_create(&motion_detect_thread, NULL, thread_md_proc, NULL);
    pthread_detach(motion_detect_thread);
#endif

    while(1)
    {        
        usleep(100*1000);
    }

    return 0;
}

