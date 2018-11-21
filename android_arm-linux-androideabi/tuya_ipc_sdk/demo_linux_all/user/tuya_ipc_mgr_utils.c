/*********************************************************************************
  *Copyright(C),2015-2020, 涂鸦科技 www.tuya.comm
  *FileName: tuya_ipc_mgr_utils.c
  *
  * 文件描述：
  * 1. 固件工具API实现
  *
  * 本文件代码为工具代码，用户可以不用关心
  * 请勿随意修改该文件中的任何内容，如需修改请联系涂鸦产品经理！！
  *
**********************************************************************************/

#include <sys/time.h>
#include <pthread.h>
#include <string.h>
#include <stdio.h>

#include "tuya_ipc_stream_storage.h"
#include "tuya_ring_buffer.h"
#include "tuya_ipc_echo_show.h"

#include "tuya_ipc_mgr_utils.h"
#include "tuya_ipc_mgr_handler.h"
#include "tuya_ipc_media_handler.h"
#include "tuya_ipc_media_utils.h"
#include "tuya_ipc_dp_utils.h"
#include "tuya_linux_wifi.h"


#define PR_ERR(fmt, ...)    printf("Err:"fmt"\r\n", ##__VA_ARGS__)
#define PR_DEBUG(fmt, ...)  printf("Dbg:"fmt"\r\n", ##__VA_ARGS__)
//#define PR_TRACE(fmt, ...)  printf("Trace:"fmt"\r\n", ##__VA_ARGS__)
#define PR_TRACE(fmt, ...)

STATIC IPC_MGR_INFO_S s_mgr_info = {0};


OPERATE_RET IPC_APP_Sync_Utc_Time(VOID)
{
    TIME_T time_utc;
    INT time_zone;
    OPERATE_RET ret = tuya_ipc_get_service_time(&time_utc, &time_zone);

    if(ret != OPRT_OK)
    {
        return ret;
    }
    //API返回OK，说明成功获取到了UTC时间，如果返回不是OK，说明还没有取到时间。

    PR_DEBUG("Get Server Time Success: %lu %d", time_utc, time_zone);

    struct timeval tv;
    tv.tv_sec = time_utc;
    tv.tv_usec = 0;
    struct timezone tz;
    tz.tz_minuteswest = time_zone/60;
    tz.tz_dsttime = 0;

//    settimeofday(&tv, &tz);

    return OPRT_OK;
}



//==================================================
/* 以下代码为工具代码，用户可以不用关心 */
/* 请勿随意修改，如需修改请联系涂鸦FAE！！ */
//==================================================

STATIC OPERATE_RET __IPC_APP_get_file_data_cb(IN CONST CHAR *fw_url, IN CONST UINT total_len,IN CONST UINT offset,\
                             IN CONST BYTE *data,IN CONST UINT len,OUT UINT *remain_len, IN PVOID pri_data)
{
    PR_DEBUG("Rev File Data");
    PR_DEBUG("total_len:%d  fw_url:%s", total_len, fw_url);
    PR_DEBUG("Offset:%d Len:%d", offset, len);

    FILE *p_upgrade_fd = (FILE *)pri_data;
    fwrite(data, 1, len, p_upgrade_fd);
    *remain_len = 0;

    return OPRT_OK;
}

STATIC VOID __IPC_APP_upgrade_notify_cb(IN CONST CHAR *fw_url, IN CONST UPGRADE_PROGRESS_E progress, IN PVOID pri_data)
{
    FILE *p_upgrade_fd = (FILE *)pri_data;
    fclose(p_upgrade_fd);

    PR_DEBUG("Upgrade Finish");
    PR_DEBUG("progress:%d fw_url:%s", progress, fw_url);

    if(progress == UPGRADE_FINISH)
    {
        IPC_APP_Upgrade_Firmware_CB();
    }
}

STATIC VOID __IPC_APP_gw_ug_inform_cb(IN CONST FW_UG_S *fw)
{
    PR_DEBUG("Rev Upgrade Info");
    PR_DEBUG("fw->fw_url:%s", fw->fw_url);
    PR_DEBUG("fw->fw_md5:%s", fw->fw_md5);
    PR_DEBUG("fw->sw_ver:%s", fw->sw_ver);
    PR_DEBUG("fw->file_size:%u", fw->file_size);

    FILE *p_upgrade_fd = fopen(s_mgr_info.upgrade_file_path, "w+b");
    tuya_ipc_upgrade_sdk(fw, __IPC_APP_get_file_data_cb, __IPC_APP_upgrade_notify_cb, p_upgrade_fd);
}



STATIC INT s_mqtt_status = 0;
INT IPC_APP_Get_Mqtt_Status(VOID)
{
    PR_DEBUG("curr mqtt status:%d", s_mqtt_status);
    return s_mqtt_status;
}

STATIC VOID __IPC_APP_mqtt_status_change_cb(IN CONST GW_STATUS_E status, IN CONST PVOID p_reserved)
{
    PR_DEBUG("ipc status changed:%d", status);
    switch(status)
    {
        case GW_MQTT_ONLINE:
        {
            IPC_APP_Notify_LED_Sound_Status_CB(IPC_MQTT_ONLINE);
            PR_DEBUG("mqtt is online\r\n");
            s_mqtt_status = 1;
            break;
        }
        case GW_MQTT_OFFLINE:
        {
            s_mqtt_status = 0;
            PR_DEBUG("mqtt is offline\r\n");
            break;
        }
    }
}


/* 收到消息回调,cmd_type为消息类型,无需关心,param为cJSON指针,包含实际消息 */
STATIC VOID __IPC_APP_cmd_dispatch(IN INT cmd_type, IN VOID *cmd)
{
#if 1
    CHAR *tmp = NULL;
    tmp = cJSON_PrintUnformatted((cJSON *)cmd);
    if (NULL == tmp)
    {
        PR_DEBUG("tuya_cmd_dispatch Error");
        return;
    }
    PR_DEBUG("tuya_cmd_dispatch InCmd:%s\n", tmp);
    cJSON_FreeBuffer(tmp);
#endif

    IPC_APP_handle_dp_cmd_jsons((cJSON *)cmd);

    return;
}

STATIC VOID __IPC_APP_query_dispatch(IN INT cmd_type, IN VOID *cmd)
{
#if 1
    CHAR *tmp = NULL;
    tmp = cJSON_PrintUnformatted((cJSON *)cmd);
    if (NULL == tmp)
    {
        PR_DEBUG("tuya_query_dispatch Error");
        return;
    }
    PR_DEBUG("tuya_query_dispatch InCmd:%s\n", tmp);
    cJSON_FreeBuffer(tmp);
#endif

    IPC_APP_handle_dp_query_jsons((cJSON *)cmd);

    return;
}

CHAR *IPC_APP_Get_SD_PATH(VOID)
{
    return s_mgr_info.sd_base_path;
}


UINT IPC_APP_Get_Max_P2P_User(VOID)
{
    return s_mgr_info.max_p2p_user;
}

OPERATE_RET IPC_APP_Init_SDK(WIFI_INIT_MODE_E init_mode, CHAR *p_token)
{
    PR_DEBUG("SDK Version:%s\r\n", tuya_ipc_get_sdk_info());

    IPC_APP_Register_Mgr_Info_CB(&s_mgr_info);
    PR_DEBUG("Init Value.storage_path %s", s_mgr_info.storage_path);
    PR_DEBUG("Init Value.upgrade_file_path %s", s_mgr_info.upgrade_file_path);
    PR_DEBUG("Init Value.sd_base_path %s", s_mgr_info.sd_base_path);
    PR_DEBUG("Init Value.product_key %s", s_mgr_info.product_key);
    PR_DEBUG("Init Value.uuid %s", s_mgr_info.uuid);
    PR_DEBUG("Init Value.auth_key %s", s_mgr_info.auth_key);
    PR_DEBUG("Init Value.p2p_id %s", s_mgr_info.p2p_id);
    PR_DEBUG("Init Value.dev_sw_version %s", s_mgr_info.dev_sw_version);
    PR_DEBUG("Init Value.max_p2p_user %u", s_mgr_info.max_p2p_user);

    IPC_APP_Refresh_Media_Info();
    TUYA_APP_Init_Ring_Buffer();

    IPC_APP_Notify_LED_Sound_Status_CB(IPC_BOOTUP_FINISH);
    TUYA_IPC_ENV_VAR_S env = {0};
    strcpy(env.storage_path, s_mgr_info.storage_path);
    strcpy(env.product_key,  s_mgr_info.product_key);
    strcpy(env.uuid, s_mgr_info.uuid);
    strcpy(env.auth_key, s_mgr_info.auth_key);
    strcpy(env.p2p_id, s_mgr_info.p2p_id);  //用户没有p2p_id可直接赋空
    strcpy(env.dev_sw_version,  s_mgr_info.dev_sw_version);
    strcpy(env.dev_serial_num,  "ipc_wifi_p2p");
    env.cmd_dispatch = __IPC_APP_cmd_dispatch;
    env.query_dispatch = __IPC_APP_query_dispatch;

    env.status_changed_cb = __IPC_APP_mqtt_status_change_cb;
    env.gw_ug_cb = __IPC_APP_gw_ug_inform_cb;
    env.gw_rst_cb = IPC_APP_Reboot_System_CB;
    env.gw_restart_cb = IPC_APP_Restart_Process_CB;
    env.mem_save_mode = FALSE;

    TUYA_WIFI_DEV_INFO_S dev_info = {0};
    WF_INTF_REG_S reg = {0};
    reg.wall_aps_cb = tuya_linux_wf_all_ap_scan;
    reg.wass_aps_cb = tuya_linux_wf_assign_ap_scan;
    reg.wrelease_ap_cb = tuya_linux_wf_release_ap;
    reg.wscc_cb = tuya_linux_wf_set_cur_channel;
    reg.wgcc_cb = tuya_linux_wf_get_cur_channel;
    reg.wss_cb = tuya_linux_wf_sniffer_set;
    reg.wgi_cb = tuya_linux_wf_get_ip;
    reg.wsi_cb = tuya_linux_wf_set_ip;
    reg.wgm_cb = tuya_linux_wf_get_mac;
    reg.wsm_cb = tuya_linux_wf_set_mac;
    reg.wwms_cb = tuya_linux_wf_wk_mode_set;
    reg.wwmg_cb = tuya_linux_wf_wk_mode_get;
    reg.wsc_cb = tuya_linux_wf_station_connect;
    reg.wsd_cb = tuya_linux_wf_station_disconnect;
    reg.wsgcar_cb = tuya_linux_wf_station_get_conn_ap_rssi;
    reg.wssg_cb = tuya_linux_wf_station_stat_get;
    reg.wa_start_cb = tuya_linux_wf_ap_start;
    reg.wa_stop_cb = tuya_linux_wf_ap_stop;
    dev_info.wf_infos = reg;

    dev_info.wifi_init_mode = init_mode;

    if(init_mode == WIFI_INIT_DEBUG)
    {
        strncpy(dev_info.wifi_init_cfg.debug_cfg.token, p_token, WIFI_TOKEN_LEN);
    }

    tuya_ipc_init_sdk(&env);
    tuya_ipc_start_sdk(&dev_info);

#if ENABLE_ECHO_SHOW == 1
    TUYA_APP_Enable_EchoShow_Chromecast();
#endif
    return OPRT_OK;
}

