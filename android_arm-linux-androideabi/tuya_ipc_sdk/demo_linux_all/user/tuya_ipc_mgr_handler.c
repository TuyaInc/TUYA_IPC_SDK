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


#define IPC_APP_STORAGE_PATH    "../"   //涂鸦SDK内部配置文件存放路径，要求可读写，掉电不丢失 
#define IPC_APP_UPGRADE_FILE    "../upgrade.file" //OTA升级时下载的固件存放位置
#define IPC_APP_SD_BASE_PATH    "../"      // SD卡挂载目录：本地存储根目录
#define IPC_APP_PID             "gpkguYNp7yI4k413"  //PID：涂鸦平台产品ID。此为demo专用，非正式。正式ID在涂鸦开发者平台创建产品生成。
#define IPC_APP_UUID            "tuya2f8e21df98b6379e" //UUID：每个设备唯一的标识，一机一号。开发、生产时请向涂鸦申请。
#define IPC_APP_AUTHKEY         "ATIevCnj0dq0mdNtg5cgBKlnRn4N0nf0" //AUTHKEY: 对应UUID的鉴权码，一机一码，与UUID配对。开发、生产时请向涂鸦申请。
//#define IPC_APP_P2PID           "79HXX5ZYWKYR9MRU111A" 新SDK不需要制定P2P ID，会自动从涂鸦云获取
#define IPC_APP_VERSION         "1.2.3"     //显示在涂鸦APP上的固件版本信息

/* 设置本地ID */
VOID IPC_APP_Register_Mgr_Info_CB(INOUT IPC_MGR_INFO_S *p_mgr_info)
{
    memset(p_mgr_info, 0, sizeof(IPC_MGR_INFO_S));

    strcpy(p_mgr_info->storage_path, IPC_APP_STORAGE_PATH);
    strcpy(p_mgr_info->upgrade_file_path, IPC_APP_UPGRADE_FILE);
    strcpy(p_mgr_info->sd_base_path, IPC_APP_SD_BASE_PATH);
    strcpy(p_mgr_info->product_key, IPC_APP_PID);
    strcpy(p_mgr_info->uuid, IPC_APP_UUID);
    strcpy(p_mgr_info->auth_key, IPC_APP_AUTHKEY);
   // strcpy(p_mgr_info->p2p_id, IPC_APP_P2PID);
    strcpy(p_mgr_info->dev_sw_version, IPC_APP_VERSION);
    p_mgr_info->max_p2p_user = 5;
}

/* 重启Linux */
/* 
参数reboot_type:
0: 本地重置，注销+恢复出厂
1: 远程移除设备，仅注销  
2: 本地仅注销
3: 远程移除并恢复出厂
*/
VOID IPC_APP_Reboot_System_CB(INT reboot_type)
{
    IPC_APP_Notify_LED_Sound_Status_CB(IPC_RESET_SUCCESS);

    printf("reset ipc success. please restart the ipc %d\n", reboot_type);
    //TODO
    /* 开发者需要实现重启IPC的操作/如果type是需要恢复出厂的操作，则需要执行恢复指令 */
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
