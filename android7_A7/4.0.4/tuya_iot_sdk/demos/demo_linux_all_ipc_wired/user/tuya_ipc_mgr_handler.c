/*********************************************************************************
  *Copyright(C),2015-2020, 涂鸦科技 www.tuya.comm
  *FileName: tuya_ipc_mgr_handler.c
  *
  * 文件描述：
  * 1. 固件函数API实现
  *
  * 开发者工作：
  * 1. 设置本地ID。
  * 2. 重启System和重启进程。
  * 3. OTA升级。
  * 4. 声音和LED提示。
  *
**********************************************************************************/

#include "tuya_ipc_mgr_handler.h"
#include <string.h>
#include <stdio.h>


#define IPC_APP_STORAGE_PATH    "../"
#define IPC_APP_UPGRADE_FILE    "../upgrade.file"
#define IPC_APP_SD_BASE_PATH    "../"
#define IPC_APP_PID             "gpkguYNp7yI4k413"
#define IPC_APP_UUID            "tuya2f8e21df98b6379e"
#define IPC_APP_AUTHKEY         "ATIevCnj0dq0mdNtg5cgBKlnRn4N0nf0"
#define IPC_APP_P2PID           "1EK4U947JYELLAZK111A"
#define IPC_APP_VERSION         "1.2.3"

/* 设置本地ID */
VOID IPC_APP_Register_Mgr_Info_CB(INOUT IPC_MGR_INFO_S *p_mgr_info)
{
    memset(p_mgr_info, 0, sizeof(IPC_MGR_INFO_S));

    strcpy(p_mgr_info->storage_path, IPC_APP_STORAGE_PATH);/* 涂鸦SDK内部配置文件存放路径，要求可读写，掉电不丢失 */
    strcpy(p_mgr_info->upgrade_file_path, IPC_APP_UPGRADE_FILE); /* OTA文件下载临时保存位置 */
    strcpy(p_mgr_info->sd_base_path, IPC_APP_SD_BASE_PATH); /* SD卡目录位置 */
    strcpy(p_mgr_info->product_key, IPC_APP_PID);
    strcpy(p_mgr_info->uuid, IPC_APP_UUID);
    strcpy(p_mgr_info->auth_key, IPC_APP_AUTHKEY);
    strcpy(p_mgr_info->p2p_id, IPC_APP_P2PID);
    strcpy(p_mgr_info->dev_sw_version, IPC_APP_VERSION);
    p_mgr_info->max_p2p_user = 5;
}

/* 重启Linux */
VOID IPC_APP_Reboot_System_CB(VOID)
{
    IPC_APP_Notify_LED_Sound_Status_CB(IPC_RESET_SUCCESS);

    printf("reset ipc success. please restart the ipc\n");
    //TODO
    /* 开发者需要实现重启IPC的操作 */

}

/* 重启进程 */
VOID IPC_APP_Restart_Process_CB(VOID)
{
    printf("sdk internal restart request. please restart the ipc\n");

    //TODO
    /* 开发者需要实现重启当前进程的操作。如果无法自持，也可以重启IPC。 */
    //    IPC_APP_Reboot_System_CB();

}

/* OTA */
VOID IPC_APP_Upgrade_Firmware_CB(VOID)
{
    //TODO
    /* 开发者需要实现OTA升级的操作，此时OTA文件已经下载成功。 [ p_mgr_info->upgrade_file_path ]。 */

//    IPC_APP_Reboot_System_CB();
}

/* 声音和LED提示 */
/* TUYA声音和LED提示请参考SDK附带的Excel文件 */
VOID IPC_APP_Notify_LED_Sound_Status_CB(IPC_APP_NOTIFY_EVENT_E notify_event)
{
    printf("curr event:%d \r\n", notify_event);
    switch (notify_event)
    {
        case IPC_BOOTUP_FINISH: /* 启动成功 */
        {
            break;
        }
        case IPC_START_WIFI_CFG: /* 开始配置网络 */
        {
            break;
        }
        case IPC_REV_WIFI_CFG: /* 收到网络配置信息 */
        {
            break;
        }
        case IPC_CONNECTING_WIFI: /* 开始连接WIFI */
        {
            break;
        }
        case IPC_MQTT_ONLINE: /* MQTT上线 */
        {
            break;
        }
        case IPC_RESET_SUCCESS: /* 重置完成 */
        {
            break;
        }
        default:
        {
            break;
        }
    }
}
