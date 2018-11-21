#ifndef __RBAVHEADER_H__
#define __RBAVHEADER_H__

#include "tuya_cloud_types.h"
#include "tuya_ipc_media.h"


#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


typedef INT_T (*TUYA_CHROMECAST_START)(PVOID_T context, PVOID_T priv_data);
typedef INT_T (*TUYA_CHROMECAST_STOP)(PVOID_T context, PVOID_T priv_data);

typedef struct {
    PVOID_T						pcontext;
	TUYA_CHROMECAST_START 		start;
	TUYA_CHROMECAST_STOP		stop;
} TUYA_CHROMECAST_CALLBACK;


typedef struct {
	IPC_MEDIA_INFO_S* 			pminfo;
	TUYA_CHROMECAST_CALLBACK    cbk;
} TUYA_CHROMECAST_PARAM_S;


OPERATE_RET tuya_ipc_chromecast_init(TUYA_CHROMECAST_PARAM_S* param);


OPERATE_RET tuya_ipc_chromecast_deinit(VOID);


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif
