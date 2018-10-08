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

#include "tuya_iot_com_api.h"


#define PR_ERR(fmt, ...)    printf("Err:"fmt"\r\n", ##__VA_ARGS__)
#define PR_DEBUG(fmt, ...)  printf("Dbg:"fmt"\r\n", ##__VA_ARGS__)
//#define PR_TRACE(fmt, ...)  printf("Trace:"fmt"\r\n", ##__VA_ARGS__)
#define PR_TRACE(fmt, ...)

STATIC IPC_MGR_INFO_S s_mgr_info = {0};


OPERATE_RET IPC_APP_Sync_Utc_Time(VOID)
{
    TIME_T time_utc;
    INT_T time_zone;
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

STATIC OPERATE_RET __IPC_APP_get_file_data_cb(IN CONST FW_UG_S *fw, IN CONST UINT_T total_len,IN CONST UINT_T offset,\
                             IN CONST BYTE_T *data,IN CONST UINT_T len,OUT UINT_T *remain_len, IN PVOID_T pri_data)
{
    PR_DEBUG("Rev File Data");
    PR_DEBUG("total_len:%d  fw_url:%s", total_len, fw->fw_url);
    PR_DEBUG("Offset:%d Len:%d", offset, len);

    FILE *p_upgrade_fd = (FILE *)pri_data;
    fwrite(data, 1, len, p_upgrade_fd);
    *remain_len = 0;

    return OPRT_OK;
}

STATIC VOID __IPC_APP_upgrade_notify_cb(IN CONST FW_UG_S *fw, IN CONST INT_T download_result, IN PVOID_T pri_data)
{
    FILE *p_upgrade_fd = (FILE *)pri_data;
    fclose(p_upgrade_fd);

    PR_DEBUG("Upgrade Finish");
    PR_DEBUG("download_result:%d fw_url:%s", download_result, fw->fw_url);

    if(download_result == 0)
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
    tuya_iot_upgrade_gw(fw, __IPC_APP_get_file_data_cb, __IPC_APP_upgrade_notify_cb, p_upgrade_fd);
}



STATIC INT_T s_mqtt_status = 0;
INT_T IPC_APP_Get_Mqtt_Status(VOID)
{
    PR_DEBUG("curr mqtt status:%d", s_mqtt_status);
    return s_mqtt_status;
}

STATIC VOID __IPC_APP_Get_Wifi_Status_cb(IN CONST GW_WIFI_NW_STAT_E stat)
{
    PR_DEBUG("wifi status changed:%d", stat);
    switch(stat)
    {
        case STAT_CLOUD_CONN:
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

UINT_T IPC_APP_Get_Max_P2P_User(VOID)
{
    return s_mgr_info.max_p2p_user;
}

OPERATE_RET IPC_APP_Init_SDK(WIFI_INIT_MODE_E init_mode, CHAR_T *p_token)
{
    PR_DEBUG("SDK Version:%s\r\n", tuya_iot_get_sdk_info());

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
    TUYA_APP_Init_Stream_Storage(s_mgr_info.sd_base_path);

    IPC_APP_Notify_LED_Sound_Status_CB(IPC_BOOTUP_FINISH);

    tuya_iot_init(s_mgr_info.storage_path);

    WF_GW_PROD_INFO_S prod_info = {s_mgr_info.uuid, s_mgr_info.auth_key, NULL, NULL};
    tuya_iot_set_wf_gw_prod_info(&prod_info);


    TY_IOT_CBS_S iot_cb = {0};
    iot_cb.gw_status_cb = NULL;
    iot_cb.gw_ug_cb = __IPC_APP_gw_ug_inform_cb;
    iot_cb.gw_reset_cb = IPC_APP_Reboot_System_CB;
    iot_cb.dev_obj_dp_cb = IPC_APP_handle_dp_cmd_objs;
    iot_cb.dev_raw_dp_cb = NULL;
    iot_cb.dev_dp_query_cb = IPC_APP_handle_dp_query_objs;

    tuya_iot_wf_soc_dev_init(GWCM_OLD, WF_START_SMART_ONLY, &iot_cb, s_mgr_info.product_key, s_mgr_info.dev_sw_version);
    tuya_iot_reg_get_wf_nw_stat_cb(__IPC_APP_Get_Wifi_Status_cb);

    if(init_mode == WIFI_INIT_DEBUG)
    {
        tuya_iot_gw_wf_user_cfg(NULL, NULL, p_token);
    }


    TUYA_APP_Enable_EchoShow_Chromecast();

    return OPRT_OK;
}

OPERATE_RET tuya_ipc_reset(VOID)
{
    return tuya_iot_wf_gw_unactive();
}



OPERATE_RET tuya_ipc_get_service_time(OUT TIME_T *time_utc, OUT INT_T *time_zone)
{
    if(NULL == time_utc || NULL == time_zone)
    {
        PR_ERR("invalid param");
        return OPRT_INVALID_PARM;
    }

    *time_utc = uni_time_get_posix();
    uni_get_time_zone_seconds(time_zone);
    PR_DEBUG("ipc get service time %d %d", *time_utc, *time_zone);

    return uni_time_check_time_sync();

}

OPERATE_RET tuya_ipc_set_service_time(IN TIME_T new_time)
{
    return uni_time_set_posix(new_time);
}
