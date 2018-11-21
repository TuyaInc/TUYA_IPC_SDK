#ifndef _UNI_TIME_H
#define _UNI_TIME_H

#include "tuya_cloud_types.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifdef  __UNI_TIME_GLOBALS
    #define __UNI_TIME_EXT
#else
    #define __UNI_TIME_EXT extern
#endif

/***********************************************************
*************************micro define***********************
***********************************************************/
typedef UINT_T TIME_T;

typedef struct {
    /** seconds [0-59] */
    INT_T tm_sec;
    /** minutes [0-59] */
    INT_T tm_min;
    /** hours [0-23] */
    INT_T tm_hour;
    /** day of the month [1-31] */
    INT_T tm_mday;
    /** month [0-11] */
    INT_T tm_mon;
    /** year. The number of years since 1900 */
    INT_T tm_year;
    /** day of the week [0-6] 0-Sunday...6-Saturday */
    INT_T tm_wday;
}POSIX_TM_S;


#define SUM_ZONE_TAB_LMT 6
typedef struct {
    TIME_T posix_min;
    TIME_T posix_max;
}SUM_ZONE_S;

/***********************************************************
*************************variable define********************
***********************************************************/

/***********************************************************
*************************function define********************
***********************************************************/
/***********************************************************
*  Function: wmtime_init
*  Input: none
*  Output: none
*  Return: OPERATE_RET
***********************************************************/
__UNI_TIME_EXT \
OPERATE_RET uni_time_init(VOID);

/***********************************************************
*  Function: uni_mktime
*  Input: tm
*  Output: none
*  Return: 0:fail other:success
***********************************************************/
__UNI_TIME_EXT \
TIME_T uni_mktime(IN CONST POSIX_TM_S *tm);

/***********************************************************
*  Function: uni_gmtime_r
*  Input: tm
*  Output: result
*  Return: POSIX_TM_S*
***********************************************************/
__UNI_TIME_EXT \
POSIX_TM_S *uni_gmtime_r(IN CONST TIME_T *tm, OUT POSIX_TM_S *result);

/***********************************************************
*  Function: uni_time_set
*  Input: tm
*  Output: none
*  Return: OPERATE_RET
***********************************************************/
__UNI_TIME_EXT \
OPERATE_RET uni_time_set(IN CONST POSIX_TM_S *tm);

/***********************************************************
*  Function: uni_time_get
*  Input: tm
*  Output: none
*  Return: OPERATE_RET
***********************************************************/
__UNI_TIME_EXT \
OPERATE_RET uni_time_get(OUT POSIX_TM_S *tm);

/***********************************************************
*  Function: uni_http_date_to_time
*  Input: date->http date
*  Output: none
*  Return: TIME_T
***********************************************************/
__UNI_TIME_EXT \
TIME_T uni_http_date_to_time(IN CONST CHAR_T *date);

/***********************************************************
*  Function: uni_time_set_posix
*  Input: time
*  Output: none
*  Return: OPERATE_RET
***********************************************************/
__UNI_TIME_EXT \
OPERATE_RET uni_time_set_posix(IN CONST TIME_T time);

/***********************************************************
*  Function: uni_time_check_time_sync
*  Input: none
*  Output: none
*  Return: OPERATE_RET
***********************************************************/
__UNI_TIME_EXT \
OPERATE_RET uni_time_check_time_sync(VOID);

/***********************************************************
*  Function: uni_time_get_posix
*  Input: time
*  Output: none
*  Return: OPERATE_RET
***********************************************************/
__UNI_TIME_EXT \
TIME_T uni_time_get_posix(VOID);

/***********************************************************
*  Function: uni_get_system_time->how long system run
*  Input: none
*  Output: pSecTime->s 
*          pMsTime->ms
*  Return: none
***********************************************************/
__UNI_TIME_EXT \
VOID uni_get_system_time(OUT TIME_S *pSecTime,OUT TIME_MS *pMsTime);

/***********************************************************
*  Function: uni_set_time_zone
*  Input: time_zone->"+/-hh:mm"
*  Output: none
*  Return: none
***********************************************************/
__UNI_TIME_EXT \
OPERATE_RET uni_set_time_zone(IN CONST CHAR_T *time_zone);

/***********************************************************
*  Function: uni_get_time_zone_seconds
*  Input: none
*  Output: timezone in second
*  Return: none
***********************************************************/
__UNI_TIME_EXT \
OPERATE_RET uni_get_time_zone_seconds(OUT INT_T *time_zone);

/***********************************************************
*  Function: uni_local_time_get
*  Input: in_time
*  Output: tm
*  Return: OPERATE_RET
***********************************************************/
__UNI_TIME_EXT \
OPERATE_RET uni_local_time_get(IN TIME_T in_time, OUT POSIX_TM_S *tm);

/***********************************************************
*  Function: uni_sum_time_get
*  Input: tm
*  Output: none
*  Return: OPERATE_RET
***********************************************************/
__UNI_TIME_EXT \
OPERATE_RET uni_sum_time_get(OUT POSIX_TM_S *tm);

/***********************************************************
*  Function: uni_set_sum_zone_tbl
*  Input: zone cnt
*  Output: none
*  Return: OPERATE_RET
***********************************************************/
__UNI_TIME_EXT \
VOID uni_set_sum_zone_tbl(IN CONST SUM_ZONE_S *zone,IN CONST UINT_T cnt);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _UNI_TIME_H */


