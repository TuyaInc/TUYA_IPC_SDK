/***********************************************************
*  File: uni_output.h
*  Author: nzy
*  Date: 20170921
***********************************************************/
#ifndef _UNI_OUTPUT_H
#define _UNI_OUTPUT_H

#include "tuya_cloud_types.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifdef _UNI_OUTPUT_GLOBAL
    #define _UNI_OUTPUT_EXT 
#else
    #define _UNI_OUTPUT_EXT extern
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
*  Function: uni_output_init 
*  Input: none
*  Output: none
*  Return: OPERATE_RET
***********************************************************/
_UNI_OUTPUT_EXT \
OPERATE_RET uni_output_init(VOID);

/***********************************************************
*  Function: uni_log_output 
*  Input: str
*  Output: none
*  Return: none
***********************************************************/
_UNI_OUTPUT_EXT \
VOID uni_log_output(IN CONST CHAR_T *str);


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif


