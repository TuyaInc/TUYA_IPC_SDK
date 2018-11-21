/*********************************************************************************
  *Copyright(C),2015-2020, 涂鸦科技 www.tuya.comm
  *FileName:    tuya_ipc_dp_utils.h
  *
  * 请勿随意修改该文件中的任何内容，如需修改请联系涂鸦产品经理！！
  * 文件描述：
  * 1. DP点定义
  * 2. DP点工具API定义
**********************************************************************************/

#ifndef _TUYA_IPC_DP_UTILS_H
#define _TUYA_IPC_DP_UTILS_H

#ifdef __cplusplus
extern "C" {
#endif

#include "tuya_ipc_api.h"
#include "tuya_cloud_types.h"
#include "cJSON.h"

/* APP基本功能设置页面 */
#define TUYA_DP_SLEEP_MODE                 "105"         /* 休眠,BOOL类型,true休眠,false不休眠 */
#define TUYA_DP_LIGHT                      "101"         /* 状态指示灯,BOOL类型,true打开,false关闭 */
#define TUYA_DP_FLIP                       "103"         /* 录制画面翻转,BOOL类型,true反转,false正常 */
#define TUYA_DP_WATERMARK                  "104"         /* 视频水印,BOOL类型,true打开水印,false关闭水印 */
#define TUYA_DP_WDR                        "107"         /* 宽动态范围模式,BOOL类型,true则打开宽动态范围模式,false则关闭宽动态范围 */
#define TUYA_DP_NIGHT_MODE                 "108"         /* 红外夜视功能,ENUM类型,0-自动 1-关 2-开 */
#define TUYA_DP_CALIBRATION                "132"         /* 摄像机校准,BOOL类型 */
/* APP移动侦测报警页面 */
#define TUYA_DP_ALARM_FUNCTION             "134"         /* 移动侦测报警功能开关,BOOL类型,true打开,false关闭 */
#define TUYA_DP_ALARM_SENSITIVITY          "106"         /* 移动侦测报警灵敏度,ENUM类型,0为低灵敏度,1为中灵敏度,2为高灵敏度*/
//#define TUYA_DP_ALARM_INTERVAL             "133"         /* 移动侦测报警间歇,单位为分钟,ENUM类型,"1","5","10","30","60" */
/* APP储存卡设置页面 */
#define TUYA_DP_SD_STATUS_ONLY_GET         "110"         /* SD卡状态,VALUE类型,1-正常,2-异常,3-空间不足,4-正在格式化,5-无SD卡 */
#define TUYA_DP_SD_STORAGE_ONLY_GET        "109"         /* SD卡容量,STR类型,"总容量|当前使用量|剩余容量",单位：kb */
#define TUYA_DP_SD_RECORD_ENABLE           "150"         /* SD卡录像功能使能,BOOL类型,true打开,false关闭 */
#define TUYA_DP_SD_RECORD_MODE             "151"         /* SD卡录像模式选择,ENUM类型,1-事件录像 2-连续录像 */
#define TUYA_DP_SD_UMOUNT                  "112"         /* 退出存储卡,BOOL类型,true为存储卡已退出,false为存储卡未退出 */
#define TUYA_DP_SD_FORMAT                  "111"         /* 格式化存储卡,BOOL类型 */
#define TUYA_DP_SD_FORMAT_STATUS_ONLY_GET  "117"         /* 格式化存储卡状态,VALUE类型,-2000:SD卡正在格式化,-2001:SD卡格式化异常,-2002:无SD卡,-2003:SD卡错误.正数为格式化进度 */
/* APP摄像头控制页面 */
#define TUYA_DP_PTZ_CONTROL                "119"         /* 云台转动开始,ENUM类型,0-上,1-右上,2-右,3-右下,4-下,5-左下,6-左,7-左上 */
#define TUYA_DP_PTZ_STOP                   "116"         /* 云台转动停止,BOOL类型 */

/* IPC特殊功能 */
#define TUYA_DP_DOOR_BELL                  "136"         /* 门铃呼叫,STR类型,"当前时间戳" */
#define TUYA_DP_BLUB_SWITCH                "138"         /* 特殊灯光控制开关,BOOL类型,true打开,false关闭 */
#define TUYA_DP_BLUB_LIGHTNESS             "158"         /* 特殊灯光亮度控制,VALUE类型,[1-100] */
#define TUYA_DP_SOUND_DETECT               "139"         /* 分贝检测功能开关,BOOL类型,true打开,false关闭 */
#define TUYA_DP_SOUND_SENSITIVITY          "140"         /* 分贝检测灵敏度,ENUM类型,0代表低灵敏度,1代表高灵敏度 */
#define TUYA_DP_SOUND_ALARM                "141"         /* 分贝报警通道,STR类型,"当前时间戳" */
#define TUYA_DP_TEMPERATURE                "142"         /* 温度检测,VALUE类型,[0-50] */
#define TUYA_DP_HUMIDITY                   "143"         /* 湿度检测,VALUE类型,[0-100] */
#define TUYA_DP_ELECTRICITY                "145"         /* 电池电量(百分比),VALUE类型,[0-100] */
#define TUYA_DP_POWERMODE                  "146"         /* 供电方式,ENUM类型,"0"为电池供电状态,"1"为插电供电状态(或电池充电状态) */
#define TUYA_DP_LOWELECTRIC                "147"         /* 低电告警阈值(百分比),VALUE类型 */

#define TUYA_DP_MOTION_DETECTION_ALARM     "115"         /* 移动侦测消息报警 */
#define TUYA_DP_DOOR_BELL_SNAP             "154"         /* 门铃推送截图提示 */


/* 上报本地所有DP点最新状态 */
VOID IPC_APP_upload_all_status(VOID);

#ifdef TUYA_DP_DOOR_BELL
/* 门铃产品形态下，当按下门铃后推送通知到APP */
VOID IPC_APP_trigger_door_bell(VOID);
#endif

#ifdef TUYA_DP_SD_FORMAT_STATUS_ONLY_GET
/* SD卡格式化进度上报 */
VOID IPC_APP_report_sd_format_status(INT status);
#endif

#ifdef TUYA_DP_SD_STATUS_ONLY_GET
/* 当SD卡发生变化时(插拔)，调用此API通知APP */
VOID IPC_APP_report_sd_status_changed(VOID);
#endif

#ifdef TUYA_DP_POWERMODE
/* 电源产品形态下，电池状态发生变动时，调用此API通知APP */
VOID IPC_APP_update_battery_status(VOID);
#endif

#ifdef TUYA_DP_MOTION_DETECTION_ALARM
/* 当发生移动侦测事件时，调用该API推送报警图片到APP */
OPERATE_RET IPC_APP_Send_Motion_Alarm_From_Buffer(CHAR *data, UINT size, NOTIFICATION_CONTENT_TYPE_E type);
OPERATE_RET IPC_APP_Send_Motion_Alarm(CHAR *p_abs_file, NOTIFICATION_CONTENT_TYPE_E file_type);
#endif

#ifdef TUYA_DP_DOOR_BELL_SNAP
/* 门铃产品形态下，当按下门铃后，门铃抓拍一张图片，调用该API推送图片到APP */
OPERATE_RET IPC_APP_Send_DoorBell_Snap(CHAR *p_snap_file, NOTIFICATION_CONTENT_TYPE_E file_type);
#endif



/* 工具API */
VOID IPC_APP_handle_dp_cmd_jsons(IN cJSON *p_dp_jsons);
/* 工具API */
VOID IPC_APP_handle_dp_query_jsons(IN cJSON *p_dp_jsons);


#ifdef __cplusplus
}
#endif

#endif  /*_TUYA_IPC_DP_UTILS_H*/
