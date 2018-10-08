/***********************************************************
*  File: uni_system.h
*  Author: nzy
*  Date: 120427
***********************************************************/
#ifndef _UNI_SYSTEM_H
#define _UNI_SYSTEM_H

#include "tuya_cloud_types.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifdef _UNI_SYSTEM_GLOBAL
    #define _UNI_SYSTEM_EXT
#else
    #define _UNI_SYSTEM_EXT extern
#endif

/***********************************************************
*  Function: GetSystemTickCount 
*  Input: none
*  Output: none
*  Return: system tick count
***********************************************************/
_UNI_SYSTEM_EXT \
UINT_T GetSystemTickCount(VOID);

/***********************************************************
*  Function: GetTickRateMs 
*  Input: none
*  Output: none
*  Return: tick rate spend how many ms
***********************************************************/
_UNI_SYSTEM_EXT \
UINT_T GetTickRateMs(VOID);

/***********************************************************
*  Function: SystemSleep
*  Input: none
*  Output: none
*  Return: system sleep ms
***********************************************************/
_UNI_SYSTEM_EXT \
VOID SystemSleep(IN CONST TIME_MS msTime);

/***********************************************************
*  Function: SystemIsrStatus->direct system interrupt status
*  Input: none
*  Output: none 
*  Return: BOOL_T
***********************************************************/
_UNI_SYSTEM_EXT \
BOOL_T SystemIsrStatus(VOID);

/***********************************************************
*  Function: SystemReset 
*  Input: msTime
*  Output: none 
*  Return: none
*  Date: 120427
***********************************************************/
_UNI_SYSTEM_EXT \
VOID SystemReset(VOID);

/***********************************************************
*  Function: SysMalloc 
*  Input: reqSize
*  Output: none 
*  Return: VOID *
***********************************************************/
_UNI_SYSTEM_EXT \
VOID *SysMalloc(IN CONST DWORD_T reqSize);

/***********************************************************
*  Function: SysFree 
*  Input: msTime
*  Output: none 
*  Return: none
***********************************************************/
_UNI_SYSTEM_EXT \
VOID SysFree(IN PVOID_T pReqMem);

/***********************************************************
*  Function: SysGetHeapSize 
*  Input: none
*  Output: none 
*  Return: INT_T-> <0 means don't support to get heapsize
***********************************************************/
_UNI_SYSTEM_EXT \
INT_T SysGetHeapSize(VOID);

/***********************************************************
*  Function: GetSerialNo 
*  Input: none
*  Output: none 
*  Return: CHAR_T *->serial number
***********************************************************/
_UNI_SYSTEM_EXT \
CHAR_T *GetSerialNo(VOID);

/***********************************************************
*  Function: system_get_rst_info 
*  Input: none
*  Output: none 
*  Return: CHAR_T *->reset reason
***********************************************************/
_UNI_SYSTEM_EXT \
CHAR_T *system_get_rst_info(VOID);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif

