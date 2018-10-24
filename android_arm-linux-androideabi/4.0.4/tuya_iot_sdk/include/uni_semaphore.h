#ifndef _UNI_SEMAPHORE_H
#define _UNI_SEMAPHORE_H

#include "tuya_cloud_types.h"

#ifdef __cplusplus
extern "C" {
#endif

// 定义信号量句柄
typedef PVOID_T SEM_HANDLE;

/**
 * @brief CreateAndInitSemaphore 创建一个信号量
 * @param pHandle 创建的一个信号量句柄
 * @param semCnt 信号量初始化值
 * @param sem_max 信号量最大值
 * @return
 */
OPERATE_RET CreateAndInitSemaphore(OUT SEM_HANDLE *pHandle, \
                                   IN CONST UINT_T semCnt, IN CONST UINT_T sem_max);

/**
 * @brief WaitSemaphore 等待一个信号量
 * @param semHandle 要等待的信号量句柄
 * @return
 */
OPERATE_RET WaitSemaphore(IN CONST SEM_HANDLE semHandle);

/**
 * @brief PostSemaphore 发送一个信号量
 * @param semHandle 要发送的信号量句柄
 * @return
 */
OPERATE_RET PostSemaphore(IN CONST SEM_HANDLE semHandle);

/**
 * @brief ReleaseSemaphore 释放一个信号量
 * @param semHandle 要释放的一个信号量句柄
 * @return
 */
OPERATE_RET ReleaseSemaphore(IN CONST SEM_HANDLE semHandle);


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif

