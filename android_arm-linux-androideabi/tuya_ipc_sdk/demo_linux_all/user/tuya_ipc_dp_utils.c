/*********************************************************************************
  *Copyright(C),2015-2020, 涂鸦科技 www.tuya.comm
  *FileName: tuya_ipc_dp_utils.c
  *
  * 文件描述：
  * 1. DP点工具API实现
  *
  * 本文件代码为工具代码，用户可以不用关心
  * 请勿随意修改该文件中的任何内容，如需修改请联系涂鸦产品经理！！
  *
**********************************************************************************/
#include "tuya_ipc_api.h"
#include "tuya_ipc_dp_utils.h"
#include "tuya_ipc_dp_handler.h"
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

STATIC VOID respone_dp_value(CHAR *p_dp_str, INT val);
STATIC VOID respone_dp_bool(CHAR *p_dp_str, BOOL true_false);
STATIC VOID respone_dp_enum(CHAR *p_dp_str, CHAR *p_val_enum);
STATIC VOID respone_dp_str(CHAR *p_dp_str, CHAR *p_val_str);
STATIC VOID handle_DP_SD_STORAGE_ONLY_GET(IN VOID *p_dp_info, IN cJSON *p_dp_json);

//------------------------------------------
VOID IPC_APP_upload_all_status(VOID)
{
#ifdef TUYA_DP_SLEEP_MODE
    respone_dp_bool(TUYA_DP_SLEEP_MODE, IPC_APP_get_sleep_mode() );
#endif

#ifdef TUYA_DP_LIGHT
    respone_dp_bool(TUYA_DP_LIGHT, IPC_APP_get_light_onoff() );
#endif

#ifdef TUYA_DP_FLIP
    respone_dp_bool(TUYA_DP_FLIP, IPC_APP_get_flip_onoff() );
#endif

#ifdef TUYA_DP_WATERMARK
    respone_dp_bool(TUYA_DP_WATERMARK, IPC_APP_get_watermark_onoff() );
#endif

#ifdef TUYA_DP_WDR
    respone_dp_bool(TUYA_DP_WDR, IPC_APP_get_wdr_onoff() );
#endif

#ifdef TUYA_DP_NIGHT_MODE
    respone_dp_enum(TUYA_DP_NIGHT_MODE, IPC_APP_get_night_mode() );
#endif

#ifdef TUYA_DP_ALARM_FUNCTION
    respone_dp_bool(TUYA_DP_ALARM_FUNCTION, IPC_APP_get_alarm_function_onoff() );
#endif

#ifdef TUYA_DP_ALARM_SENSITIVITY
    respone_dp_enum(TUYA_DP_ALARM_SENSITIVITY, IPC_APP_get_alarm_sensitivity() );
#endif

//#ifdef TUYA_DP_ALARM_INTERVAL
//    respone_dp_enum(TUYA_DP_ALARM_INTERVAL, IPC_APP_get_alarm_interval() );
//#endif

#ifdef TUYA_DP_SD_STATUS_ONLY_GET
    respone_dp_value(TUYA_DP_SD_STATUS_ONLY_GET, IPC_APP_get_sd_status() );
#endif

#ifdef TUYA_DP_SD_STORAGE_ONLY_GET
    handle_DP_SD_STORAGE_ONLY_GET(NULL, NULL);
#endif

#ifdef TUYA_DP_SD_RECORD_ENABLE
    respone_dp_bool(TUYA_DP_SD_RECORD_ENABLE, IPC_APP_get_sd_record_onoff() );
#endif

#ifdef TUYA_DP_SD_RECORD_MODE
    respone_dp_enum(TUYA_DP_SD_RECORD_MODE, IPC_APP_get_sd_record_mode() );
#endif

#ifdef TUYA_DP_SD_FORMAT_STATUS_ONLY_GET
    respone_dp_value(TUYA_DP_SD_FORMAT_STATUS_ONLY_GET, 0 );
#endif

#ifdef TUYA_DP_BLUB_SWITCH
    respone_dp_bool(TUYA_DP_BLUB_SWITCH, IPC_APP_get_blub_onoff() );
#endif

#ifdef TUYA_DP_POWERMODE
    IPC_APP_update_battery_status();
#endif
}

#ifdef TUYA_DP_DOOR_BELL
VOID IPC_APP_trigger_door_bell(VOID)
{
    printf("door bell is triggered\r\n");

    BYTE wakeup_data_arr[10] = {0};
    UINT wakeup_data_len = 10;
    CHAR data[128] = {0};
    unsigned int wakeup_data = 0;
    tuya_ipc_get_wakeup_data(wakeup_data_arr, &wakeup_data_len);
    wakeup_data = (wakeup_data_arr[8] & 0xFF) |
                 ((wakeup_data_arr[7] << 8) & (0xFF00)) |
                 ((wakeup_data_arr[7] <<16) & (0xFF0000)) |
                 ((wakeup_data_arr[7] <<24) & (0xFF000000));

    TIME_T timeutc = 0;
    INT timezone = 0;
    tuya_ipc_get_service_time(&timeutc, &timezone);
    snprintf(data, 128, "{\"etype\":\"doorbell_press\",\"edata\":\"%x%d\"}", wakeup_data, (INT)timeutc);
    printf("DoorBell PUSH:%s\r\n", data);
    tuya_ipc_send_custom_mqtt_msg(43, data);


    UINT intval = time(NULL);
    CHAR strval[64] = {0};
    snprintf(strval, 64, "%d", intval);
    respone_dp_str(TUYA_DP_DOOR_BELL, strval);
}
#endif

#ifdef TUYA_DP_SD_FORMAT_STATUS_ONLY_GET
VOID IPC_APP_report_sd_format_status(INT status)
{
    respone_dp_value(TUYA_DP_SD_FORMAT_STATUS_ONLY_GET, status);
}
#endif

#ifdef TUYA_DP_SD_STATUS_ONLY_GET
VOID IPC_APP_report_sd_status_changed(VOID)
{
    respone_dp_value(TUYA_DP_SD_STATUS_ONLY_GET, IPC_APP_get_sd_status());
}
#endif

#ifdef TUYA_DP_POWERMODE
VOID IPC_APP_update_battery_status(VOID)
{
    CHAR *power_mode = IPC_APP_get_power_mode();
    INT percent = IPC_APP_get_battery_percent();

    printf("current power mode:%s\r\n", power_mode);
    respone_dp_enum(TUYA_DP_POWERMODE, power_mode);
    printf("current battery percent:%d\r\n", percent);
    respone_dp_value(TUYA_DP_ELECTRICITY, percent);
}
#endif

//------------------------------------------
STATIC VOID respone_dp_value(CHAR *p_dp_str, INT val)
{
    CHAR report[32] = {0};
    snprintf(report, 32, "{\"%s\":%d}", p_dp_str, val);
    printf("Respone: %s \r\n", report);
    tuya_ipc_dp_report(report, TRUE);
}

STATIC VOID respone_dp_bool(CHAR *p_dp_str, BOOL true_false)
{
    CHAR report[32] = {0};
    if(true_false == TRUE)
    {
        snprintf(report, 32, "{\"%s\":true}", p_dp_str);
    }
    else
    {
        snprintf(report, 32, "{\"%s\":false}", p_dp_str);
    }
    printf("Respone: %s \r\n", report);
    tuya_ipc_dp_report(report, TRUE);
}

STATIC VOID respone_dp_enum(CHAR *p_dp_str, CHAR *p_val_enum)
{
    CHAR report[32] = {0};
    snprintf(report, 32, "{\"%s\":\"%s\"}",p_dp_str, p_val_enum);
    printf("Respone: %s \r\n", report);
    tuya_ipc_dp_report(report, TRUE);
}

STATIC VOID respone_dp_str(CHAR *p_dp_str, CHAR *p_val_str)
{
    CHAR report[128] = {0};
    snprintf(report, 128, "{\"%s\":\"%s\"}",p_dp_str, p_val_str);
    printf("Respone: %s \r\n", report);
    tuya_ipc_dp_report(report, TRUE);
}

//------------------------------------------
STATIC BOOL check_dp_bool_invalid(IN cJSON *p_dp_json)
{
    if(p_dp_json == NULL)
    {
        printf("error! input is null \r\n");
        return -1;
    }

    if(p_dp_json->type == cJSON_False)
    {
        return FALSE;
    }
    else if(p_dp_json->type == cJSON_True)
    {
        return TRUE;
    }else
    {
        printf("Error!! type invalid %d \r\n", p_dp_json->type);
        return -2;
    }
}

//------------------------------------------

#ifdef TUYA_DP_SLEEP_MODE
STATIC VOID handle_DP_SLEEP_MODE(IN VOID *p_dp_info, IN cJSON *p_dp_json)
{
    BOOL sleep_mode = check_dp_bool_invalid(p_dp_json);

    IPC_APP_set_sleep_mode(sleep_mode);
    sleep_mode = IPC_APP_get_sleep_mode();

    respone_dp_bool(TUYA_DP_SLEEP_MODE, sleep_mode);
}
#endif

#ifdef TUYA_DP_LIGHT
STATIC VOID handle_DP_LIGHT(IN VOID *p_dp_info, IN cJSON *p_dp_json)
{
    BOOL light_on_off = check_dp_bool_invalid(p_dp_json);

    IPC_APP_set_light_onoff(light_on_off);
    light_on_off = IPC_APP_get_light_onoff();

    respone_dp_bool(TUYA_DP_LIGHT, light_on_off);
}
#endif

#ifdef TUYA_DP_FLIP
STATIC VOID handle_DP_FLIP(IN VOID *p_dp_info, IN cJSON *p_dp_json)
{
    BOOL flip_on_off = check_dp_bool_invalid(p_dp_json);

    IPC_APP_set_flip_onoff(flip_on_off);
    flip_on_off = IPC_APP_get_flip_onoff();

    respone_dp_bool(TUYA_DP_FLIP, flip_on_off);
}
#endif

#ifdef TUYA_DP_WATERMARK
STATIC VOID handle_DP_WATERMARK(IN VOID *p_dp_info, IN cJSON *p_dp_json)
{
    BOOL watermark_on_off = check_dp_bool_invalid(p_dp_json);

    IPC_APP_set_watermark_onoff(watermark_on_off);
    watermark_on_off = IPC_APP_get_watermark_onoff();

    respone_dp_bool(TUYA_DP_WATERMARK, watermark_on_off);
}
#endif

#ifdef TUYA_DP_WDR
STATIC VOID handle_DP_WDR(IN VOID *p_dp_info, IN cJSON *p_dp_json)
{
    BOOL wdr_on_off = check_dp_bool_invalid(p_dp_json);

    IPC_APP_set_wdr_onoff(wdr_on_off);
    wdr_on_off = IPC_APP_get_wdr_onoff();

    respone_dp_bool(TUYA_DP_WDR, wdr_on_off);
}
#endif

#ifdef TUYA_DP_NIGHT_MODE
STATIC VOID handle_DP_NIGHT_MODE(IN VOID *p_dp_info, IN cJSON *p_dp_json)
{
    if( (p_dp_json == NULL) || (p_dp_json->type != cJSON_String) )
    {
        printf("Error!! type invalid %d \r\n", p_dp_json->type);
        return;
    }

    CHAR *p_night_mode = p_dp_json->valuestring;

    IPC_APP_set_night_mode(p_night_mode);
    p_night_mode = IPC_APP_get_night_mode();

    respone_dp_enum(TUYA_DP_NIGHT_MODE, p_night_mode);
}
#endif

#ifdef TUYA_DP_CALIBRATION
STATIC VOID handle_DP_CALIBRATION(IN VOID *p_dp_info, IN cJSON *p_dp_json)
{
    IPC_APP_start_calibration();
    respone_dp_bool(TUYA_DP_CALIBRATION, TRUE);
}
#endif

#ifdef TUYA_DP_ALARM_FUNCTION
STATIC VOID handle_DP_ALARM_FUNCTION(IN VOID *p_dp_info, IN cJSON *p_dp_json)
{
    BOOL alarm_on_off = check_dp_bool_invalid(p_dp_json);

    IPC_APP_set_alarm_function_onoff(alarm_on_off);
    alarm_on_off = IPC_APP_get_alarm_function_onoff();

    respone_dp_bool(TUYA_DP_ALARM_FUNCTION, alarm_on_off);
}
#endif

#ifdef TUYA_DP_ALARM_SENSITIVITY
STATIC VOID handle_DP_ALARM_SENSITIVITY(IN VOID *p_dp_info, IN cJSON *p_dp_json)
{
    if( (p_dp_json == NULL) || (p_dp_json->type != cJSON_String) )
    {
        printf("Error!! type invalid %d \r\n", p_dp_json->type);
        return;
    }

    CHAR *p_sensitivity = p_dp_json->valuestring;

    IPC_APP_set_alarm_sensitivity(p_sensitivity);
    p_sensitivity = IPC_APP_get_alarm_sensitivity();

    respone_dp_enum(TUYA_DP_ALARM_SENSITIVITY, p_sensitivity);
}
#endif

//#ifdef TUYA_DP_ALARM_INTERVAL
//STATIC VOID handle_DP_ALARM_INTERVAL(IN VOID *p_dp_info, IN cJSON *p_dp_json)
//{
//    if( (p_dp_json == NULL) || (p_dp_json->type != cJSON_String) )
//    {
//        printf("Error!! type invalid %d \r\n", p_dp_json->type);
//        return;
//    }

//    CHAR *p_interval = p_dp_json->valuestring;

//    IPC_APP_set_alarm_interval(p_interval);
//    p_interval = IPC_APP_get_alarm_interval();

//    respone_dp_enum(TUYA_DP_ALARM_INTERVAL, p_interval);
//}
//#endif

#ifdef TUYA_DP_SD_STATUS_ONLY_GET
STATIC VOID handle_DP_SD_STATUS_ONLY_GET(IN VOID *p_dp_info, IN cJSON *p_dp_json)
{
    INT sd_status = IPC_APP_get_sd_status();

    respone_dp_value(TUYA_DP_SD_STATUS_ONLY_GET, sd_status);
}
#endif

#ifdef TUYA_DP_SD_STORAGE_ONLY_GET
STATIC VOID handle_DP_SD_STORAGE_ONLY_GET(IN VOID *p_dp_info, IN cJSON *p_dp_json)
{
    CHAR tmp_str[100] = {0};

    UINT total = 100;
    UINT used = 0;
    UINT empty = 100;
    IPC_APP_get_sd_storage(&total, &used, &empty);

    //"总容量|当前使用量|剩余容量"
    snprintf(tmp_str, 100, "%u|%u|%u", total, used, empty);
    respone_dp_str(TUYA_DP_SD_STORAGE_ONLY_GET, tmp_str);
}
#endif

#ifdef TUYA_DP_SD_RECORD_ENABLE
STATIC VOID handle_DP_SD_RECORD_ENABLE(IN VOID *p_dp_info, IN cJSON *p_dp_json)
{
    BOOL sd_record_on_off = check_dp_bool_invalid(p_dp_json);

    IPC_APP_set_sd_record_onoff(sd_record_on_off);
    sd_record_on_off = IPC_APP_get_sd_record_onoff();

    respone_dp_bool(TUYA_DP_SD_RECORD_ENABLE, sd_record_on_off);
}
#endif

#ifdef TUYA_DP_SD_RECORD_MODE
STATIC VOID handle_DP_SD_RECORD_MODE(IN VOID *p_dp_info, IN cJSON *p_dp_json)
{
    if( (p_dp_json == NULL) || (p_dp_json->type != cJSON_String) )
    {
        printf("Error!! type invalid %d \r\n", p_dp_json->type);
        return;
    }

    CHAR *p_mode = p_dp_json->valuestring;

    IPC_APP_set_sd_record_mode(p_mode);
    p_mode = IPC_APP_get_sd_record_mode();

    respone_dp_enum(TUYA_DP_SD_RECORD_MODE, p_mode);
}
#endif

#ifdef TUYA_DP_SD_UMOUNT
STATIC VOID handle_DP_SD_UMOUNT(IN VOID *p_dp_info, IN cJSON *p_dp_json)
{
    BOOL umount_result = IPC_APP_unmount_sd_card();
    respone_dp_bool(TUYA_DP_SD_UMOUNT, umount_result);
}
#endif

#ifdef TUYA_DP_SD_FORMAT
STATIC VOID handle_DP_SD_FORMAT(IN VOID *p_dp_info, IN cJSON *p_dp_json)
{
    IPC_APP_format_sd_card();
    respone_dp_bool(TUYA_DP_SD_FORMAT, TRUE);
}
#endif

#ifdef TUYA_DP_SD_FORMAT_STATUS_ONLY_GET
STATIC VOID handle_DP_SD_FORMAT_STATUS_ONLY_GET(IN VOID *p_dp_info, IN cJSON *p_dp_json)
{
    INT progress = IPC_APP_get_sd_format_status();
    respone_dp_value(TUYA_DP_SD_FORMAT_STATUS_ONLY_GET, progress);
}
#endif

#ifdef TUYA_DP_PTZ_CONTROL
STATIC VOID handle_DP_PTZ_CONTROL(IN VOID *p_dp_info, IN cJSON *p_dp_json)
{
    if( (p_dp_json == NULL) || (p_dp_json->type != cJSON_String) )
    {
        printf("Error!! type invalid %d \r\n", p_dp_json->type);
        return;
    }

    CHAR *p_control = p_dp_json->valuestring;
    IPC_APP_ptz_start_move(p_control);
    respone_dp_enum(TUYA_DP_PTZ_CONTROL, p_control);
}
#endif

#ifdef TUYA_DP_PTZ_STOP
STATIC VOID handle_DP_PTZ_STOP(IN VOID *p_dp_info, IN cJSON *p_dp_json)
{
    IPC_APP_ptz_stop_move();
    respone_dp_bool(TUYA_DP_PTZ_STOP, TRUE);
}
#endif

#ifdef TUYA_DP_DOOR_BELL
STATIC VOID handle_DP_DOOR_BELL(IN VOID *p_dp_info, IN cJSON *p_dp_json)
{
    printf("error! door bell can only trigged by IPC side.\r\n");
    respone_dp_str(TUYA_DP_DOOR_BELL, "-1");
}
#endif

#ifdef TUYA_DP_BLUB_SWITCH
STATIC VOID handle_DP_BLUB_SWITCH(IN VOID *p_dp_info, IN cJSON *p_dp_json)
{
    BOOL blub_on_off = check_dp_bool_invalid(p_dp_json);

    IPC_APP_set_blub_onoff(blub_on_off);
    blub_on_off = IPC_APP_get_blub_onoff();

    respone_dp_bool(TUYA_DP_BLUB_SWITCH, blub_on_off);
}
#endif

#ifdef TUYA_DP_ELECTRICITY
STATIC VOID handle_DP_ELECTRICITY(IN VOID *p_dp_info, IN cJSON *p_dp_json)
{
    INT percent = IPC_APP_get_battery_percent();
    printf("current battery percent:%d\r\n", percent);
    respone_dp_value(TUYA_DP_ELECTRICITY, percent);
}
#endif

#ifdef TUYA_DP_POWERMODE
STATIC VOID handle_DP_POWERMODE(IN VOID *p_dp_info, IN cJSON *p_dp_json)
{
    CHAR *power_mode = IPC_APP_get_power_mode();
    printf("current power mode:%s\r\n", power_mode);
    respone_dp_enum(TUYA_DP_POWERMODE, power_mode);
}
#endif

#ifdef TUYA_DP_LOWELECTRIC
STATIC VOID handle_DP_LOWELECTRIC(IN VOID *p_dp_info, IN cJSON *p_dp_json)
{
    if( (p_dp_json == NULL) || (p_dp_json->type != cJSON_Number) )
    {
        printf("Error!! type invalid %d \r\n", p_dp_json->type);
        return;
    }
    respone_dp_value(TUYA_DP_LOWELECTRIC, p_dp_json->valueint);
}
#endif

STATIC VOID handle_DP_RESERVED(IN VOID *p_dp_info, IN cJSON *p_dp_json)
{
    printf("error! not implememt yet.\r\n");
}


typedef VOID (*TUYA_DP_HANDLER)(IN VOID *p_dp_info, IN cJSON *p_dp_json);
typedef struct
{
    CHAR* p_dp_str;
    TUYA_DP_HANDLER handler;
}TUYA_DP_INFO_S;

STATIC TUYA_DP_INFO_S s_dp_table[] =
{
#ifdef TUYA_DP_SLEEP_MODE
    {TUYA_DP_SLEEP_MODE,            handle_DP_SLEEP_MODE},
#endif
#ifdef TUYA_DP_LIGHT
    {TUYA_DP_LIGHT,                 handle_DP_LIGHT},
#endif
#ifdef TUYA_DP_FLIP
    {TUYA_DP_FLIP,                  handle_DP_FLIP},
#endif
#ifdef TUYA_DP_WATERMARK
    {TUYA_DP_WATERMARK,             handle_DP_WATERMARK},
#endif
#ifdef TUYA_DP_WDR
    {TUYA_DP_WDR,                   handle_DP_WDR},
#endif
#ifdef TUYA_DP_NIGHT_MODE
    {TUYA_DP_NIGHT_MODE,            handle_DP_NIGHT_MODE},
#endif
#ifdef TUYA_DP_CALIBRATION
    {TUYA_DP_CALIBRATION,           handle_DP_CALIBRATION},
#endif
#ifdef TUYA_DP_ALARM_FUNCTION
    {TUYA_DP_ALARM_FUNCTION,        handle_DP_ALARM_FUNCTION},
#endif
#ifdef TUYA_DP_ALARM_SENSITIVITY
    {TUYA_DP_ALARM_SENSITIVITY,     handle_DP_ALARM_SENSITIVITY},
#endif
//#ifdef TUYA_DP_ALARM_INTERVAL
//    {TUYA_DP_ALARM_INTERVAL,        handle_DP_ALARM_INTERVAL},
//#endif
#ifdef TUYA_DP_SD_STATUS_ONLY_GET
    {TUYA_DP_SD_STATUS_ONLY_GET,    handle_DP_SD_STATUS_ONLY_GET},
#endif
#ifdef TUYA_DP_SD_STORAGE_ONLY_GET
    {TUYA_DP_SD_STORAGE_ONLY_GET,   handle_DP_SD_STORAGE_ONLY_GET},
#endif
#ifdef TUYA_DP_SD_RECORD_ENABLE
    {TUYA_DP_SD_RECORD_ENABLE,      handle_DP_SD_RECORD_ENABLE},
#endif
#ifdef TUYA_DP_SD_RECORD_MODE
    {TUYA_DP_SD_RECORD_MODE,        handle_DP_SD_RECORD_MODE},
#endif
#ifdef TUYA_DP_SD_UMOUNT
    {TUYA_DP_SD_UMOUNT,             handle_DP_SD_UMOUNT},
#endif
#ifdef TUYA_DP_SD_FORMAT
    {TUYA_DP_SD_FORMAT,             handle_DP_SD_FORMAT},
#endif
#ifdef TUYA_DP_SD_FORMAT_STATUS_ONLY_GET
    {TUYA_DP_SD_FORMAT_STATUS_ONLY_GET, handle_DP_SD_FORMAT_STATUS_ONLY_GET},
#endif
#ifdef TUYA_DP_PTZ_CONTROL
    {TUYA_DP_PTZ_CONTROL,           handle_DP_PTZ_CONTROL},
#endif
#ifdef TUYA_DP_PTZ_STOP
    {TUYA_DP_PTZ_STOP,              handle_DP_PTZ_STOP},
#endif
#ifdef TUYA_DP_DOOR_BELL
    {TUYA_DP_DOOR_BELL,             handle_DP_DOOR_BELL},
#endif
#ifdef TUYA_DP_BLUB_SWITCH
    {TUYA_DP_BLUB_SWITCH,           handle_DP_BLUB_SWITCH},
#endif
#ifdef TUYA_DP_SOUND_DETECT
    {TUYA_DP_SOUND_DETECT,          handle_DP_RESERVED},
#endif
#ifdef TUYA_DP_SOUND_SENSITIVITY
    {TUYA_DP_SOUND_SENSITIVITY,     handle_DP_RESERVED},
#endif
#ifdef TUYA_DP_SOUND_ALARM
    {TUYA_DP_SOUND_ALARM,           handle_DP_RESERVED},
#endif
#ifdef TUYA_DP_TEMPERATURE
    {TUYA_DP_TEMPERATURE,           handle_DP_RESERVED},
#endif
#ifdef TUYA_DP_HUMIDITY
    {TUYA_DP_HUMIDITY,              handle_DP_RESERVED},
#endif
#ifdef TUYA_DP_ELECTRICITY
    {TUYA_DP_ELECTRICITY,           handle_DP_ELECTRICITY},
#endif
#ifdef TUYA_DP_POWERMODE
    {TUYA_DP_POWERMODE,             handle_DP_POWERMODE},
#endif
#ifdef TUYA_DP_LOWELECTRIC
    {TUYA_DP_LOWELECTRIC,           handle_DP_LOWELECTRIC},
#endif
};

VOID IPC_APP_handle_dp_cmd_jsons(IN cJSON *p_dp_jsons)
{
    INT table_count = ( sizeof(s_dp_table) / sizeof(s_dp_table[0]) );
    INT i = 0;
    cJSON *json = NULL;
    for(i = 0; i < table_count; i ++)
    {
        json = cJSON_GetObjectItem(p_dp_jsons, s_dp_table[i].p_dp_str );
        if(json != NULL)
        {
            s_dp_table[i].handler( s_dp_table + i , json);
        }
    }
}

VOID IPC_APP_handle_dp_query_jsons(IN cJSON *p_dp_jsons)
{
    INT json_arr_size = cJSON_GetArraySize(p_dp_jsons);
    INT table_count = ( sizeof(s_dp_table) / sizeof(s_dp_table[0]) );
    INT i = 0;
    INT j = 0;
    cJSON *json = NULL;

    for(j = 0; j < json_arr_size; j++)
    {
        json = cJSON_GetArrayItem(p_dp_jsons, j);
        if(json != NULL)
        {
            for(i = 0; i < table_count; i ++)
            {
                if(strcmp(s_dp_table[i].p_dp_str, json->valuestring) == 0)
                {
                    s_dp_table[i].handler( s_dp_table + i , NULL);
                    break;
                }
            }
        }
    }
}

OPERATE_RET IPC_APP_Send_Motion_Alarm_From_Buffer(CHAR *data, UINT size, NOTIFICATION_CONTENT_TYPE_E type)
{
    OPERATE_RET ret = OPRT_OK;
    INT try = 3;
    INT count = 1;
    VOID *message = NULL;
    INT message_size = 0;
#ifdef TUYA_DP_ALARM_FUNCTION
    if(IPC_APP_get_alarm_function_onoff() != TRUE)
    {
        printf("motion alarm upload not enabled.skip \r\n");
        return OPRT_COM_ERROR;
    }
#endif

    printf("Send Motion Alarm. size:%d type:%d\r\n", size, type);
    message_size = tuya_ipc_notification_message_malloc(count, &message);
    if((message_size == 0)||(message == NULL))
    {
        printf("tuya_ipc_notification_message_malloc failed\n");
        return OPRT_COM_ERROR;
    }

    memset(message, 0, message_size);
    while (try != 0)
    {
        ret = tuya_ipc_notification_content_upload_from_buffer(type,data,size,message);
        if(ret != OPRT_OK)
        {
            try --;
            continue;
        }
        break;
    }
    if(ret == OPRT_OK)
    {
        ret = tuya_ipc_notification_message_upload(atoi(TUYA_DP_MOTION_DETECTION_ALARM), message, 5);
    }

    tuya_ipc_notification_message_free(message);

    return ret;
}

OPERATE_RET IPC_APP_Send_Motion_Alarm(CHAR *p_abs_file, NOTIFICATION_CONTENT_TYPE_E file_type)
{
#ifdef TUYA_DP_ALARM_FUNCTION
    if(IPC_APP_get_alarm_function_onoff() != TRUE)
    {
        printf("motion alarm upload not enabled.skip \r\n");
        return OPRT_COM_ERROR;
    }
#endif

    OPERATE_RET ret = OPRT_OK;
    INT try = 3;
    INT count = 1;
    VOID *message = NULL;
    INT size = 0;

    printf("Send Motion Alarm. type:%d File:%s\r\n", file_type, p_abs_file);

    size = tuya_ipc_notification_message_malloc(count, &message);
    if((size == 0)||(message == NULL))
    {
        printf("tuya_ipc_notification_message_malloc failed\n");
        return OPRT_COM_ERROR;
    }

    memset(message, 0, size);
    while (try != 0)
    {
        ret = tuya_ipc_notification_content_upload_from_file(p_abs_file, file_type, message);
        if(ret != OPRT_OK)
        {
            try --;
            continue;
        }
        break;
    }
    if(ret == OPRT_OK)
    {
        ret = tuya_ipc_notification_message_upload(atoi(TUYA_DP_MOTION_DETECTION_ALARM), message, 5);
    }

    tuya_ipc_notification_message_free(message);

    return ret;
}

OPERATE_RET IPC_APP_Send_DoorBell_Snap(CHAR *p_snap_file, NOTIFICATION_CONTENT_TYPE_E file_type)
{
    OPERATE_RET ret = OPRT_OK;
    INT try = 3;
    INT count = 1;
    VOID *message = NULL;
    INT size = 0;

    printf("Send DoorBell Snap. type:%d File:%s\r\n", file_type, p_snap_file);
    size = tuya_ipc_notification_message_malloc(count, &message);
    if((size == 0)||(message == NULL))
    {
        printf("tuya_ipc_notification_message_malloc failed\n");
        return OPRT_COM_ERROR;
    }

    memset(message, 0, size);
    while (try != 0)
    {
        ret = tuya_ipc_notification_content_upload_from_file(p_snap_file, file_type, message);
        if(ret != OPRT_OK)
        {
            try --;
            continue;
        }
        break;
    }
    if(ret == OPRT_OK)
    {
        ret = tuya_ipc_snapshot_message_upload(atoi(TUYA_DP_DOOR_BELL_SNAP), message, 5);
    }

    tuya_ipc_notification_message_free(message);

    return ret;
}



