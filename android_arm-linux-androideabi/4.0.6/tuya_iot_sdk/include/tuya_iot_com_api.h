/*!
\file tuya_iot_com_api.h
Copyright(C),2017, 涂鸦科技 www.tuya.comm
*/

#ifndef __TUYA_IOT_COM_API_H
#define __TUYA_IOT_COM_API_H

#ifdef __cplusplus
extern "C" {
#endif

#include "tuya_cloud_types.h"
#include "tuya_cloud_com_defs.h"

/***********************************************************
*  Function: tuya_iot_get_sdk_info 
*  Input: none
*  Output: none
*  Return: (CHAR_T *)->sdk info
***********************************************************/
CHAR_T *tuya_iot_get_sdk_info(VOID);

/***********************************************************
*  Function: tuya_iot_init 
*  Input: fs_storge_path->filesystem read write storge path
*         (if os have no fs,then fs_storge_path is invalid)
*  Output: none
*  Return: OPERATE_RET
***********************************************************/
OPERATE_RET tuya_iot_init(IN CONST CHAR_T *fs_storge_path);

/***********************************************************
*  Function: tuya_iot_force_reg_gw_ug_cb
*  Input: ug_cb
*  Output: none
*  Return: none
*  Note: force register gw_ug_cb,Once set,take it as the main
         Highest priority
***********************************************************/
VOID tuya_iot_force_reg_gw_ug_cb(IN CONST GW_UG_INFORM_CB ug_cb);

/***********************************************************
*  Function: tuya_iot_oem_set
*  Input: ug_cb
*  Output: none
*  Return: none
*  Note: must call first
***********************************************************/
VOID tuya_iot_oem_set(IN CONST BOOL_T oem);

/***********************************************************
*  Function: tuya_iot_upgrade_gw->tuya iot upgrade gateway
*  Input: fw->fireware upgrade
*         get_file_cb->get file callback
*         upgrd_nofity_cb->upgrade notify callback
*         pri_data->private data
*  Output: none
*  Return: OPERATE_RET
***********************************************************/
OPERATE_RET tuya_iot_upgrade_gw(IN CONST FW_UG_S *fw,IN CONST GET_FILE_DATA_CB get_file_cb,\
                                         IN CONST UPGRADE_NOTIFY_CB upgrd_nofity_cb,IN CONST PVOID_T pri_data);

/***********************************************************
*  Function: tuya_iot_upgrade_gw->tuya iot upgrade gateway
*  Input: dev_id->设备ID
*         如果ID为空表示升级GW物理上在一起的其他模块
*        （比如：zigbee、ble、单品mcu(DEV_NM_NOT_ATH_SNGL)等）
*         不为空：表示升级与GW物理上分离的模块
*         fw->fireware upgrade
*         get_file_cb->get file callback
*         upgrd_nofity_cb->upgrade notify callback
*         pri_data->private data
*  Output: none
*  Return: OPERATE_RET
***********************************************************/
OPERATE_RET tuya_iot_upgrade_dev(IN CONST CHAR_T *devid,IN CONST FW_UG_S *fw, \
                                           IN CONST GET_FILE_DATA_CB get_file_cb,\
                                           IN CONST UPGRADE_NOTIFY_CB upgrd_nofity_cb,\
                                           IN CONST PVOID_T pri_data);

/***********************************************************
*  Function: tuya_iot_get_dev_if
*  Input: dev_id->device id,Note:if(dev_id == NULL) then means
*         the gateway device
*  Output: none
*  Return: (DEV_DESC_IF_S *) fail(NULL)
***********************************************************/
DEV_DESC_IF_S *tuya_iot_get_dev_if(IN CONST CHAR_T *dev_id);

/***********************************************************
*  Function: tuya_iot_dev_traversal 
*            Traversing all devices under the gateway, 
*            except the gateway device
*  Input: iterator
*  Output: none
*  Return: (DEV_DESC_IF_S *) (NULL means end)
***********************************************************/
DEV_DESC_IF_S *tuya_iot_dev_traversal(INOUT VOID **iterator);

/***********************************************************
*  Function: tuya_iot_get_dp_desc
*  Input: dev_id dpid
*  Output: none
*  Return: (DP_DESC_IF_S *) fail(NULL)
***********************************************************/
DP_DESC_IF_S *tuya_iot_get_dp_desc(IN CONST CHAR_T *dev_id,IN CONST BYTE_T dpid);

/***********************************************************
*  Function: tuya_iot_get_dp_desc
*  Input: none
*  Output: none
*  Return: (DP_DESC_IF_S *) fail(NULL)
***********************************************************/
DP_PROP_VALUE_U *tuya_iot_get_dp_prop_value(IN CONST CHAR_T *dev_id,IN CONST BYTE_T dpid);

/***********************************************************
*  Function: tuya_iot_gw_bind_dev
*  Input: tp uddd id pk ver
*  Output: none
*  Return: OPERATE_RET
***********************************************************/
OPERATE_RET tuya_iot_gw_bind_dev(IN CONST GW_PERMIT_DEV_TP_T tp,IN CONST USER_DEV_DTL_DEF_T uddd,\
                                           IN CONST CHAR_T *id,IN CONST CHAR_T *pk,IN CONST CHAR_T *ver);

/***********************************************************
*  Function: tuya_iot_gw_subdevice_update
*  Input: id ver
*  Output: none
*  Return: OPERATE_RET
***********************************************************/
OPERATE_RET tuya_iot_gw_subdevice_update(IN CONST CHAR_T *id,IN CONST CHAR_T *ver);

/***********************************************************
*  Function: tuya_iot_gw_unbind_dev
*  Input: id
*  Output: none
*  Return: OPERATE_RET
***********************************************************/
OPERATE_RET tuya_iot_gw_unbind_dev(IN CONST CHAR_T *id);

/***********************************************************
*  Function: tuya_iot_dev_online_stat_update
*  Input: dev_id online
*  Output: none
*  Return: OPERATE_RET
***********************************************************/
OPERATE_RET tuya_iot_dev_online_stat_update(IN CONST CHAR_T *dev_id,IN CONST BOOL_T online);

/***********************************************************
*  Function: tuya_iot_sys_mag_hb_init
*  Input: hb_send_cb
*         if(NULL == hb_send_cb) 
*             passive heartbeat pattern
*         else
*             active heartbeat pattern
*  Output: none
*  Return: OPERATE_RET
***********************************************************/
OPERATE_RET tuya_iot_sys_mag_hb_init(IN CONST DEV_HEARTBEAT_SEND_CB hb_send_cb);

/***********************************************************
*  Function: tuya_iot_set_dev_hb_timeout
*  Input: dev_id
*         hb_timeout_time->if(0xffffffff == hb_timeout_time)
*                          then device online forever
*  Output: none
*  Return: OPERATE_RET
***********************************************************/
OPERATE_RET tuya_iot_set_dev_hb_timeout(IN CONST CHAR_T *dev_id,IN CONST TIME_S hb_timeout_time);

/***********************************************************
*  Function: tuya_iot_refresh_dev_hb
*  Input: dev_id
*  Output: none
*  Return: OPERATE_RET
***********************************************************/
OPERATE_RET tuya_iot_fresh_dev_hb(IN CONST CHAR_T *dev_id);

/***********************************************************
*  Function: dev_report_dp_json_async
*  Input: dev_id->device id,Note:if(dev_id == NULL) then means
*         the gateway device report data
*         dp_data->dp data,note "time_stamp" is useless.The data 
                   reporting time is based on the actual arrival of the server
*         cnt->count
*  Output: none
*  Return: OPERATE_RET
***********************************************************/
OPERATE_RET dev_report_dp_json_async(IN CONST CHAR_T *dev_id,IN CONST TY_OBJ_DP_S *dp_data,IN CONST UINT_T cnt);

/***********************************************************
*  Function: dev_report_dp_raw_sync
*  Input: dev_id->device id,Note:if(dev_id == NULL) then means
*         the gateway device report data
*         data->raw data
          len->raw data len
*         timeout
*  Output: none
*  Return: OPERATE_RET
***********************************************************/
OPERATE_RET dev_report_dp_raw_sync(IN CONST CHAR_T *dev_id,IN CONST BYTE_T dpid,\
                                              IN CONST BYTE_T *data,IN CONST UINT_T len,\
                                              IN CONST UINT_T timeout);

/***********************************************************
*  Function: dev_report_dp_stat_sync
*  Input: dev_id->device id,Note:if(dev_id == NULL) then means
*         the gateway device report data
*         dp_data->dp data,note "time_stamp" is useless.if(0 == time_stamp) then 
                   the data reporting time is based on the actual arrival of the server
                   else reporting time depend on the time_stamp
*         cnt->count
*         timeout
*  Output: none
*  Return: OPERATE_RET
***********************************************************/
OPERATE_RET dev_report_dp_stat_sync(IN CONST CHAR_T *dev_id,IN CONST TY_OBJ_DP_S *dp_data,\
                                               IN CONST UINT_T cnt,IN CONST UINT_T timeout);

OPERATE_RET tuya_iot_get_wakeup_data(INOUT BYTE_T *wakeup_data_arr, INOUT UINT_T *p_len);

OPERATE_RET tuya_iot_get_heartbeat_data(INOUT BYTE_T *heartbeat_data_arr, INOUT UINT_T *p_len);

OPERATE_RET tuya_iot_book_wakeup_topic(VOID);

INT_T tuya_iot_get_mqtt_socket_fd(VOID);

OPERATE_RET tuya_iot_send_custom_mqtt_msg(IN CONST UINT_T protocol, IN CONST BYTE_T *p_data);



/*
开启流式传输服务
*/
OPERATE_RET tuya_iot_start_media_transfer(IN CONST USHORT_T session_id);

/*
停止流式传输服务
*/
OPERATE_RET tuya_iot_stop_media_transfer(IN CONST USHORT_T session_id);

/*
流式发送数据
*/
typedef VOID (*MEDIA_SEND_CB)(IN CONST OPERATE_RET op_ret,IN VOID *prv_data);
OPERATE_RET tuya_iot_send_media(IN CONST USHORT_T session_id,
                                UINT_T data_offset, USHORT_T data_len, BYTE_T *p_data,
                                MEDIA_SEND_CB send_cb, IN VOID *prv_data);





#ifdef __cplusplus
}
#endif

#endif
