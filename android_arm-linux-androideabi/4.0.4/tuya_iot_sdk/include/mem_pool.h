/***********************************************************
*  File: mem_pool.h
*  Author: nzy
*  Date: 20130106
***********************************************************/
#ifndef _MEM_POOL_H
#define _MEM_POOL_H

#include "tuya_cloud_types.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifdef _MEM_POOL_GLOBAL
    #define _MEM_POOL_EXT 
#else
    #define _MEM_POOL_EXT extern
#endif

/***********************************************************
*************************micro define***********************
***********************************************************/
#define SYS_MEM_DEBUG 0 // 系统内存调试
#define SHOW_MEM_POOL_DEBUG 1 // 内存池使用情况显示调试支持
#define MEM_POOL_MUTLI_THREAD 1 // 是否支持多线程处理

// 内存分区
typedef struct {            // MEMORY CONTROL BLOCK                                         
    VOID   *pMemAddr;       // Pointer to beginning of memory partition    
    VOID   *pMemAddrEnd;    // Pointer to ending of memory partition
    VOID   *pMemFreeList;   // Pointer to list of free memory blocks                        
    DWORD_T  memBlkSize;      // Size (in bytes) of each block of memory                      
    DWORD_T  memNBlks;        // Total number of blocks in this partition                     
    DWORD_T  memNFree;        // Number of memory blocks remaining in this partition          
}MEM_PARTITION;

// 内存池建立表
typedef struct {
    DWORD_T  memBlkSize; // 块大小 >=2
    DWORD_T  memNBlks; // 块数量
}MEM_POOL_SETUP_TBL;
/***********************************************************
*************************variable define********************
***********************************************************/

/***********************************************************
*************************function define********************
***********************************************************/
/***********************************************************
*  Function: SysMemoryPoolSetup 建立系统内存池
*  Input: none
*  Output: none
*  Return: oprt->结果集
*  Date: 20130106
***********************************************************/
_MEM_POOL_EXT \
OPERATE_RET SysMemoryPoolSetup(VOID);

/***********************************************************
*  Function: SysMemPoolSetupUserDef 
*  Input: none
*  Output: none
*  Return: oprt->结果集
***********************************************************/
_MEM_POOL_EXT \
OPERATE_RET SysMemPoolSetupUserDef(IN CONST MEM_POOL_SETUP_TBL *tbl,\
                                              IN CONST INT_T cnt);

/***********************************************************
*  Function: MemPartitionCreate 创建内存分区
*  Input: nblks:内存块数 blksize:块尺寸
*  Output: ppMemPartition:内存分区存储缓冲,注意:如ppMemPartition为NULL，
*  则生成的内存分区归系统管理，否则由创建者自己管理
*  Return: oprt->结果集
*  Date: 20130106
***********************************************************/
_MEM_POOL_EXT \
OPERATE_RET MemPartitionCreate(IN CONST DWORD_T nblks, \
                               IN CONST DWORD_T blksize,\
                               OUT MEM_PARTITION **ppMemPartition);

/***********************************************************
*  Function: MemBlockGet 获取内存块
*  Input: pMemPart:内存分区
*  Output: pRet:结果集
*  Return: 内存块地址
*  Date: 20130106
***********************************************************/
_MEM_POOL_EXT \
VOID *MemBlockGet(INOUT MEM_PARTITION *pMemPart,\
                  OUT OPERATE_RET *pOprtRet);

/***********************************************************
*  Function: MemBlockPut 归还内存块
*  Input: pMemPart:内存分区
*  Output: pRet:结果集
*  Return: 内存块地址
*  Date: 20130106
***********************************************************/
_MEM_POOL_EXT \
OPERATE_RET MemBlockPut(INOUT MEM_PARTITION *pMemPart,\
                        IN CONST VOID *pblk);

/***********************************************************
*  Function: MemPartitionDelete 内存分区删除
*  Input: pMemPart:内存分区
*  Output: none
*  Return: oprt->结果集
*  Date: 20130106
***********************************************************/
_MEM_POOL_EXT \
OPERATE_RET MemPartitionDelete(IN MEM_PARTITION *pMemPart);

/***********************************************************
*  Function: SysMemoryPoolDelete 删除系统内存池
*  Input: none
*  Output: none
*  Return: oprt->结果集
*  Date: 20130107
***********************************************************/
_MEM_POOL_EXT \
VOID SysMemoryPoolDelete(VOID);

/***********************************************************
*  Function: MallocFromSysMemPool 由系统内存池中申请内存
*  Input: reqSize:需要分配的内存 
*  Output: pRet:返回值结果集
*  Return: 申请的内存地址
*  Date: 20130107
***********************************************************/
_MEM_POOL_EXT \
VOID *MallocFromSysMemPool(IN CONST DWORD_T reqSize,OPERATE_RET *pRet);

/***********************************************************
*  Function: FreeFromSysMemPool 归还内存快至内存池
*  Input: pReqMem:需要归还的内存 
*  Output: none
*  Return: opRt:返回值结果集
*  Date: 20130107
***********************************************************/
_MEM_POOL_EXT \
OPERATE_RET FreeFromSysMemPool(IN PVOID_T pReqMem);

/***********************************************************
*  Function: ShowSysMemPoolInfo
*  Input: none 
*  Output: none
*  Return: opRt:返回值结果集
*  Date: 20130107
***********************************************************/
#if SHOW_MEM_POOL_DEBUG
_MEM_POOL_EXT \
VOID ShowSysMemPoolInfo(VOID);
#endif

#if SYS_MEM_DEBUG
VOID *__Malloc(IN CONST DWORD_T reqSize,IN CONST CHAR_T *file,IN CONST INT_T line);
VOID __Free(IN PVOID_T pReqMem,IN CONST CHAR_T *file,IN CONST INT_T line);
#define Malloc(x) __Malloc(x,__FUNCTION__,__LINE__)
#define Free(x) __Free(x,__FUNCTION__,__LINE__)
#else
/***********************************************************
*  Function: Malloc 内存申请
*  Input: reqSize->申请的内存大小
*  Output: none
*  Return: 失败NULL 成功返回内存地址
*  Date: 120427
***********************************************************/
_MEM_POOL_EXT \
VOID *Malloc(IN SIZE_T reqSize);

/***********************************************************
*  Function: Free 安全内存释放
*  Input: ppReqMem
*  Output: none
*  Return: none
*  Date: 120427
***********************************************************/
_MEM_POOL_EXT \
VOID Free(IN PVOID_T pReqMem);
#endif


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif



