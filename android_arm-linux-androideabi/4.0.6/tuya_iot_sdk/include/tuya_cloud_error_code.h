/*!
\file tuya_cloud_error_code.h
Copyright(C),2017, 涂鸦科技 www.tuya.comm
*/

#ifndef TUYA_CLOUD_ERROR_CODE_H
#define TUYA_CLOUD_ERROR_CODE_H

#ifdef __cplusplus
extern "C" {
#endif


typedef int OPERATE_RET; /*!<  操作结果返回值 */

#define OPRT_OK                             (0)         /*!< 执行成功 */
#define OPRT_COM_ERROR                      (-1)        /*!<  通用错误 */
#define OPRT_INVALID_PARM                   (-2)        /*!<  无效的入参 */
#define OPRT_MALLOC_FAILED                  (-3)        /*!<  内存分配失败 */
#define OPRT_NOT_SUPPORTED                  (-4)        /*!<  不支持 */
#define OPRT_NETWORK_ERROR                  (-5)        /*!<  内存分配失败 */


#define OPRT_INIT_MUTEX_ATTR_FAILED         (-101)      /*!<  初始化同步属性失败 */
#define OPRT_SET_MUTEX_ATTR_FAILED          (-102)      /*!<  设置同步属性失败 */
#define OPRT_DESTROY_MUTEX_ATTR_FAILED      (-103)      /*!<  销毁同步属性失败 */
#define OPRT_INIT_MUTEX_FAILED              (-104)      /*!<  初始化互斥量失败 */
#define OPRT_MUTEX_LOCK_FAILED              (-105)      /*!<  互斥量加锁失败 */
#define OPRT_MUTEX_TRYLOCK_FAILED           (-106)      /*!<  互斥量尝试加锁失败 */
#define OPRT_MUTEX_LOCK_BUSY                (-107)      /*!<  互斥量忙 */
#define OPRT_MUTEX_UNLOCK_FAILED            (-108)      /*!<  互斥量解锁失败 */
#define OPRT_MUTEX_RELEASE_FAILED           (-109)      /*!<  互斥量释放失败 */
#define OPRT_CR_MUTEX_ERR                   (-110)      /*!<  互斥量创建失败 */
#define OPRT_MEM_PARTITION_EMPTY            (-111)      /*!<  内存分区空 */
#define OPRT_MEM_PARTITION_FULL             (-112)      /*!<  内存分区满 */
#define OPRT_MEM_PARTITION_NOT_FOUND        (-113)      /*!<  内存分区不存在 */
#define OPRT_DONOT_FOUND_MODULE             (-114)      /*!<  未发现log模块 */


#define OPRT_INIT_SEM_FAILED                (-201)      /*!<  初始化信号量失败 */
#define OPRT_WAIT_SEM_FAILED                (-202)      /*!<  等待信号量失败 */
#define OPRT_POST_SEM_FAILED                (-203)      /*!<  释放信号量失败 */

#define OPRT_THRD_STA_UNVALID               (-301)      /*!<  线程状态非法 */
#define OPRT_THRD_CR_FAILED                 (-302)      /*!<  线程创建失败 */
#define OPRT_THRD_JOIN_FAILED               (-303)      /*!<  线程JOIN函数调用失败 */
#define OPRT_THRD_SELF_CAN_NOT_JOIN         (-304)      /*!<  自身线程不能调用JOIN函数 */

#define OPRT_TIMERID_EXIST                  (-401)      /*!<  定时器ID已存在 */
#define OPRT_TIMERID_NOT_FOUND              (-402)      /*!<  未找到指定定时器ID */
#define OPRT_TIMERID_UNVALID                (-403)      /*!<  定时器ID非法 */
#define OPRT_GET_IDLE_TIMERID_ERROR         (-404)      /*!<  获取空闲定时器ID错误 */

#define OPRT_MSG_NOT_FOUND                  (-501)      /*!<  未找到指定消息 */
#define OPRT_MSG_LIST_EMPTY                 (-502)      /*!<  消息链空 */

#define OPRT_WIFI_SCAN_FAIL                 (-601)
#define OPRT_WF_MAC_SET_FAIL                (-602)
#define OPRT_WF_CONN_FAIL                   (-603)
#define OPRT_WF_NW_CFG_FAIL                 (-604)
#define OPRT_WF_AP_SACN_FAIL                (-605)
#define OPRT_WF_NOT_FIND_ASS_AP             (-606)


#define OPRT_SOCK_ERR                       (-701)
#define OPRT_SET_SOCK_ERR                   (-702)
#define OPRT_SOCK_CONN_ERR                  (-703)
#define OPRT_BUF_NOT_ENOUGH                 (-704)
#define OPRT_URL_PARAM_OUT_LIMIT            (-705)
#define OPRT_HTTP_OS_ERROR                  (-706)
#define OPRT_HTTP_PR_REQ_ERROR              (-707)
#define OPRT_HTTP_SD_REQ_ERROR              (-708)
#define OPRT_HTTP_RD_ERROR                  (-709)
#define OPRT_HTTP_AD_HD_ERROR               (-710)
#define OPRT_HTTP_GET_RESP_ERROR            (-711)
#define OPRT_HTTP_AES_INIT_ERR              (-712)
#define OPRT_HTTP_AES_OPEN_ERR              (-713)
#define OPRT_HTTP_AES_SET_KEY_ERR           (-714)
#define OPRT_HTTP_AES_ENCRYPT_ERR           (-715)
#define OPRT_CR_HTTP_URL_H_ERR              (-716)
#define OPRT_HTTPS_HANDLE_FAIL              (-717)
#define OPRT_HTTPS_RESP_UNVALID             (-718)

#define OPRT_CR_CJSON_ERR                   (-801)
#define OPRT_CJSON_PARSE_ERR                (-802)
#define OPRT_CJSON_GET_ERR                  (-803)

#define OPRT_NOT_FOUND                  (-901)
#define OPRT_DP_ATTR_ILLEGAL                (-902)
#define OPRT_DP_TYPE_PROP_ILLEGAL           (-903)
#define OPRT_DP_REPORT_CLOUD_ERR            (-904)
#define OPRT_NO_NEED_SET_PRODINFO           (-905)
#define OPRT_NW_INVALID                     (-906)
#define OPRT_SELECT_ERR                     (-907)
#define OPRT_SELECT_TM                      (-908)
#define OPRT_SEND_ERR                       (-909)
#define OPRT_DEV_NOT_BIND                   (-910)
#define OPRT_FW_UG_FAILED                   (-911)
#define OPRT_VER_FMT_ERR                    (-912)
#define OPRT_FW_NOT_EXIST                   (-913)
#define OPRT_SEM_CR_ERR                     (-914)
#define OPRT_SELECT_TIMEOUT                 (-915)
#define OPRT_GW_MQ_OFFLILNE                 (-916)
#define OPRT_NOT_SPT_CLX_DP                 (-917)
#define OPRT_RECV_ERR                       (-918)
#define OPRT_UG_PKG_NOT_ENOUGH              (-919)
#define OPRT_SCMA_INVALID                   (-920)
#define OPRT_PRODECT_KEY_NULL               (-921)
#define OPRT_DEVICE_VER_NULL                (-922)
#define OPRT_MSG_OUT_OF_LMT                 (-923)
#define OPRT_NOT_FOUND_AUTH_SSID            (-924)
#define OPRT_SOCKET_FAULT                   (-925)
#define OPRT_MQ_PUBLISH_TIMEOUT             (-926)
#define OPRT_GW_NOT_EXIST                   (-927)
#define OPRT_GW_SCHEMA_SIZE_LMT_OUT         (-928)
#define OPRT_DEV_DP_CNT_INVALID             (-929)
#define OPRT_TOKEN_OVERTIME                 (-930)
#define OPRT_WF_NW_CFG_RECV_CONTINUE        (-931)
#define OPRT_RECV_DA_NOT_ENOUGH             (-932)
#define OPRT_SERV_VRFY_FAIL                 (-933)
#define OPRT_KVS_WR_FAIL                    (-934)
#define OPRT_KVS_RD_FAIL                    (-935)
#define OPRT_NO_AUTHENTICATION              (-936)
#define OPRT_CMD_OUT_OF_TIME_NOT_EXEC       (-937)
#define OPRT_CMD_NOT_EXEC                   (-938)
#define OPRT_CRC32_FAILED                   (-939)
#define OPRT_LAN_NO_CLIENT                  (-940)
#define OPRT_WF_INTF_LACK_REG_CB            (-941)
#define OPRT_WF_INTF_NO_REG_CB              (-942)
#define OPRT_STORAGE_UPLOAD                 (-943)
#define OPRT_DP_ID_NOT_FOUND                (-944)
#define OPRT_DP_TP_NOT_MATCH                (-945)
#define OPRT_DEV_NEED_REGISTER              (-946)

#define OPRT_OPEN_FILE_FAILED               (-1001)
#define OPRT_PRINT_LOG_LEVEL_HIGHER         (-1002)
#define OPRT_FORMAT_STRING_FAILED           (-1003)
#define OPRT_STRING_TOO_LONG                (-1004)
#define OPRT_WR_FLASH_ERROR                 (-1005)

#define OPRT_ROUTER_NOT_FIND                (-1006)

#define OPRT_OSS_OPERATION_INIT_FAILED      (-1100)
#define OPRT_OSS_OPERATION_UPDATE_FAILED    (-1101)
#define OPRT_OSS_OPERATION_UPLOAD_FAILED    (-1102)
#define OPRT_MOTION_DETECTION_ALARM_PACK_REPORT_FAILED      (-1103)
#define OPRT_TRANSFER_INIT_FAILED           (-1104)

//ipc
#define OPRT_EVENT_STATUS_INVALID           (-2000)
#define OPRT_ENCRYPT_KEY_UPDATED            (-2001)
#define OPRT_ORDER_EXPIRE                   (-2002)
#define OPRT_NO_MORE_DATA                   (-2003)



#ifdef __cplusplus
}
#endif
#endif

