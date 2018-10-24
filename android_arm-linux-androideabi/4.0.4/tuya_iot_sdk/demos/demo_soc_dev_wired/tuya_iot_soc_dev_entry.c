#include "tuya_cloud_types.h"
#include "tuya_cloud_error_code.h"
#include "tuya_cloud_com_defs.h"
#include "tuya_iot_com_api.h"
#include "uni_log.h"

#include "tuya_cloud_base_defs.h"
#include "tuya_iot_base_api.h"

#include <stdio.h>

// UserTODO
// SOC固件版本，用于OTA管理，格式必须为"XX.XX.XX"，其中XX必须为数字
#define USER_SW_VER         "1.2.3"
// 涂鸦云上的产品KEY，需登陆tuya.com创建产品分配唯一key
#define PRODUCT_KEY         "hDbYlEbRkVkUuj6R"
// SD本地配置存储路径，该路径必须对应一个可读写文件系统分区
#define CFG_STORAGE_PATH    "./"
// UUID和AUTHKEY用于涂鸦云设备的安全认证，每个设备所用key均为唯一
#define UUID                "tuya985d5f5255e0342b"
#define AUTHKEY             "xONNZA9yFvLZeEDQRh0CpaE2wAAKAZJT"
// OTA文件本地保存绝对路径，SDK会自动进行OTA管理
#define SOC_OTA_FILE        "/tmp/soc_upgrade.ota"



STATIC VOID __soc_dev_rev_upgrade_info_cb(IN CONST FW_UG_S *fw);// SOC设备升级入口
STATIC VOID __soc_dev_status_changed_cb(IN CONST GW_STATUS_E status);// SOC设备云端状态变更回调
STATIC VOID __soc_dev_dp_query_cb(IN CONST TY_DP_QUERY_S *dp_qry);// SOC设备特定数据查询入口
STATIC VOID __soc_dev_obj_dp_cmd_cb(IN CONST TY_RECV_OBJ_DP_S *dp);// SOC设备格式化指令数据下发入口
STATIC VOID __soc_dev_raw_dp_cmd_cb(IN CONST TY_RECV_RAW_DP_S *dp);// SOC设备透传指令数据下发入口
STATIC VOID __soc_dev_restart_req_cb(VOID);// SOC设备进程重启请求入口
STATIC VOID __soc_dev_net_status_cb(IN CONST GW_BASE_NW_STAT_T stat);// SOC外网状态变动回调

int main(void)
{
    printf("IOT SDK Version: %s \r\n", tuya_iot_get_sdk_info());
    printf("PRODUCT_KEY: %s \r\n", PRODUCT_KEY);
    printf("USER_SW_VER: %s \r\n", USER_SW_VER);
    printf("CFG_STORAGE_PATH: %s \r\n", CFG_STORAGE_PATH);
    printf("UUID: %s \r\n", UUID);
    printf("AUTHKEY: %s \r\n", AUTHKEY);

    OPERATE_RET op_ret = tuya_iot_init(CFG_STORAGE_PATH);
    if(OPRT_OK != op_ret) {
        PR_ERR("tuya_iot_init err:%d PATH:%s", op_ret, CFG_STORAGE_PATH);
        return -1;
    }
    PR_DEBUG("tuya_iot_init success");

    GW_PROD_INFO_S prod_info = {UUID, AUTHKEY};
    op_ret = tuya_iot_set_gw_prod_info(&prod_info);
    if(OPRT_OK != op_ret) {
        PR_ERR("tuya_iot_set_gw_prod_info err:%d", op_ret);
        return -2;
    }
    PR_DEBUG("tuya_iot_set_gw_prod_info success");

    TY_IOT_CBS_S iot_cbs = {
        __soc_dev_status_changed_cb,
        __soc_dev_rev_upgrade_info_cb,
        __soc_dev_restart_req_cb,
        __soc_dev_obj_dp_cmd_cb,
        __soc_dev_raw_dp_cmd_cb,
        __soc_dev_dp_query_cb,
        NULL,
    };
    op_ret = tuya_iot_soc_init(&iot_cbs, PRODUCT_KEY, USER_SW_VER);
    if(OPRT_OK != op_ret) {
        PR_ERR("tuya_iot_soc_init err:%d",op_ret);
        return -3;
    }
    PR_DEBUG("tuya_iot_soc_init success");

    op_ret = tuya_iot_reg_get_nw_stat_cb(__soc_dev_net_status_cb);
    if(OPRT_OK != op_ret) {
        PR_ERR("tuya_iot_reg_get_nw_stat_cb err:%d",op_ret);
        return -4;
    }
    PR_DEBUG("tuya_iot_reg_get_nw_stat_cb success");

    while (1)
    {
        sleep(1);
        INT_T local_reset = 0;
        // UserTODO

        if(local_reset)// 本地reset
        {
            OPERATE_RET ret = tuya_iot_gw_unactive();
            if(OPRT_OK != ret) {
                PR_ERR("tuya_iot_gw_unactive err:%d", ret);
                return -5;
            }
            PR_DEBUG("tuya_iot_gw_unactive success");
        }

    }

    return 0;
}

// SOC设备升级相关代码开始
STATIC VOID __upgrade_notify_cb(IN CONST FW_UG_S *fw, IN CONST INT_T download_result, IN PVOID_T pri_data)
{
    FILE *p_upgrade_fd = (FILE *)pri_data;
    fclose(p_upgrade_fd);

    if(download_result == 0) {
        PR_DEBUG("SOC Upgrade File Download Success");
        // UserTODO

    }else {
        PR_ERR("SOC Upgrade File Download Fail.ret = %d", download_result);
    }
}

STATIC OPERATE_RET __get_file_data_cb(IN CONST FW_UG_S *fw, IN CONST UINT_T total_len, IN CONST UINT_T offset,
                                      IN CONST BYTE_T *data, IN CONST UINT_T len, OUT UINT_T *remain_len, IN PVOID_T pri_data)
{
//    PR_DEBUG("Rev File Data");
//    PR_DEBUG("Total_len:%u", total_len);
//    PR_DEBUG("Offset:%u Len:%u", offset, len);
    FILE *p_upgrade_fd = (FILE *)pri_data;
    fwrite(data, 1, len, p_upgrade_fd);
    *remain_len = 0;

    return OPRT_OK;
}

// SOC设备升级入口
VOID __soc_dev_rev_upgrade_info_cb(IN CONST FW_UG_S *fw)
{
    PR_DEBUG("SOC Rev Upgrade Info");
    PR_DEBUG("fw->tp:%d", fw->tp);
    PR_DEBUG("fw->fw_url:%s", fw->fw_url);
    PR_DEBUG("fw->fw_md5:%s", fw->fw_md5);
    PR_DEBUG("fw->sw_ver:%s", fw->sw_ver);
    PR_DEBUG("fw->file_size:%u", fw->file_size);

    FILE *p_upgrade_fd = fopen(SOC_OTA_FILE, "w+b");
    if(NULL == p_upgrade_fd){
        PR_ERR("open upgrade file fail. upgrade fail %s", SOC_OTA_FILE);
        return;
    }
    OPERATE_RET op_ret = tuya_iot_upgrade_gw(fw, __get_file_data_cb, __upgrade_notify_cb, p_upgrade_fd);
    if(OPRT_OK != op_ret) {
        PR_ERR("tuya_iot_upgrade_gw err:%d",op_ret);
    }
}
// 升级相关代码结束

// SOC设备云端状态变更回调
VOID __soc_dev_status_changed_cb(IN CONST GW_STATUS_E status)
{
    PR_DEBUG("SOC TUYA-Cloud Status:%d", status);
}

// SOC设备特定数据查询入口
VOID __soc_dev_dp_query_cb(IN CONST TY_DP_QUERY_S *dp_qry)
{
    PR_DEBUG("SOC Rev DP Query Cmd");
    if(dp_qry->cid != NULL) PR_ERR("soc not have cid.%s", dp_qry->cid);

    if(dp_qry->cnt == 0) {
        PR_DEBUG("soc rev all dp query");
        // UserTODO
    }else {
        PR_DEBUG("soc rev dp query cnt:%d", dp_qry->cnt);
        UINT_T index = 0;
        for(index = 0; index < dp_qry->cnt; index++) {
            PR_DEBUG("rev dp query:%d", dp_qry->dpid[index]);
            // UserTODO
        }
    }
}

// SOC设备格式化指令数据下发入口
VOID __soc_dev_obj_dp_cmd_cb(IN CONST TY_RECV_OBJ_DP_S *dp)
{
    PR_DEBUG("SOC Rev DP Obj Cmd t1:%d t2:%d CNT:%u", dp->cmd_tp, dp->dtt_tp, dp->dps_cnt);
    if(dp->cid != NULL) PR_ERR("soc not have cid.%s", dp->cid);

    UINT_T index = 0;
    for(index = 0; index < dp->dps_cnt; index++)
    {
        TY_OBJ_DP_S *p_dp_obj = dp->dps + index;
        PR_DEBUG("idx:%d dpid:%d type:%d ts:%u", index, p_dp_obj->dpid, p_dp_obj->type, p_dp_obj->time_stamp);
        switch (p_dp_obj->type) {
        case PROP_BOOL:     { PR_DEBUG("bool value:%d", p_dp_obj->value.dp_bool); break;}
        case PROP_VALUE:    { PR_DEBUG("INT value:%d", p_dp_obj->value.dp_value); break;}
        case PROP_STR:      { PR_DEBUG("str value:%s", p_dp_obj->value.dp_str); break;}
        case PROP_ENUM:     { PR_DEBUG("enum value:%u", p_dp_obj->value.dp_enum); break;}
        case PROP_BITMAP:   { PR_DEBUG("bits value:0x%X", p_dp_obj->value.dp_bitmap); break;}
        default:            { PR_ERR("idx:%d dpid:%d type:%d ts:%u is invalid", index, p_dp_obj->dpid, p_dp_obj->type, p_dp_obj->time_stamp); break;}
        }// end of switch
    }
    // UserTODO

    // 用户处理完成之后需要主动上报最新状态，这里简单起见，直接返回收到的数据，认为处理全部成功。
    OPERATE_RET op_ret = dev_report_dp_json_async(dp->cid,dp->dps,dp->dps_cnt);
    if(OPRT_OK != op_ret) {
        PR_ERR("dev_report_dp_json_async op_ret:%d",op_ret);
    }
}

// SOC设备透传指令数据下发入口
VOID __soc_dev_raw_dp_cmd_cb(IN CONST TY_RECV_RAW_DP_S *dp)
{
    PR_DEBUG("SOC Rev DP Raw Cmd t1:%d t2:%d dpid:%d len:%u", dp->cmd_tp, dp->dtt_tp, dp->dpid, dp->len);
    if(dp->cid != NULL) PR_ERR("soc not have cid.%s", dp->cid);

    // UserTODO

    // 用户处理完成之后需要主动上报最新状态，这里简单起见，直接返回收到的数据，认为处理全部成功。
    OPERATE_RET op_ret = dev_report_dp_raw_sync(dp->cid,dp->dpid,dp->data,dp->len,0);
    if(OPRT_OK != op_ret) {
        PR_ERR("dev_report_dp_json_async op_ret:%d",op_ret);
    }
}

// SOC设备进程重启请求入口
VOID __soc_dev_restart_req_cb(VOID)
{
    PR_DEBUG("SOC Rev Restart Req");

    // UserTODO
}

// SOC外网状态变动回调
STATIC VOID __soc_dev_net_status_cb(IN CONST GW_BASE_NW_STAT_T stat)
{
    PR_DEBUG("network status:%d", stat);
}

