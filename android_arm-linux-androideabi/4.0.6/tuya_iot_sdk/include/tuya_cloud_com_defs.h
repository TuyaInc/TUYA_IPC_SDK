/*
tuya_cloud_com_defs.h
Copyright(C),2018-2020, 涂鸦科技 www.tuya.comm
*/

#ifndef TUYA_CLOUD_COM_DEFS_H
#define TUYA_CLOUD_COM_DEFS_H

#include "tuya_cloud_types.h"

#ifdef __cplusplus
extern "C" {
#endif

// TUYA SDK CONSTANTS DEFINES
#define GW_UUID_LEN             25      // max string length of UUID
#define AUTH_KEY_LEN            32      // max string length of AUTH_KEY
#define PRODUCT_KEY_LEN         16      // max string length of PRODUCT_KEY
#define SW_VER_LEN              10      // max string length of VERSION

#define GW_ID_LEN               25      // max string length of GW_ID
#define DEV_ID_LEN              25      // max string length of DEV_ID
#define SCHEMA_ID_LEN           16      // max string length of SCHEMA
#define BIND_TOKEN_LEN          16      // max string length of BIND_TOKEN
#define TOKEN_LEN               8       // max string length of TOKEN
#define REGION_LEN              2       // max string length of REGIN IN TOKEN
#define REGIST_KEY_LEN          4       // max string length of REGIST_KEY IN TOKEN
#define HTTP_URL_LMT            128     // max string length of URL
#define MQ_URL_LMT              128     // max string length of MQTT_URL
#define TIME_ZONE_LEN           10      // max string length of TIME_ZONE
#define SUMMER_TM_ZONE_LEN      256     // max string length of SUMMER_TIME_ZONE

#define SEC_KEY_LEN             16      // max string length of SECURITY_KEY
#define LOCAL_KEY_LEN           16      // max string length of LOCAL_KEY

#define WXAPPID_LEN             32      // max string length of wechat_appid
#define WXUUID_LEN              32      // max string length of wechat_uuid

#define GRP_ID_LEN              5       // max string length of group id range:1-65535
#define SCENE_ID_LEN            3       // max string length of scene id range:1-255


// Product Function Specification
typedef BYTE_T GW_ABI;
#define GW_VIRTUAL 0x00 // product doesn't have subdevices functions,it only has device function. For example: SOC or MCU
#define GW_ENTITY_ATTH_DEV 0x01 // Product has subdevices functions and it also has device function.
#define GW_ENTITY 0x02 // Product only has subdevices functions.

// Product Network Connection
typedef BYTE_T GW_NW_STAT_T;
#define GNS_UNVALID 0    // product is offline in LAN and WAN
#define GNS_LAN_VALID 1  // product is online in LAN but offline in WAN
#define GNS_WAN_VALID 2  // product is online in LAN and WAN

// Product Extend State
typedef BYTE_T GW_EXT_STAT_T;
#define EXT_UNAUTH 0
#define EXT_PROD_TEST 1
#define EXT_NORMAL_S 2
#define EXT_GW_UPGRD 3
#define EXT_DEV_UPGRD 4
#define EXT_DEV_ADD 5

// Subdevice Type IN Local Namespace
typedef BYTE_T DEV_TYPE_T;
#define DEV_NM_ATH_SNGL 0       // netlink module attach single device,(devid == gwid)
#define DEV_NM_NOT_ATH_SNGL 1   // netlink module not attach single device,(devid == gwid)
#define DEV_ZB_SNGL 2           // zigbee single device
#define DEV_RF433_SNGL 3        // rf433 single device
#define DEV_BLE_SNGL 4          // ble single device

// device detial type
typedef UINT_T USER_DEV_DTL_DEF_T; // user detial type define

typedef BYTE_T GW_PERMIT_DEV_TP_T;
#define GP_DEV_ZB DEV_ZB_SNGL
#define GP_DEV_RF433 DEV_RF433_SNGL
#define GP_DEV_BLE DEV_BLE_SNGL

typedef struct {
    CHAR_T id[DEV_ID_LEN+1];
    CHAR_T sw_ver[SW_VER_LEN+1];
    CHAR_T schema_id[SCHEMA_ID_LEN+1];
    CHAR_T product_key[PRODUCT_KEY_LEN+1]; // add by nzy 20151016,for support product key to activate device
    USER_DEV_DTL_DEF_T uddd; // user detial type define
    DEV_TYPE_T tp;
    BOOL_T bind;
    BOOL_T sync;
}DEV_DESC_IF_S;

typedef struct {
    GW_PERMIT_DEV_TP_T tp;
    CHAR_T ver[SW_VER_LEN+1]; // xx.xx.xx
}GW_ATTACH_ATTR_T;

// dp type
typedef BYTE_T DP_TYPE_E;
#define T_OBJ 0
#define T_RAW 1
#define T_FILE 2

// dp mode
typedef BYTE_T DP_MODE_E;
#define M_RW 0
#define M_WR 1    /* 外部只可以写入dev 不可读取 */
#define M_RO 2    /* 外部只能从dev读取，不可写入 */

// dp schema type
typedef BYTE_T DP_PROP_TP_E;
#define PROP_BOOL 0
#define PROP_VALUE 1
#define PROP_STR 2
#define PROP_ENUM 3
#define PROP_BITMAP 4

// dp prop
typedef struct {
    INT_T min;
    INT_T max;
    SHORT_T step;
    USHORT_T scale; // 描述value型DP的10的指数
    INT_T value;
}DP_PROP_VAL_S;

typedef struct {
    INT_T cnt;
    CHAR_T **pp_enum;
    INT_T value;
}DP_PROP_ENUM_S;

typedef struct {
    INT_T max_len;
    CHAR_T *value;
}DP_PROP_STR_S;

typedef struct {
    BOOL_T value;
}DP_BOOL_S;

typedef struct {
    UINT_T max_len;
    UINT_T value;
}DP_PROP_BITMAP;

typedef union {
    DP_PROP_VAL_S prop_value;
    DP_PROP_ENUM_S prop_enum;
    DP_PROP_STR_S prop_str;
    DP_BOOL_S prop_bool;
    DP_PROP_BITMAP prop_bitmap;
}DP_PROP_VALUE_U;

typedef BYTE_T DP_TRIG_T_E;
#define TRIG_PULSE 0      /* 变动上传 */
#define TRIG_DIRECT 1    /* 强制上传 */

/*
标识某DP状态是否为主动上报
DP SCHEMA中记录值为bool型
*/
typedef BYTE_T DP_PSV_E;
#define PSV_FALSE 0    /* disabled */
#define PSV_TRUE 1     /* 目前没有手机APP来主动查询 */
#define PSV_F_ONCE 2   /* 手机APP已经查询过，需要上报 */

// dp statistics type
typedef BYTE_T DP_STAT_TP_T;
#define DST_NONE 0 // no need statistics
#define DST_INC 1 // dp statistics increase
#define DST_TOTAL 2 // dp statistics total

typedef struct {
    BYTE_T dp_id;
    DP_MODE_E mode;  /* read rw write device视角来查看dp点属性*/
    DP_PSV_E passive;  /* true false once */
    DP_TYPE_E type;   /* obj raw file */
    DP_PROP_TP_E prop_tp; // type == obj时有效
    DP_TRIG_T_E trig_t; // 联动触发类型  pulse direct
    DP_STAT_TP_T stat; /* none total increase */
}DP_DESC_IF_S;

typedef BYTE_T DP_CMD_TYPE_E;

#define DP_CMD_LAN 0 /*!< 数据来自局域网 */
#define DP_CMD_MQ 1 /*!< 数据来自涂鸦云 */
#define DP_CMD_TIMER 2 /*!< 数据来自本地定时任务 */


typedef BYTE_T DP_TRANS_TYPE_T; 
#define DTT_SCT_UNC 0 // unicast 单品设备默认为单播
#define DTT_SCT_BNC 1 // boardcast
#define DTT_SCT_MNC 2 // multicast
#define DTT_SCT_SCENE 3 // scene


/*!
\brief 统计类DP信息值联合
\union STAT_DP_VALUE_U
*/
typedef union {
    INT_T dp_value;   /*!< 当DP类型为VALUE时，用来存储DP值 */
    UINT_T dp_enum;   /*!< 当DP类型为ENUM时，用来存储代表特定ENUM的序号 */
    CHAR_T *dp_str;   /*!< 当DP类型为STRING时，用来存储字符串 */
    BOOL_T dp_bool;   /*!< 当DP类型为BOOL时，用来存储DP值 */
    UINT_T dp_bitmap; /*!< 当DP类型为BITMAP时，用来存储DP值 */
}TY_OBJ_DP_VALUE_U;

/*!
\brief 统计类DP信息结构体
\struct TUYA_STAT_DP_INFO_S
*/
typedef struct {
    BYTE_T dpid; /*!< DP ID */
    DP_PROP_TP_E type; /*!< 对象DP的类型 */
    TY_OBJ_DP_VALUE_U value; /*!< 对象DP的值 */
    UINT_T time_stamp;  /*!< 对象DP的记录时间点，为0则为当前时间点 */
}TY_OBJ_DP_S;

typedef struct {
    DP_CMD_TYPE_E cmd_tp;
    DP_TRANS_TYPE_T dtt_tp;
    CHAR_T *cid; // if(NULL == cid) then then the cid represents gwid
    UINT_T dps_cnt; 
    TY_OBJ_DP_S dps[0];
}TY_RECV_OBJ_DP_S;

typedef struct {
    DP_CMD_TYPE_E cmd_tp;
    DP_TRANS_TYPE_T dtt_tp;
    CHAR_T *cid; // if(NULL == cid) then then the cid represents gwid
    BYTE_T dpid;
    UINT_T len;
    BYTE_T data[0];
}TY_RECV_RAW_DP_S;

typedef struct {
    CHAR_T *cid; // if(NULL == cid) then then the cid represents gwid
    UINT_T cnt; // dpid cnt if(0 == cnt) then query all object dp
    BYTE_T dpid[0]; // dpid
}TY_DP_QUERY_S;

/*!
\def FW_URL_LEN
\brief 固件下载URL长度
*/
#define FW_URL_LEN 255

/*!
\def FW_MD5_LEN
\brief 固件MD5字符串长度
*/
#define FW_MD5_LEN 32

/*!
\brief 固件升级信息结构体
\struct FW_UG_S
*/
typedef struct {
    DEV_TYPE_T tp;              /*!< 固件类型 */
    CHAR_T fw_url[FW_URL_LEN+1];  /*!< 固件文件URL */
    CHAR_T fw_md5[FW_MD5_LEN+1];  /*!< 固件文件md5值 */
    CHAR_T sw_ver[SW_VER_LEN+1];  /*!< 固件版本号 */
    UINT_T file_size;             /*!< 固件文件大小（单位字节） */
}FW_UG_S;

/*!
\brief 设备心跳发送回调
*/
typedef VOID (*DEV_HEARTBEAT_SEND_CB)(IN CONST CHAR_T *dev_id);

/*!
\brief 网关状态枚举
*/
typedef BYTE_T GW_STATUS_E;
#define GW_RESET 0 // gw reset
#define GW_ACTIVED 1 // gw actived

/*! 
\typedef typedef VOID (*GW_STATUS_CHANGED_CB)(IN CONST GW_STATUS_E status);
\brief 网关状态变化通知回调函数定义。

\param status [in] 最新状态
\param p_reserved [in] 目前为NULL
\return VOID
*/
typedef VOID (*GW_STATUS_CHANGED_CB)(IN CONST GW_STATUS_E status);

/*! 
typedef VOID (*DEV_OBJ_DP_CMD_CB)(IN CONST TY_RECV_OBJ_DP_S *dp);
\brief 子设备dp数据下发回调函数定义。
\param dp [in] 指令描述
\return VOID
*/
typedef VOID (*DEV_OBJ_DP_CMD_CB)(IN CONST TY_RECV_OBJ_DP_S *dp);

/*! 
typedef VOID (*DEV_RAW_DP_CMD_CB)(IN CONST TY_RECV_RAW_DP_S *dp);
\brief 子设备dp数据下发回调函数定义。
\param dp[in] 指令描述
\return VOID
*/
typedef VOID (*DEV_RAW_DP_CMD_CB)(IN CONST TY_RECV_RAW_DP_S *dp);

/*! 
\typedef typedef VOID (*DEV_DP_QUERY_CB)(IN DP_CMD_TYPE_E cmd_tp, IN CONST CHAR_T *p_dev_id, IN CONST BYTE_T *p_dp_arr, IN CONST UINT_T size);
\brief 子设备dp数据查询回调函数定义。

\param dp_qry [in] 设备数据查询通道,仅支持object类型dp查询
\return VOID
*/
typedef VOID (*DEV_DP_QUERY_CB)(IN CONST TY_DP_QUERY_S *dp_qry);

/*! 
\typedef typedef VOID (*GW_UG_INFORM_CB)(IN CONST FW_UG_S *fw);
\brief 获取网关固件升级信息回调函数定义

\param fw [in] 固件文件信息
\return VOID
*/
typedef VOID (*GW_UG_INFORM_CB)(IN CONST FW_UG_S *fw);

/*! 
\typedef typedef VOID (*GW_RESET_IFM_CB)(VOID);
\brief 网关重启应用程序通知

\param fw [in] none
\return VOID
*/
typedef VOID (*GW_RESET_IFM_CB)(VOID);


/*! 
\typedef typedef VOID (*DEV_UG_INFORM_CB)(IN CONST CHAR_T *dev_id,IN CONST FW_UG_S *fw);
\brief 获取子设备固件升级信息回调函数定义

\param tp [in] 子设备类型
\param dev_id [in] 子设备ID
       如果ID为空表示升级GW物理上在一起的其他模块（比如：zigbee、ble、单品mcu(DEV_NM_NOT_ATH_SNGL)等）
       不为空：表示升级与GW物理上分离的模块
\param fw [in] 固件文件信息
\return VOID
*/
typedef VOID (*DEV_UG_INFORM_CB)(IN CONST CHAR_T *dev_id,IN CONST FW_UG_S *fw);

/*! 
\typedef typedef OPERATE_RET (*GET_FILE_DATA_CB)(IN CONST FW_UG_S *fw, IN CONST UINT_T total_len,IN CONST UINT_T offset,IN CONST BYTE_T *data,IN CONST UINT_T len,OUT UINT_T *remain_len, IN PVOID_T pri_data);
\brief 固件文件下载回调函数定义

\param fw [in] 固件文件信息
\param total_len [in] 固件文件大小（单位字节）
\param offset [in] 当前回调函数文件下载偏移量
\param data [in] 当前回调函数已经下载内容
\param len [in] data的长度
\param remain_len [out] 通知涂鸦云当前回调函数本次处理内容长度 (*remain_len<=len)，本参数用于解决本地存储时可能出现的字节对齐问题
\param pri_data [in] 注册回调函数时设置的额外参数
\return VOID
*/
typedef OPERATE_RET (*GET_FILE_DATA_CB)(IN CONST FW_UG_S *fw, IN CONST UINT_T total_len,IN CONST UINT_T offset,\
                                                IN CONST BYTE_T *data,IN CONST UINT_T len,OUT UINT_T *remain_len, IN PVOID_T pri_data);

/*! 
\typedef typedef VOID (*UPGRADE_NOTIFY_CB)(IN CONST FW_UG_S *fw, IN CONST INT_T download_result, IN PVOID_T pri_data);
\brief 固件下载结束通知回调函数定义

\param fw [in] 固件文件信息
\param download_result [in] 固件下载状态
0:固件下载成功
其他值:固件下载失败
\param pri_data [in] 注册回调函数时设置的额外参数
\return VOID
*/
typedef VOID (*UPGRADE_NOTIFY_CB)(IN CONST FW_UG_S *fw, IN CONST INT_T download_result, IN PVOID_T pri_data);

typedef struct {
    GW_STATUS_CHANGED_CB gw_status_cb; /*!< 网关状态变更回调通知函数，可以为NULL */
    GW_UG_INFORM_CB gw_ug_cb; /*!< 网关固件升级通知信息回调函数，可以为NULL */
    GW_RESET_IFM_CB gw_reset_cb; /*!< 重置网关时,通过此接口通知用户重启应用程序 */
    DEV_OBJ_DP_CMD_CB dev_obj_dp_cb; /*!< 网关object类型dp指令回调，可以为NULL */
    DEV_RAW_DP_CMD_CB dev_raw_dp_cb; /*!< 网关raw类型dp指令回调，可以为NULL */
    DEV_DP_QUERY_CB dev_dp_query_cb; /*!< 设备指令查询接口，可以为NULL*/
    DEV_UG_INFORM_CB dev_ug_cb; /*!< 设备固件升级通知信息回调函数，可以为NULL */
}TY_IOT_CBS_S;

/***********************************************************
*  callback function: GW_PERMIT_ADD_DEV_CB
*  Input:   tp      subdevice type
*  Input:   permit  is permit to add subdevices
*  Output:  timeout timeout
*  Return:  BOOL_T  if function process success, return true, if function process fails,return false.
***********************************************************/
typedef BOOL_T (*GW_PERMIT_ADD_DEV_CB)(IN CONST GW_PERMIT_DEV_TP_T tp,IN CONST BOOL_T permit,IN CONST UINT_T timeout);

/***********************************************************
*  callback function: GW_DEV_DEL_CB
*  Input:   dev_id  subdevice_id
*  Output:  none
*  Return:  VOID    when a subdevice is deleted,use this callback to notify user
***********************************************************/
typedef VOID (*GW_DEV_DEL_CB)(IN CONST CHAR_T *dev_id);

/*
group process type
*/
typedef enum {
    GRP_ADD = 0, // add a group
    GRP_DEL      // delete a group
}GRP_ACTION_E;

/***********************************************************
*  callback function: GW_DEV_GRP_INFM_CB
*  Input:   action  group action type
*  Input:   dev_id  subdevice_id
*  Input:   grp_id  group_id
*  Output:  none
*  Return:  BOOL_T  if function process success, return true, if function process fails,return false.
***********************************************************/
typedef BOOL_T (*GW_DEV_GRP_INFM_CB)(IN CONST GRP_ACTION_E action,IN CONST CHAR_T *dev_id,IN CONST CHAR_T *grp_id);

/*
scene process type
*/
typedef enum {
    SCE_ADD = 0, // add a scene
    SCE_DEL,     // delete a scene
    SCE_EXEC     // execute a scene
}SCE_ACTION_E;

/***********************************************************
*  callback function: GW_DEV_SCENE_INFM_CB
*  Input:   action  scene action type
*  Input:   dev_id  subdevice_id
*  Input:   grp_id  group_id
*  Input:   sce_id  scene_id
*  Output:  none
*  Return:  BOOL_T  if function process success, return true, if function process fails,return false.
***********************************************************/
typedef BOOL_T (*GW_DEV_SCENE_INFM_CB)(IN CONST SCE_ACTION_E action, IN CONST CHAR_T *dev_id,
                                       IN CONST CHAR_T *grp_id, IN CONST CHAR_T *sce_id);

/***********************************************************
*  callback function: GW_BIND_DEV_INFORM_CB
*  Desc:
*  Input:   dev_id  subdevice_id
*  Input:   op_ret  OPRT_OK:bind success. other values:bind failure
*  Output:  none
*  Return:  OPERATE_RET
***********************************************************/
typedef VOID (*GW_BIND_DEV_INFORM_CB)(IN CONST CHAR_T *dev_id, IN CONST OPERATE_RET op_ret);

/*
callback functions of gateway subdevices management
*/
typedef struct {
    GW_PERMIT_ADD_DEV_CB gw_add_dev_cb;
    GW_DEV_DEL_CB gw_del_cb;
    GW_DEV_GRP_INFM_CB gw_dev_grp_cb;
    GW_DEV_SCENE_INFM_CB gw_dev_scene_cb;
    GW_BIND_DEV_INFORM_CB gw_ifm_cb;
}TY_IOT_GW_CBS_S;


#ifdef __cplusplus
}
#endif

#endif
