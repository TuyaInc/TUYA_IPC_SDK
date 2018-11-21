#ifndef _UNI_THREAD_H
#define _UNI_THREAD_H

#include "tuya_cloud_types.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifdef  __UNI_THREAD_GLOBAL
    #define __UNI_THREAD_EXT
#else
    #define __UNI_THREAD_EXT extern
#endif


// 线程执行状态
typedef INT_T THRD_STA;
#define STATE_EMPTY 0 
#define STATE_RUNNING 1 
#define STATE_STOP 2 

// 线程执行函数
typedef VOID (*P_THRD_FUNC)(PVOID_T pArg);

// 定义线程管理句柄
typedef PVOID_T THRD_HANDLE;
typedef VOID(*P_CONSTRUCT_FUNC)(VOID); // thread construct
typedef VOID(*P_EXTRACT_FUNC)(VOID); // thread extract

typedef unsigned int TRD_PRI;
#define TRD_PRIO_0     5    /** High **/
#define TRD_PRIO_1     4
#define TRD_PRIO_2     3
#define TRD_PRIO_3     2
#define TRD_PRIO_4     1
#define TRD_PRIO_5     0
#define TRD_PRIO_6     0


typedef unsigned short STACK_SIZE;
typedef struct {
    STACK_SIZE stackDepth;
    TRD_PRI priority;
    CHAR_T *thrdname;
}THRD_PARAM_S;

/***********************************************************
*  Function: CreateAndStart 创建并正常运行，在其他线程中可进行JOIN处理
*  Input: enter->construct
*         exit->extract
*         pThrdFunc->线程执行函数
*         pThrdFuncArg->线程执行函数指针
*         thrd_param-> yes/none
*  Output: pThrdHandle->生成的线程句柄
*  Return: none
***********************************************************/
__UNI_THREAD_EXT \
OPERATE_RET CreateAndStart(OUT THRD_HANDLE *pThrdHandle,\
                           IN CONST P_CONSTRUCT_FUNC enter,\
                           IN CONST P_EXTRACT_FUNC exit,\
                           IN CONST P_THRD_FUNC pThrdFunc,\
                           IN CONST PVOID_T pThrdFuncArg,\
                           IN CONST THRD_PARAM_S *thrd_param);

/***********************************************************
*  Function: ThreadRunSelfSpace
*  Input: thrdHandle->线程句柄 
*  Output: bl
*  Return: OPERATE_RET
***********************************************************/
__UNI_THREAD_EXT
OPERATE_RET ThreadRunSelfSpace(IN CONST THRD_HANDLE thrdHandle,OUT BOOL_T *bl);

/***********************************************************
*  Function: GetThrdSta
*  Input: thrdHandle->线程句柄
*  Output: none
*  Return: THRD_STA
***********************************************************/
__UNI_THREAD_EXT \
THRD_STA GetThrdSta(IN CONST THRD_HANDLE thrdHandle);

/***********************************************************
*  Function: DeleteThrdHandle
*  Input: thrdHandle->线程句柄  
*  Output: none
*  Return: OPERATE_RET
***********************************************************/
__UNI_THREAD_EXT \
OPERATE_RET DeleteThrdHandle(IN CONST THRD_HANDLE thrdHandle);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif

