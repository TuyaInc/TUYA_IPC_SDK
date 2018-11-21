/*********************************************************************************
  *Copyright(C),2015-2020, 涂鸦科技 www.tuya.comm
  *FileName: tuya_ipc_dp_handler.h
  *
  * 文件描述：
  * 1. DP点设置和获取函数API定义，具体内容请参考.c文件中的代码注释
**********************************************************************************/

#ifndef _TUYA_IPC_DP_HANDLER_H
#define _TUYA_IPC_DP_HANDLER_H

#ifdef __cplusplus
extern "C" {
#endif

#include "tuya_cloud_types.h"
#include "tuya_ipc_dp_utils.h"

#ifdef TUYA_DP_SLEEP_MODE
VOID IPC_APP_set_sleep_mode(BOOL sleep_mode);
BOOL IPC_APP_get_sleep_mode(VOID);
#endif

#ifdef TUYA_DP_LIGHT
VOID IPC_APP_set_light_onoff(BOOL light_on_off);
BOOL IPC_APP_get_light_onoff(VOID);
#endif

#ifdef TUYA_DP_FLIP
VOID IPC_APP_set_flip_onoff(BOOL flip_on_off);
BOOL IPC_APP_get_flip_onoff(VOID);
#endif

#ifdef TUYA_DP_WATERMARK
VOID IPC_APP_set_watermark_onoff(BOOL watermark_on_off);
BOOL IPC_APP_get_watermark_onoff(VOID);
#endif

#ifdef TUYA_DP_WDR
VOID IPC_APP_set_wdr_onoff(BOOL wdr_on_off);
BOOL IPC_APP_get_wdr_onoff(VOID);
#endif

#ifdef TUYA_DP_NIGHT_MODE
VOID IPC_APP_set_night_mode(CHAR *p_night_mode);
CHAR *IPC_APP_get_night_mode(VOID);
#endif

#ifdef TUYA_DP_CALIBRATION
VOID IPC_APP_start_calibration(VOID);
#endif

#ifdef TUYA_DP_ALARM_FUNCTION
VOID IPC_APP_set_alarm_function_onoff(BOOL alarm_on_off);
BOOL IPC_APP_get_alarm_function_onoff(VOID);
#endif

#ifdef TUYA_DP_ALARM_SENSITIVITY
VOID IPC_APP_set_alarm_sensitivity(CHAR *p_sensitivity);
CHAR *IPC_APP_get_alarm_sensitivity(VOID);
#endif

//#ifdef TUYA_DP_ALARM_INTERVAL
//VOID IPC_APP_set_alarm_interval(CHAR *p_interval);
//CHAR *IPC_APP_get_alarm_interval(VOID);
//#endif

#ifdef TUYA_DP_SD_STATUS_ONLY_GET
INT IPC_APP_get_sd_status(VOID);
#endif

#ifdef TUYA_DP_SD_STORAGE_ONLY_GET
VOID IPC_APP_get_sd_storage(UINT *p_total, UINT *p_used, UINT *p_empty);
#endif

#ifdef TUYA_DP_SD_RECORD_ENABLE
VOID IPC_APP_set_sd_record_onoff(BOOL sd_record_on_off);
BOOL IPC_APP_get_sd_record_onoff(VOID);
#endif

#ifdef TUYA_DP_SD_RECORD_MODE
VOID IPC_APP_set_sd_record_mode(CHAR *p_sd_record_mode);
CHAR *IPC_APP_get_sd_record_mode(VOID);
#endif

#ifdef TUYA_DP_SD_UMOUNT
BOOL IPC_APP_unmount_sd_card(VOID);
#endif

#ifdef TUYA_DP_SD_FORMAT
VOID IPC_APP_format_sd_card(VOID);
#endif

#ifdef TUYA_DP_SD_FORMAT_STATUS_ONLY_GET
INT IPC_APP_get_sd_format_status(VOID);
#endif

#ifdef TUYA_DP_PTZ_CONTROL
VOID IPC_APP_ptz_start_move(CHAR *p_direction);
#endif

#ifdef TUYA_DP_PTZ_STOP
VOID IPC_APP_ptz_stop_move(VOID);
#endif

#ifdef TUYA_DP_BLUB_SWITCH
VOID IPC_APP_set_blub_onoff(BOOL blub_on_off);
BOOL IPC_APP_get_blub_onoff(VOID);
#endif

#ifdef TUYA_DP_ELECTRICITY
INT IPC_APP_get_battery_percent(VOID);
#endif

#ifdef TUYA_DP_POWERMODE
CHAR *IPC_APP_get_power_mode(VOID);
#endif

#ifdef __cplusplus
}
#endif

#endif  /*_TUYA_IPC_DP_HANDLER_H*/
