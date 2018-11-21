/*!
\file tuya_iot_com_api.h
Copyright(C),2017, 涂鸦科技 www.tuya.comm
*/
#ifndef __TUYA_IOT_BASE_API_H
    #define __TUYA_IOT_BASE_API_H

    #include "tuya_cloud_types.h"
    #include "tuya_cloud_com_defs.h"
    #include "tuya_cloud_base_defs.h"
    #include "tuya_iot_com_api.h"

#ifdef __cplusplus
	extern "C" {
#endif

#ifdef  __TUYA_IOT_BASE_API_GLOBALS
    #define __TUYA_IOT_BASE_API_EXT
#else
    #define __TUYA_IOT_BASE_API_EXT extern
#endif

/***********************************************************
*************************micro define***********************
***********************************************************/

/***********************************************************
*************************variable define********************
***********************************************************/

/***********************************************************
*************************function define********************
***********************************************************/
/***********************************************************
*  Function: tuya_iot_set_gw_prod_info 
*  Input: prod_info
*  Output: none
*  Return: OPERATE_RET
***********************************************************/
__TUYA_IOT_BASE_API_EXT \
OPERATE_RET tuya_iot_set_gw_prod_info(IN CONST GW_PROD_INFO_S *prod_info);

/***********************************************************
*  Function: tuya_iot_wf_mcu_dev_init->The device consists of MCU and WiFi
*  Input: cfg
*         cbs->tuya wifi sdk user callbacks
*         product_key->product key/proudct id,get from tuya open platform
*         sw_ver->netlink module software version format:xx.xx.xx (0<=x<=9)
*         mcu_sw_ver->mcu software version format:xx.xx.xx (0<=x<=9)
*  Output: none
*  Return: OPERATE_RET
***********************************************************/
__TUYA_IOT_BASE_API_EXT \
OPERATE_RET tuya_iot_mcu_dev_init(IN CONST TY_IOT_CBS_S *cbs,IN CONST CHAR_T *product_key,\
                                            IN CONST CHAR_T *sw_ver,IN CONST CHAR_T *mcu_sw_ver);

/***********************************************************
*  Function: tuya_iot_soc_init->The devcie consists of wifi soc
*  Input: cbs->tuya wifi sdk user callbacks
*         product_key->product key/proudct id,get from tuya open platform
*         sw_ver->netlink module software version format:xx.xx.xx (0<=x<=9)
*  Output: none
*  Return: OPERATE_RET
***********************************************************/
__TUYA_IOT_BASE_API_EXT \
OPERATE_RET tuya_iot_soc_init(IN CONST TY_IOT_CBS_S *cbs,IN CONST CHAR_T *product_key,\
                                      IN CONST CHAR_T *sw_ver);


/***********************************************************
*  Function: tuya_iot_gw_init->tuya iot entity gateway initialization
*  Input: cbs->tuya wifi sdk user callbacks
*         gw_cbs->tuya gw user callbacks
*         product_key->product key/proudct id,get from tuya open platform
*         sw_ver->netlink module software version format:xx.xx.xx (0<=x<=9)
*         attr->gw attr
*         attr_num
*  Output: none
*  Return: OPERATE_RET
***********************************************************/
__TUYA_IOT_BASE_API_EXT \
OPERATE_RET tuya_iot_gw_init(IN CONST TY_IOT_CBS_S *cbs,IN CONST TY_IOT_GW_CBS_S *gw_cbs,\
                                     IN CONST CHAR_T *product_key,IN CONST CHAR_T *sw_ver,\
                                     IN CONST GW_ATTACH_ATTR_T *attr,IN CONST UINT_T attr_num);


/***********************************************************
*  Function: tuya_iot_gw_dev_init->tuya iot entity gateway initialization
*                                  the gateway make as a device meanwhile
*  Input: cfg
*         cbs->tuya wifi sdk user callbacks
*         gw_cbs->tuya gw user callbacks
*         product_key->product key/proudct id,get from tuya open platform
*         sw_ver->wifi module software version format:xx.xx.xx (0<=x<=9)
*         attr->gw attr
*         attr_num
*  Output: none
*  Return: OPERATE_RET
***********************************************************/
__TUYA_IOT_BASE_API_EXT \
OPERATE_RET tuya_iot_gw_dev_init(IN CONST TY_IOT_CBS_S *cbs,IN CONST TY_IOT_GW_CBS_S *gw_cbs,\
                                           IN CONST CHAR_T *product_key,IN CONST CHAR_T *sw_ver,\
                                           IN CONST GW_ATTACH_ATTR_T *attr,IN CONST UINT_T attr_num);

/***********************************************************
*  Function: tuya_iot_gw_unactive
*  Input: none
*  Output: none
*  Return: OPERATE_RET
***********************************************************/
__TUYA_IOT_BASE_API_EXT \
OPERATE_RET tuya_iot_gw_unactive(VOID);

/***********************************************************
*  Function: tuya_iot_reg_get_wf_nw_stat_cb
*  Input: wf_nw_stat_cb
*  Output: none
*  Return: OPERATE_RET
***********************************************************/
__TUYA_IOT_BASE_API_EXT \
OPERATE_RET tuya_iot_reg_get_nw_stat_cb(IN CONST GET_NW_STAT_CB nw_stat_cb);

#ifdef __cplusplus
}
#endif
#endif

