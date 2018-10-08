/*
tuya_cloud_base_defs.h
Copyright(C),2018-2020, 涂鸦科技 www.tuya.comm
*/

#ifndef TUYA_CLOUD_BASE_DEFS_H
#define TUYA_CLOUD_BASE_DEFS_H

#include "tuya_cloud_types.h"

#ifdef __cplusplus
extern "C" {
#endif

// curr network connection status
typedef BYTE_T GW_BASE_NW_STAT_T;
#define GB_STAT_LAN_UNCONN 0  // offline in LAN
#define GB_STAT_LAN_CONN 1    // online in LAN But offline in WAN
#define GB_STAT_CLOUD_CONN 2  // online in WAN

typedef struct {
    CHAR_T *uuid;        // strlen(uuid) <= 16,must not be null
    CHAR_T *auth_key;    // strlen(auth_key) <= 32,must not be null
}GW_PROD_INFO_S;

typedef VOID (*GET_NW_STAT_CB)(IN CONST GW_BASE_NW_STAT_T stat);

#ifdef __cplusplus
}
#endif

#endif
