/***********************************************************
*  File: uni_queue.h // 通用队列实现
*  Author: nzy
*  Date: 121126
***********************************************************/
#ifndef _UNI_QUEUE_H
#define _UNI_QUEUE_H
#ifdef __cplusplus
	extern "C" {
#endif


#include "adapter_platform.h"
#include "uni_mutex.h"



#define QUEUE_SAFE_FUNC // 是否需要安全版本

#ifdef  __UNI_QUEUE_GLOBALS
    #define __UNI_QUEUE_EXT
#else
    #define __UNI_QUEUE_EXT extern
#endif

/***********************************************************
*************************micro define***********************
***********************************************************/
typedef struct{
    unsigned int queUnitSize; // 队列队员尺寸
    unsigned int queTolNum; // 队列可容纳的总的队员
    unsigned int queCurNum; // 当前队列中队员数
    
    unsigned char *queData; // 队列缓冲
    unsigned char *queIn; // 入队位置
    unsigned char *queOut; // 出对位置

    #ifdef QUEUE_SAFE_FUNC
    MUTEX_HANDLE mutex;
    #endif
}QUEUE_CLASS,*P_QUEUE_CLASS;


/***********************************************************
*************************variable define********************
***********************************************************/

/***********************************************************
*************************function define********************
***********************************************************/
/***********************************************************
*  Function: CreateQueueObj初始化
*  Input: queTolNum:队列可容纳的队员数
*         queUnitSize:队员尺寸
*  Output: none
*  Return: NULL:失败
*  Date: 121127
***********************************************************/
__UNI_QUEUE_EXT \
P_QUEUE_CLASS CreateQueueObj(const unsigned int queTolNum,\
                                    const unsigned int queUnitSize);


/***********************************************************
*  Function:RegisterQueueObj 队列数据缓冲以及队列管理对象缓冲来自外部注册
*  Input: pQueObj
*         queData
*         queTolNum:队列可容纳的队员数
*         queUnitSize:队员尺寸
*  Output: pQueObj
*  Return: 0:失败
*  Date: 121127

NOT USED !!!

***********************************************************/
__UNI_QUEUE_EXT \
unsigned char RegisterQueueObj(P_QUEUE_CLASS pQueObj,\
                               const unsigned char *pQueData,\
                               const unsigned int queTolNum,\
                               const unsigned int queUnitSize);

/***********************************************************
*  Function: InQueue 入队
*  Input: pQueObj:队列对象
*         pQueUnit:队员缓冲 queNum:队员数
*  Output: none
*  Return: 0:失败
*  Date: 121127
***********************************************************/
__UNI_QUEUE_EXT \
unsigned char InQueue(P_QUEUE_CLASS pQueObj, const unsigned char *pQueUnit,\
                         const unsigned int queNum);

/***********************************************************
*  Function:OutQueue 出队
*  Input: pQueObj:队列对象
*         queNum:出队员数
*  Output: pQueUnit:队员缓冲
*  Return: 0:失败
*  Date: 121127
***********************************************************/
__UNI_QUEUE_EXT \
unsigned char OutQueue(P_QUEUE_CLASS pQueObj,unsigned char *pQueUnit,\
                           const unsigned int queNum);

/***********************************************************
*  Function:GetCurFreeQueNum
*  Input: none
*  Output: none
*  Return: 当前队列可入队数
*  Date: 121127
***********************************************************/
__UNI_QUEUE_EXT \
unsigned int GetCurFreeQueNum(P_QUEUE_CLASS pQueObj);

/***********************************************************
*  Function:GetCurQueNum
*  Input: pQueObj
*  Output: none
*  Return: 当前入队成员数
*  Date: 121127
***********************************************************/
__UNI_QUEUE_EXT \
unsigned int GetCurQueNum(P_QUEUE_CLASS pQueObj);

/***********************************************************
*  Function:GetQueueMember 成员出队，但不删除成员
*  Input: pQueObj:队列对象 
*         start:从第几个成员开始获取 从一开始(包括获取自身)
*         pQueUnit:出队成员缓冲
*         queNum:出队员数
*  Output: pQueUnit:队员缓冲
*  Return: 0:失败
*  Date: 121127
***********************************************************/
__UNI_QUEUE_EXT \
unsigned char GetQueueMember(P_QUEUE_CLASS pQueObj,\
                                    const unsigned int start,\
                                    unsigned char *pQueUnit,\
                                    const unsigned int queNum);

/***********************************************************
*  Function:ClearQueue 清空队列
*  Input: pQueObj:队列对象
*         queNum:删除成员数目
*  Output: none
*  Return: 0:失败
*  Date: 121127
***********************************************************/
__UNI_QUEUE_EXT \
unsigned char ClearQueue(P_QUEUE_CLASS pQueObj);

/***********************************************************
*  Function:DelQueueMember 删除队列成员
*  Input: pQueObj:队列对象
*         queNum:删除成员数目
*  Output: none
*  Return: 0:失败
*  Date: 121127
***********************************************************/
__UNI_QUEUE_EXT \
unsigned char DelQueueMember(P_QUEUE_CLASS pQueObj,const unsigned int queNum);

/***********************************************************
*  Function: ReleaseQueueObj 释放
*  Input: pQueObj
*         
*  Output: none
*  Return: none
*  Date: 121127
***********************************************************/
__UNI_QUEUE_EXT \
void ReleaseQueueObj(P_QUEUE_CLASS pQueObj);


#ifdef __cplusplus
}
#endif
#endif

