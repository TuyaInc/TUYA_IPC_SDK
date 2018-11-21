/*********************************************************************************
  *Copyright(C),2017, 涂鸦科技 www.tuya.comm
  *FileName:    tuya_echo_show.h
**********************************************************************************/

#ifndef __TUYA_IPC_ECHO_SHOW_H__
#define __TUYA_IPC_ECHO_SHOW_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "tuya_cloud_types.h"
#include "tuya_cloud_error_code.h"
#include "tuya_ipc_media.h"


typedef INT_T (*TUYA_ECHOSHOW_START)(PVOID_T context, PVOID_T priv_data);
typedef INT_T (*TUYA_ECHOSHOW_STOP)(PVOID_T context, PVOID_T priv_data);

typedef struct {
    PVOID_T						pcontext;
	TUYA_ECHOSHOW_START 		start;
	TUYA_ECHOSHOW_STOP			stop;
} TUYA_ECHOSHOW_CALLBACK;


typedef struct {
	IPC_MEDIA_INFO_S* 			pminfo;
	TUYA_ECHOSHOW_CALLBACK    	cbk;
} TUYA_ECHOSHOW_PARAM_S;


OPERATE_RET tuya_ipc_echoshow_init(IN TUYA_ECHOSHOW_PARAM_S* pparam);


OPERATE_RET tuya_ipc_echoshow_deinit(VOID);


#ifdef __cplusplus
}
#endif

#endif
