#ifndef _UNI_MUTEX_H
#define _UNI_MUTEX_H

#include "tuya_cloud_types.h"

#ifdef __cplusplus
extern "C" {
#endif

// 定义互斥量管理句柄
typedef PVOID_T MUTEX_HANDLE;

/**
 * @brief CreateMutexAndInit 创建一个互斥锁
 * @param pMutexHandle 创建的互斥锁句柄
 * @return
 */
OPERATE_RET CreateMutexAndInit(OUT MUTEX_HANDLE *pMutexHandle);

/**
 * @brief MutexLock 锁定一个互斥锁
 * @param mutexHandle 互斥锁句柄
 * @return
 */
OPERATE_RET MutexLock(IN CONST MUTEX_HANDLE mutexHandle);

/**
 * @brief MutexTryLock 尝试锁定一个互斥锁
 * @param mutexHandle 互斥锁句柄
 * @return
 */
OPERATE_RET MutexTryLock(IN CONST MUTEX_HANDLE mutexHandle);

/**
 * @brief MutexUnLock 解锁一个互斥锁
 * @param mutexHandle 互斥锁句柄
 * @return
 */
OPERATE_RET MutexUnLock(IN CONST MUTEX_HANDLE mutexHandle);

/**
 * @brief ReleaseMutex 释放一个互斥锁
 * @param mutexHandle 互斥锁句柄
 * @return
 */
OPERATE_RET ReleaseMutex(IN CONST MUTEX_HANDLE mutexHandle);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif
