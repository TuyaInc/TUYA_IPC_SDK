/*********************************************************************************
  *Copyright(C),2015-2020, 涂鸦科技 www.tuya.comm
  *FileName: tuya_ipc_dp_handler.c
  *
  * 文件描述：
  * 1. DP点设置和获取函数API实现
  *
  * 开发者工作：
  * 1. 本地配置的获取与更新。
  * 2. 设置本地IPC属性，比如画面翻转，时间水印等。开发者可以根据IPC的硬件特性选择实现；
  *    如果有特性不支持，函数留空即可。
  *
**********************************************************************************/
#include "tuya_ipc_dp_utils.h"
#include "tuya_ipc_dp_handler.h"
#include "tuya_ipc_stream_storage.h"
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>

/* 设置和获取本地配置，INT类型。这里参考代码直接写到tmp了，需要开发者自己替换成本地存储。 */
STATIC VOID __tuya_app_write_INT(CHAR_T *key, INT_T value)
{
    //TODO

    CHAR_T tmp_cmd[128] = {0};
    snprintf(tmp_cmd, 128, "echo %d > /tmp/tuya.cfgs/%s", value, key);
    printf("write int exc: %s \r\n", tmp_cmd);
    system(tmp_cmd);
}

STATIC INT_T __tuya_app_read_INT(CHAR_T *key)
{
    //TODO

    CHAR_T tmp_file[64] = {0};
    snprintf(tmp_file, 64, "cat /tmp/tuya.cfgs/%s", key);
    printf("read int exc: %s \r\n", tmp_file);
    FILE *p_file = popen(tmp_file, "r");
    if(p_file == NULL)
    {
        printf("open fail.\r\n");
        return 0;
    }
    memset(tmp_file, 0, 64);
    fgets(tmp_file, 64, p_file);
    INT_T value = atoi(tmp_file);
    printf("readstr:%s andint:%d \r\n", tmp_file, value);

    pclose(p_file);

    return value;
}

/* 设置和获取本地配置，STR类型。这里参考代码直接写到tmp了，需要开发者自己替换成本地存储。 */
STATIC VOID __tuya_app_write_STR(CHAR_T *key, CHAR_T *value)
{
    //TODO

    CHAR_T tmp_cmd[256] = {0};
    snprintf(tmp_cmd, 256, "echo %s > /tmp/tuya.cfgs/%s", value, key);
    printf("write STR exc: %s \r\n", tmp_cmd);
    system(tmp_cmd);
}

STATIC INT_T __tuya_app_read_STR(CHAR_T *key, CHAR_T *value, INT_T value_size)
{
    //TODO

    memset(value, 0, value_size);

    CHAR_T tmp_file[64] = {0};
    snprintf(tmp_file, 64, "cat /tmp/tuya.cfgs/%s", key);
    printf("read str exc: %s \r\n", tmp_file);
    FILE *p_file = popen(tmp_file, "r");
    if(p_file == NULL)
    {
        printf("open fail.\r\n");
        return 0;
    }

    fgets(value, 256, p_file);
    value[strlen(value)] = 0;
    printf("readstr:%s \r\n", value);
    pclose(p_file);
    return 0;
}


#ifdef TUYA_DP_SLEEP_MODE
VOID IPC_APP_set_sleep_mode(BOOL_T sleep_mode)
{
    printf("set sleep_mode:%d \r\n", sleep_mode);
    //TODO
    /* 休眠,BOOL类型,true休眠,false不休眠 */

    __tuya_app_write_INT("tuya_sleep_mode", sleep_mode);
}

BOOL_T IPC_APP_get_sleep_mode(VOID)
{
    BOOL_T sleep_mode = __tuya_app_read_INT("tuya_sleep_mode");
    printf("curr sleep_mode:%d \r\n", sleep_mode);
    return sleep_mode;
}
#endif

//------------------------------------------

#ifdef TUYA_DP_LIGHT
VOID IPC_APP_set_light_onoff(BOOL_T light_on_off)
{
    printf("set light_on_off:%d \r\n", light_on_off);
    //TODO
    /* 状态指示灯,BOOL类型,true打开,false关闭 */

    __tuya_app_write_INT("tuya_light_onoff", light_on_off);
}

BOOL_T IPC_APP_get_light_onoff(VOID)
{
    BOOL_T light_on_off = __tuya_app_read_INT("tuya_light_onoff");
    printf("curr light_on_off:%d \r\n", light_on_off);
    return light_on_off;
}
#endif

//------------------------------------------

#ifdef TUYA_DP_FLIP
VOID IPC_APP_set_flip_onoff(BOOL_T flip_on_off)
{
    printf("set flip_on_off:%d \r\n", flip_on_off);
    //TODO
    /* 录制画面翻转,BOOL类型,true反转,false正常 */

    __tuya_app_write_INT("tuya_flip_onoff", flip_on_off);
}

BOOL_T IPC_APP_get_flip_onoff(VOID)
{
    BOOL_T flip_on_off = __tuya_app_read_INT("tuya_flip_onoff");
    printf("curr flip_on_off:%d \r\n", flip_on_off);
    return flip_on_off;
}
#endif

//------------------------------------------

#ifdef TUYA_DP_WATERMARK
VOID IPC_APP_set_watermark_onoff(BOOL_T watermark_on_off)
{
    printf("set watermark_on_off:%d \r\n", watermark_on_off);
    //TODO
    /* 视频水印,BOOL类型,true打开水印,false关闭水印 */

    __tuya_app_write_INT("tuya_watermark_onoff", watermark_on_off);
}

BOOL_T IPC_APP_get_watermark_onoff(VOID)
{
    BOOL_T watermark_on_off = __tuya_app_read_INT("tuya_watermark_onoff");
    printf("curr watermark_on_off:%d \r\n", watermark_on_off);
    return watermark_on_off;
}
#endif

//------------------------------------------

#ifdef TUYA_DP_WDR
VOID IPC_APP_set_wdr_onoff(BOOL_T wdr_on_off)
{
    printf("set wdr_on_off:%d \r\n", wdr_on_off);
    //TODO
    /* 宽动态范围模式,BOOL类型,true则打开宽动态范围模式,false则关闭宽动态范围 */

    __tuya_app_write_INT("tuya_wdr_onoff", wdr_on_off);

}

BOOL_T IPC_APP_get_wdr_onoff(VOID)
{
    BOOL_T wdr_on_off = __tuya_app_read_INT("tuya_wdr_onoff");
    printf("curr watermark_on_off:%d \r\n", wdr_on_off);
    return wdr_on_off;
}
#endif

//------------------------------------------

#ifdef TUYA_DP_NIGHT_MODE
STATIC CHAR_T s_night_mode[4] = {0};//for demo
VOID IPC_APP_set_night_mode(CHAR_T *p_night_mode)
{//0-自动 1-关 2-开
    printf("set night_mode:%s \r\n", p_night_mode);
    //TODO
    /* 红外夜视功能,ENUM类型,0-自动 1-关 2-开 */

    __tuya_app_write_STR("tuya_night_mode", p_night_mode);
}

CHAR_T *IPC_APP_get_night_mode(VOID)
{
    __tuya_app_read_STR("tuya_night_mode", s_night_mode, 4);
    printf("curr watermark_on_off:%s \r\n", s_night_mode );
    return  s_night_mode;
}
#endif

//------------------------------------------

#ifdef TUYA_DP_CALIBRATION
VOID IPC_APP_start_calibration(VOID)
{
    printf("start calibration\r\n");
    //非阻塞执行
    //TODO
    /* 摄像机校准 */
}
#endif

//------------------------------------------

#ifdef TUYA_DP_ALARM_FUNCTION
VOID IPC_APP_set_alarm_function_onoff(BOOL_T alarm_on_off)
{
    printf("set alarm_on_off:%d \r\n", alarm_on_off);
    /* 移动侦测报警功能开关,BOOL类型,true打开,false关闭。
     * 这里SDK已经将该功能整合了，开发者只要做好本地配置的设置和属性即可。 */

    __tuya_app_write_INT("tuya_alarm_on_off", alarm_on_off);
}

BOOL_T IPC_APP_get_alarm_function_onoff(VOID)
{
    BOOL_T alarm_on_off = __tuya_app_read_INT("tuya_alarm_on_off");
    printf("curr alarm_on_off:%d \r\n", alarm_on_off);
    return alarm_on_off;
}
#endif

//------------------------------------------

#ifdef TUYA_DP_ALARM_SENSITIVITY
STATIC CHAR_T s_alarm_sensitivity[4] = {0};//for demo
VOID IPC_APP_set_alarm_sensitivity(CHAR_T *p_sensitivity)
{
    printf("set alarm_sensitivity:%s \r\n", p_sensitivity);
    //TODO
    /* 移动侦测报警灵敏度,ENUM类型,0为低灵敏度,1为中灵敏度,2为高灵敏度*/

    __tuya_app_write_STR("tuya_alarm_sen", p_sensitivity);
}

CHAR_T *IPC_APP_get_alarm_sensitivity(VOID)
{
    __tuya_app_read_STR("tuya_alarm_sen", s_alarm_sensitivity, 4);
    printf("curr alarm_sensitivity:%s \r\n", s_alarm_sensitivity);
    return s_alarm_sensitivity;
}
#endif

//------------------------------------------

//#ifdef TUYA_DP_ALARM_INTERVAL
//STATIC CHAR_T s_alarm_interval[4] = {0};//for demo
//VOID IPC_APP_set_alarm_interval(CHAR_T *p_interval)
//{
//    printf("set alarm_interval:%s \r\n", p_interval);
//    //TODO
//    /* 移动侦测报警间歇,单位为分钟,ENUM类型,"1","5","10","30","60" */

//    __tuya_app_write_STR("tuya_alarm_interval", p_interval);
//}

//CHAR_T *IPC_APP_get_alarm_interval(VOID)
//{
//    //移动侦测报警间歇,单位为分钟,ENUM类型,"1","5","10","30","60"
//    __tuya_app_read_STR("tuya_alarm_interval", s_alarm_interval, 4);
//    printf("curr alarm_intervaly:%s \r\n", s_alarm_interval);
//    return s_alarm_interval;
//}
//#endif

//------------------------------------------

#ifdef TUYA_DP_SD_STATUS_ONLY_GET
INT_T IPC_APP_get_sd_status(VOID)
{
    INT_T sd_status = 1;
    /* SD卡状态,VALUE类型,1-正常,2-异常,3-空间不足,4-正在格式化,5-无SD卡 */
    /* 开发者需要返回本地SD卡状态 */
    //TODO
    sd_status = 1;

    printf("curr sd_status:%d \r\n", sd_status);
    return sd_status;
}
#endif

//------------------------------------------

#ifdef TUYA_DP_SD_STORAGE_ONLY_GET
VOID IPC_APP_get_sd_storage(UINT_T *p_total, UINT_T *p_used, UINT_T *p_empty)
{//单位：kb
    //TODO
    /* 开发者需要返回本地SD卡容量 */
    *p_total = 128 * 1000 * 1000;
    *p_used = 32 * 1000 * 1000;
    *p_empty = *p_total - *p_used;

    printf("curr sd total:%u used:%u empty:%u \r\n", *p_total, *p_used, *p_empty);
}
#endif

//------------------------------------------

#ifdef TUYA_DP_SD_RECORD_ENABLE
VOID IPC_APP_set_sd_record_onoff(BOOL_T sd_record_on_off)
{
    printf("set sd_record_on_off:%d \r\n", sd_record_on_off);
    /* SD卡录像功能使能,BOOL类型,true打开,false关闭。
     * 这里SDK已经将该功能整合了，开发者只要做好本地配置的设置和属性即可。 */

//    if(sd_record_on_off == TRUE)
//    {
//         IPC_APP_set_sd_record_mode( IPC_APP_get_sd_record_mode()  );
//    }else
//    {
//        tuya_ipc_ss_set_write_mode(SS_WRITE_MODE_NONE);
//    }

    __tuya_app_write_INT("tuya_sd_record_on_off", sd_record_on_off);
}

BOOL_T IPC_APP_get_sd_record_onoff(VOID)
{
    BOOL_T sd_record_on_off  = __tuya_app_read_INT("tuya_sd_record_on_off");
    printf("curr sd_record_on_off:%d \r\n", sd_record_on_off);
    return sd_record_on_off;
}
#endif

//------------------------------------------

#ifdef TUYA_DP_SD_RECORD_MODE
STATIC CHAR_T s_sd_record_mode[4] = {0};//for demo
VOID IPC_APP_set_sd_record_mode(CHAR_T *p_sd_record_mode)
{
    printf("set sd_record_mode:%s \r\n", p_sd_record_mode);
    /* SD卡录像模式选择,ENUM类型,1-事件录像 2-连续录像。
     * 这里SDK已经将该功能整合了，开发者只要做好本地配置的设置和属性即可。 */

    if(strcmp(p_sd_record_mode, "1") == 0)
    {
         tuya_ipc_ss_set_write_mode(SS_WRITE_MODE_EVENT);
    }else if(strcmp(p_sd_record_mode, "2") == 0)
    {
         tuya_ipc_ss_set_write_mode(SS_WRITE_MODE_ALL);
    }else
    {
        printf("Error");
        tuya_ipc_ss_set_write_mode(SS_WRITE_MODE_ALL);
    }
    __tuya_app_write_STR("tuya_sd_record_mode", p_sd_record_mode);
}

CHAR_T *IPC_APP_get_sd_record_mode(VOID)
{
    __tuya_app_read_STR("tuya_sd_record_mode", s_sd_record_mode, 4);
    printf("curr sd_record_mode:%s \r\n", s_sd_record_mode);
    return s_sd_record_mode;
}
#endif

//------------------------------------------

#ifdef TUYA_DP_SD_UMOUNT
BOOL_T IPC_APP_unmount_sd_card(VOID)
{
    BOOL_T umount_ok = TRUE;

    //TODO
    /* 退出存储卡 */

    printf("unmount result:%d \r\n", umount_ok);
    return umount_ok;
}
#endif

//------------------------------------------

#ifdef TUYA_DP_SD_FORMAT
/* -2000:SD卡正在格式化,-2001:SD卡格式化异常,-2002:无SD卡,-2003:SD卡错误.正数为格式化进度 */
STATIC INT_T s_sd_format_progress = 0;
void *thread_sd_format(void *arg)
{
    /* 先通知APP，进度0% */
    s_sd_format_progress = 0;
    IPC_APP_report_sd_format_status(s_sd_format_progress);
    sleep(1);

    /* 停止本地SD卡录制和回访，进度10% */
    s_sd_format_progress = 10;
    IPC_APP_report_sd_format_status(s_sd_format_progress);
    tuya_ipc_ss_set_write_mode(SS_WRITE_MODE_NONE);
    tuya_ipc_ss_pb_stop_all();
    sleep(1);

    /* 删除SD卡内的媒体文件，进度30% */
    s_sd_format_progress = 30;
    IPC_APP_report_sd_format_status(s_sd_format_progress);
    tuya_ipc_ss_delete_all_files();
    sleep(1);

    //TODO
    /* 执行真正的SD卡格式化操作 */

    s_sd_format_progress = 80;
    IPC_APP_report_sd_format_status(s_sd_format_progress);
    //TODO
    tuya_ipc_ss_set_write_mode(SS_WRITE_MODE_ALL);
//    IPC_APP_set_sd_record_onoff( IPC_APP_get_sd_record_onoff());

    sleep(1);

    /* 最后100% */
    s_sd_format_progress = 100;
    IPC_APP_report_sd_format_status(s_sd_format_progress);

    pthread_exit(0);
}

VOID IPC_APP_format_sd_card(VOID)
{
    printf("start to format sd_card \r\n");
    /* SD卡格式化。
     * 这里SDK已经完成了部分代码的编写，开发者只需要实现格式化操作即可。 */

    pthread_t sd_format_thread;
    pthread_create(&sd_format_thread, NULL, thread_sd_format, NULL);
    pthread_detach(sd_format_thread);
}
#endif

#ifdef TUYA_DP_SD_FORMAT_STATUS_ONLY_GET
INT_T IPC_APP_get_sd_format_status(VOID)
{
    return s_sd_format_progress;
}
#endif

//------------------------------------------

#ifdef TUYA_DP_PTZ_CONTROL
VOID IPC_APP_ptz_start_move(CHAR_T *p_direction)
{
    printf("ptz start move:%s \r\n", p_direction);
    //TODO
    /* 云台转动开始,ENUM类型,0-上,1-右上,2-右,3-右下,4-下,5-左下,6-左,7-左上 */
}
#endif

#ifdef TUYA_DP_PTZ_STOP
VOID IPC_APP_ptz_stop_move(VOID)
{
    printf("ptz stop move \r\n");
    //TODO
    /* 云台转动停止 */

}
#endif

//------------------------------------------

#ifdef TUYA_DP_BLUB_SWITCH
VOID IPC_APP_set_blub_onoff(BOOL_T blub_on_off)
{
    printf("set blub_on_off:%d \r\n", blub_on_off);
    //TODO
    /* 特殊灯光控制开关,BOOL类型,true打开,false关闭 */

    __tuya_app_write_INT("tuya_blub_on_off", blub_on_off);
}

BOOL_T IPC_APP_get_blub_onoff(VOID)
{
    BOOL_T blub_on_off = __tuya_app_read_INT("tuya_blub_on_off");
    printf("curr blub_on_off:%d \r\n", blub_on_off);
    return blub_on_off;
}
#endif

//------------------------------------------

#ifdef TUYA_DP_ELECTRICITY
INT_T IPC_APP_get_battery_percent(VOID)
{
    //TODO
    /* 电池电量(百分比),VALUE类型,[0-100] */

    return 80;
}
#endif

#ifdef TUYA_DP_POWERMODE
CHAR_T *IPC_APP_get_power_mode(VOID)
{
    //TODO
    /* 供电方式,ENUM类型,"0"为电池供电状态,"1"为插电供电状态(或电池充电状态) */

    return "1";
}
#endif
