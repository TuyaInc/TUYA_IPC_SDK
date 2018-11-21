/*********************************************************************************
  *Copyright(C),2017, 涂鸦科技 www.tuya.comm
  *FileName:    tuya_cloud_types.h
**********************************************************************************/
#ifndef TUYA_CLOUD_TYPES_H
#define TUYA_CLOUD_TYPES_H

#ifdef __cplusplus
extern "C" {
#endif

#include <time.h>
#include <stddef.h>
#include "tuya_cloud_error_code.h"

typedef int INT32;
typedef unsigned int UINT32;

typedef long long DLONG;
typedef DLONG *PDLONG;
typedef unsigned long long DDWORD;
typedef DDWORD *PDDWORD;
typedef float FLOAT;
typedef FLOAT *PFLOAT;
typedef signed int INT;
typedef int *PINT;
typedef void *PVOID;
typedef char CHAR;
typedef signed char SCHAR;
typedef unsigned char UCHAR;
typedef char *PCHAR;
typedef short SHORT;
typedef unsigned short USHORT;
typedef short *PSHORT;
typedef long LONG;
typedef unsigned long ULONG;
typedef long *PLONG;
typedef unsigned char BYTE;
typedef BYTE *PBYTE;
typedef unsigned short WORD;
typedef WORD *PWORD;
typedef unsigned int DWORD;
typedef DWORD *PDWORD;
typedef unsigned int UINT;
typedef unsigned int *PUINT;
typedef unsigned int BOOL;
typedef BOOL *PBOOL;

typedef long long int INT64;
typedef INT64 *PINT64;

typedef unsigned long long int UINT64;
typedef UINT64 *PUINT64;

typedef short INT16;
typedef INT64 *PINT16;

typedef unsigned short UINT16;
typedef UINT64 *PUINT16;

typedef unsigned char UINT8;
typedef UINT8 *PUINT8;

typedef ULONG TIME_MS;
typedef ULONG TIME_S;
typedef time_t TIME_T;

#ifndef FALSE
#define FALSE 0
#endif

#ifndef TRUE
#define TRUE 1
#endif

#ifndef IN
#define IN
#endif

#ifndef OUT
#define OUT
#endif

#ifndef INOUT
#define INOUT
#endif

#ifndef VOID
#define VOID void
#endif

#ifndef CONST
#define CONST const
#endif

#ifndef STATIC
#define STATIC static
#endif

#ifndef SIZEOF
#define SIZEOF sizeof
#endif

#ifndef INLINE
#define INLINE inline
#endif

#ifndef LITTLE_END
#define LITTLE_END // 主机为小端模式
#endif

#ifndef NULL
    #ifdef __cplusplus
    #define NULL 0
    #else
    #define NULL ((void *)0)
    #endif
#endif


//#ifndef bool
//typedef int bool;
//#endif

#ifndef true
#define true 1
#endif

#ifndef false
#define false 0
#endif

typedef size_t SIZE_T; // result of the sizeof

typedef enum {
    DP_TYPE_BOOL = 0,
    DP_TYPE_VALUE,
    DP_TYPE_ENUM,
    DP_TYPE_STRING,
    DP_TYPE_INVALID
}TUYA_DP_TYPE_E;

typedef struct {
    INT cmd;
    TUYA_DP_TYPE_E type;
}TUYA_DP_CMD_S;

/* 常用操作宏定义 */
#ifndef MAX
#define MAX(a,b) (((a) > (b)) ? (a) : (b))
#endif

#ifndef MIN
#define MIN(a,b) (((a) < (b)) ? (a) : (b))
#endif

#ifndef BIT
#define BIT(nr)     (1UL << (nr))
#endif

#define MAKEWORD(a, b) ((WORD)(((BYTE)(a)) | ((WORD)((BYTE)(b))) << 8))
#define MAKELONG(a, b) ((LONG)(((WORD)(a)) | ((DWORD)((WORD)(b))) << 16))
#define LOWORD(l) ((WORD)(l))
#define HIWORD(l) ((WORD)(((DWORD)(l) >> 16) & 0xFFFF))
#define LOBYTE(w) ((BYTE)(w))
#define HIBYTE(w) ((BYTE)(((WORD)(w) >> 8) & 0xFF))

/* 字节序置换 */
#define WORD_SWAP(X) ((X << 8) | (X >> 8))
#define DWORD_SWAP(X)(((X)&0xff)<<24) + \
                     (((X)&0xff00)<<8) + \
                     (((X)&0xff0000)>>8) + \
                     (((X)&0xff000000)>>24)
#define DDWORD_SWAP(X) { \
    unsigned long temp_low, temp_high; \
    temp_low = DWORD_SWAP((unsigned long)((X) & 0xffffffff)); \
    temp_high = DWORD_SWAP((unsigned long)((X) >> 32)); \
    X = 0; \
    X |= temp_low; \
    X <<= 32; \
    X |= temp_high;\
}
#ifdef LITTLE_END
#define TY_NTOHS(X) WORD_SWAP(X)
#define TY_HTONS(X) WORD_SWAP(X)
#define TY_NTOHL(X) DWORD_SWAP(X)
#define TY_HTONL(X) DWORD_SWAP(X)
#define TY_NTOHLL(X) DDWORD_SWAP(X)
#define TY_HTONLL(X) DDWORD_SWAP(X)
#else
#define TY_NTOHS(X) X
#define TY_HTONS(X) X
#define TY_NTOHL(X) X
#define TY_HTONL(X) X
#define TY_NTOHLL(X) X
#define TY_HTONLL(X) X
#endif

#define OFFSOF(s,m) ((SIZE_T)(&(((s*)0)->m)))   // 结构体成员偏移宏
#define CNTSOF(a)   (sizeof(a)/sizeof(a[0]))    // 数组元素的个数宏 
#define CNTR_OF(ptr, type, member) \
        ({(type *)( (char *)ptr - OFFSOF(type,member) );}) // continer of


#ifdef __cplusplus
}
#endif

#endif // TUYA_CLOUD_TYPES_H
