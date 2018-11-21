/*********************************************************************************
  *Copyright(C),2017, 涂鸦科技 www.tuya.comm
  *FileName:    tuya_ipc_mgr_handler.h
  *
  * 文件描述：
  * 1. 固件函数API定义，具体内容请参考.c文件中的代码注释
**********************************************************************************/

#ifndef _TUYA_IPC_MGR_HANDLER_H
#define _TUYA_IPC_MGR_HANDLER_H

#ifdef __cplusplus
extern "C" {
#endif

#include "tuya_ipc_api.h"
#include "tuya_ipc_mgr_utils.h"


VOID IPC_APP_Register_Mgr_Info_CB(INOUT IPC_MGR_INFO_S *p_mgr_info);

VOID IPC_APP_Reboot_System_CB(INT from);

VOID IPC_APP_Restart_Process_CB(VOID);

VOID IPC_APP_Upgrade_Firmware_CB(VOID);

typedef enum
{
    IPC_BOOTUP_FINISH,
    IPC_START_WIFI_CFG,
    IPC_REV_WIFI_CFG,
    IPC_CONNECTING_WIFI,
    IPC_MQTT_ONLINE,
    IPC_RESET_SUCCESS,
}IPC_APP_NOTIFY_EVENT_E;

VOID IPC_APP_Notify_LED_Sound_Status_CB(IPC_APP_NOTIFY_EVENT_E notify_event);



#ifdef __cplusplus
}
#endif

#endif  /*_TUYA_IPC_MGR_HANDLER_H*/
