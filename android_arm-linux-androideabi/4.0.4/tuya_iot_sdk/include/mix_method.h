/***********************************************************
*  File: mix_method.h
*  Author: nzy
*  Date: 20160826
***********************************************************/
#ifndef _MIX_METHOD_H
    #define _MIX_METHOD_H

#include "adapter_platform.h"
#include "tuya_cloud_error_code.h"

    
#ifdef __cplusplus
    extern "C" {
#endif
    
#ifdef  __MIX_METHOD_GLOBALS
    #define __MIX_METHOD_EXT
#else
    #define __MIX_METHOD_EXT extern
#endif
    
/***********************************************************
*************************micro define***********************
***********************************************************/
#define STRCASE_CMP

/***********************************************************
*************************variable define********************
***********************************************************/

/***********************************************************
*************************function define********************
***********************************************************/
#ifndef STRCASE_CMP
__MIX_METHOD_EXT \
int strcasecmp(const char *s1, const char *s2);
#endif

__MIX_METHOD_EXT \
int tuya_strncasecmp(const char *s1, const char *s2, size_t n);
    
__MIX_METHOD_EXT \
unsigned char asc2hex(char asccode);

__MIX_METHOD_EXT \
void ascs2hex(unsigned char *hex,unsigned char *ascs,int srclen);

__MIX_METHOD_EXT \
void hex2str(unsigned char *pbDest, unsigned char *pbSrc, int nLen);

// input: str->string
//        revr_index->reverse index,start from 0
//        ch->find char
// return: find position
__MIX_METHOD_EXT \
int str_revr_find_ch(const char *str,const int revr_index,const char ch);

__MIX_METHOD_EXT \
void byte_sort(unsigned char is_ascend,unsigned char *buf,int buf_cnt);

__MIX_METHOD_EXT \
char *mm_strdup(char *str);

// ver is xx.xx.xx
__MIX_METHOD_EXT \
int is_valid_version(char *ver);

#ifdef __cplusplus
}
#endif
#endif

