/*********************************************************************************
  *Copyright(C),2015-2020, 涂鸦科技 www.tuya.comm
  *FileName: tuya_linux_wifi.c
  *
  * 文件描述：
  * 1. WIFI操作API实现
  *
  * 开发者工作：
  * 1. 根据SSID和PASSWD连接WIFI。
  * 2. 抓取图片并且识别二维码。
  * 3. 特定WIFI芯片适配。
  *
**********************************************************************************/

#include "tuya_linux_wifi.h"
#include "tuya_ipc_mgr_handler.h"
#include "tuya_iot_wifi_api.h"
#include "cJSON.h"
#include <stdio.h>
#include <stdlib.h>

#include <string.h>
#include <unistd.h>

#include <net/if.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <pthread.h>

#define WLAN_DEV    "wlp3s0"
//#define WLAN_DEV    "wlan0"

//#define WIFI_CHIP_7601

/*
用户需要实现的接口：
hwl_wf_station_connect
__tuya_linux_get_snap_qrcode
其他接口理论上均可以直接使用，如果有用户有自己的实现可以替换掉
*/
static BOOL_T sniffer_set_done = FALSE;
OPERATE_RET hwl_wf_station_connect(IN CONST CHAR_T *ssid,IN CONST CHAR_T *passwd)
{
    if(sniffer_set_done)
    {
        sniffer_set_done = FALSE;
        IPC_APP_Notify_LED_Sound_Status_CB(IPC_REV_WIFI_CFG);
        usleep(1000*1000);
    }

    IPC_APP_Notify_LED_Sound_Status_CB(IPC_CONNECTING_WIFI);

    if(NULL == ssid)
    {
        return OPRT_INVALID_PARM;
    }

    printf("STA Con AP ssid:%s passwd:%s\r\n", ssid, passwd);

    //最多阻塞10s

    sleep(2);

    return OPRT_OK;
}

STATIC CHAR_T* __tuya_linux_get_snap_qrcode(VOID)
{
    return "1234";
}

//==================================================
/* 以下代码为工具代码，用户可以不用关心 */
/* 如果用户有自定义实现，可以自行修改。 */
//==================================================

STATIC OPERATE_RET __tuya_ipc_qrcode_direct_connect(IN CONST CHAR_T *p_str)
{
    static INT_T flag = 0;
    CHAR_T *p_ssid = NULL;
    CHAR_T *p_passwd = NULL;
    CHAR_T *p_token = NULL;

    if(flag == 1)
    {
        return OPRT_OK;
    }

    cJSON *root = cJSON_Parse(p_str);
    if(NULL == root)
    {
        return OPRT_CJSON_PARSE_ERR;
    }

    if( NULL == (cJSON_GetObjectItem(root,"s")) || NULL == (cJSON_GetObjectItem(root,"t")) )
    {
        cJSON_Delete(root);
        return OPRT_COM_ERROR;
    }

    p_ssid = cJSON_GetObjectItem(root,"s")->valuestring;
    p_token = cJSON_GetObjectItem(root,"t")->valuestring;
    printf("Parse ssid:%s token:%s \r\n",p_ssid, p_token);

    if((cJSON_GetObjectItem(root,"p") && \
       (cJSON_GetObjectItem(root,"p")->valuestring[0])))
    {
        p_passwd = cJSON_GetObjectItem(root,"p")->valuestring;
        printf("Parse passwd:%s \r\n", p_passwd);
    }

    tuya_iot_gw_wf_user_cfg(p_ssid, p_passwd, p_token);

    cJSON_Delete(root);
    flag = 1;
    return OPRT_OK;
}



#define MAX_AP_SEARCH 20
static void exec_cmd(char *pCmd)
{
    printf("Exec Cmd:%s \r\n", pCmd);
    system(pCmd);
}

/*
wlp3s0    Scan completed :
          Cell 01 - Address: 94:77:2B:0A:96:10
                    Channel:1
                    Frequency:2.412 GHz (Channel 1)
。。。。。。。
。。。。。。。

*/
OPERATE_RET hwl_wf_all_ap_scan(OUT AP_IF_S **ap_ary,OUT UINT_T *num)
{
    if(NULL == ap_ary || NULL == num)
    {
        return OPRT_INVALID_PARM;
    }

    static AP_IF_S s_aps[MAX_AP_SEARCH];

    memset(s_aps, 0, sizeof(s_aps));
    *ap_ary = s_aps;
    *num = 0;

    FILE *pp = popen("iwlist "WLAN_DEV" scan", "r");
    if(pp == NULL)
    {
        printf("popen fails\r\n");
        return OPRT_COM_ERROR;
    }

    char tmp[256] = {0};
    memset(tmp, 0, sizeof(tmp));

    int recordIdx = -1;
    while (fgets(tmp, sizeof(tmp), pp) != NULL)
    {
        /* 首先找BSSID	  以此为基准 */
        char *pBSSIDStart = strstr(tmp, " - Address: ");
        if(pBSSIDStart != NULL)
        {
            recordIdx++;
            if(recordIdx >= MAX_AP_SEARCH)
            {
                printf(" Reach Max Record \r\n");
                recordIdx--;
                break;
            }

            BYTE_T *pTmp = s_aps[recordIdx].bssid;
            int x1,x2,x3,x4,x5,x6;

            sscanf(pBSSIDStart + strlen(" - Address: "), "%x:%x:%x:%x:%x:%x",&x1,&x2,&x3,&x4,&x5,&x6);
            pTmp[0] = x1 & 0xFF;
            pTmp[1] = x2 & 0xFF;
            pTmp[2] = x3 & 0xFF;
            pTmp[3] = x4 & 0xFF;
            pTmp[4] = x5 & 0xFF;
            pTmp[5] = x6 & 0xFF;

            goto ReadNext;
        }else
        {
            if(recordIdx < 0)
            {/* 只有找到第一个BSSID 才能继续读下去 */
                goto ReadNext;
            }
        }

        {
            /* 查找signal  */
            char *pSIGNALStart = strstr(tmp, "Quality=");
            if(pSIGNALStart != NULL)
            {
                int x = 0;
                int y = 0;
                sscanf(pSIGNALStart + strlen("Quality=") , "%d/%d ",&x,&y);
                s_aps[recordIdx].rssi = x * 100/ (y+1);
                goto ReadNext;
            }
        }

        {
            /* 查找channel	*/
            char *pCHANNELStart = strstr(tmp, "(Channel ");
            if(pCHANNELStart != NULL)
            {
                int x = 0;
                sscanf(pCHANNELStart + strlen("(Channel "), "%d)", &x);
                s_aps[recordIdx].channel = x;
                goto ReadNext;
            }
        }

        {
            /* 查找ssid  */
            char *pSSIDStart = strstr(tmp, "ESSID:\"");
            if(pSSIDStart != NULL)
            {
                sscanf(pSSIDStart + strlen("ESSID:\""), "%s", s_aps[recordIdx].ssid);
                s_aps[recordIdx].s_len = strlen(s_aps[recordIdx].ssid);
                if(s_aps[recordIdx].s_len != 0)
                {
                    s_aps[recordIdx].ssid[s_aps[recordIdx].s_len - 1] = 0;
                    s_aps[recordIdx].s_len--;
                }
                goto ReadNext;
            }
        }

ReadNext:
        memset(tmp, 0, sizeof(tmp));
    }

    pclose(pp);
    *num = recordIdx + 1;

    printf("WIFI Scan AP Begin\r\n");
    int index = 0;
    for(index = 0; index < *num; index++)
    {
        printf("index:%d bssid:%02X-%02X-%02X-%02X-%02X-%02X RSSI:%d SSID:%s\r\n",
               index, s_aps[index].bssid[0],  s_aps[index].bssid[1],  s_aps[index].bssid[2],  s_aps[index].bssid[3],
                s_aps[index].bssid[4],  s_aps[index].bssid[5], s_aps[index].rssi, s_aps[index].ssid);
    }
    printf("WIFI Scan AP End\r\n");

    return OPRT_OK;
}


OPERATE_RET hwl_wf_assign_ap_scan(IN CONST CHAR_T *ssid,OUT AP_IF_S **ap)
{
    if(NULL == ssid || NULL == ap)
    {
        return OPRT_INVALID_PARM;
    }

    /*
    这里简单处理，扫描所有ap后进行查找
    */
    AP_IF_S *pTotalAp = NULL;
    UINT_T tatalNum = 0;
    int index = 0;
    hwl_wf_all_ap_scan(&pTotalAp, &tatalNum);

    *ap = NULL;

    for(index = 0; index < tatalNum; index++)
    {
        if(memcmp(pTotalAp[index].ssid, ssid, pTotalAp[index].s_len) == 0)
        {
            *ap = pTotalAp + index;
            break;
        }
    }

    return OPRT_OK;
}

OPERATE_RET hwl_wf_release_ap(IN AP_IF_S *ap)
{//静态变量，无需释放
    return OPRT_OK;
}

static int s_curr_channel = 1;
OPERATE_RET hwl_wf_set_cur_channel(IN CONST BYTE_T chan)
{
    char tmpCmd[100] = {0};
    snprintf(tmpCmd, 100, "iwconfig %s channel %d", WLAN_DEV, chan);
    exec_cmd(tmpCmd);
    s_curr_channel = chan;

    printf("WIFI Set Channel:%d \r\n", chan);

#ifdef WIFI_CHIP_7601
    tuya_linux_wf_wk_mode_set(WWM_SNIFFER);
#endif

    return OPRT_OK;
}

/*
wlp3s0    13 channels in total; available frequencies :
          Channel 01 : 2.412 GHz
。。。。。。
。。。。。。
          Channel 13 : 2.472 GHz
          Current Frequency:2.452 GHz (Channel 9)
*/
OPERATE_RET hwl_wf_get_cur_channel(OUT BYTE_T *chan)
{
    if(NULL == chan) {
        return OPRT_INVALID_PARM;
    }

    FILE *pp = popen("iwlist "WLAN_DEV" channel", "r");

    if(pp == NULL)
    {
       return OPRT_COM_ERROR;
    }

    char tmp[128] = {0};
    memset(tmp, 0, sizeof(tmp));
    while (fgets(tmp, sizeof(tmp), pp) != NULL)
    {
        char *pIPStart = strstr(tmp, " (Channel ");
        if(pIPStart != NULL)
        {
            break;
        }
    }

    /* 查找channel	*/
    char *pCHANNELStart = strstr(tmp, " (Channel ");
    if(pCHANNELStart != NULL)
    {
        int x = 0;
        sscanf(pCHANNELStart + strlen(" (Channel "), "%d", &x);
        *chan = x;
    }else
    {
        *chan = s_curr_channel;
    }

    pclose(pp);

    printf("WIFI Get Curr Channel:%d \r\n", *chan);

    return OPRT_OK;
}


#pragma pack(1)
/*
http://www.radiotap.org/
*/
typedef struct {
    /**
     * @it_version: radiotap version, always 0
     */
    BYTE_T it_version;

    /**
     * @it_pad: padding (or alignment)
     */
    BYTE_T it_pad;

    /**
     * @it_len: overall radiotap header length
     */
    USHORT_T it_len;

    /**
     * @it_present: (first) present word
     */
    UINT_T it_present;
}ieee80211_radiotap_header;
#pragma pack()

static volatile SNIFFER_CALLBACK s_pSnifferCall = NULL;
static volatile int s_enable_sniffer = 0;

static void * func_Sniffer(void *pReserved)
{
    printf("Sniffer Thread Create\r\n");

    int sock = socket(PF_PACKET, SOCK_RAW, htons(0x03));//ETH_P_ALL
    if(sock < 0)
    {
        printf("Sniffer Socket Alloc Fails %d \r\n", sock);
        perror("Sniffer Socket Alloc");
        return (void *)0;
    }

    {/* 强制绑定到wlan0 上。后续可以考虑去掉 */
        struct ifreq ifr;
        memset(&ifr, 0x00, sizeof(ifr));
        strncpy(ifr.ifr_name, WLAN_DEV , strlen(WLAN_DEV) + 1);
        setsockopt(sock, SOL_SOCKET, SO_BINDTODEVICE, (char *)&ifr, sizeof(ifr));
    }

    #define MAX_REV_BUFFER 512
    BYTE_T rev_buffer[MAX_REV_BUFFER];

    int skipLen = 26;/* radiotap 默认长度为26 */

    while((s_pSnifferCall != NULL) && (s_enable_sniffer == 1))
    {
        int rev_num = recvfrom(sock, rev_buffer, MAX_REV_BUFFER, 0, NULL, NULL);
        ieee80211_radiotap_header *pHeader = (ieee80211_radiotap_header *)rev_buffer;
        skipLen = pHeader->it_len;

#ifdef WIFI_CHIP_7601
        skipLen = 144;
#endif
        if(skipLen >= MAX_REV_BUFFER)
        {/* 有出现过header全ff的情况，这里直接丢掉这个包 */
            continue;
        }

        if(0)
        {
            printf("skipLen:%d ", skipLen);
            int index = 0;
            for(index = 0; index < 180; index++)
            {
                printf("%02X-", rev_buffer[index]);
            }
            printf("\r\n");
        }
        if(rev_num > skipLen)
        {
            s_pSnifferCall(rev_buffer + skipLen, rev_num - skipLen);
        }
    }

     s_pSnifferCall = NULL;

    close(sock);

    printf("Sniffer Proc Finish\r\n");
    return (void *)0;
}

static volatile int s_enable_qrcode = 0;
static void *func_Qrcode(void *pReserved)
{
    printf("Qrcode Thread Create\r\n");

    while( s_enable_qrcode == 1 )
    {
#ifdef WIFI_CHIP_7601
        tuya_linux_wf_set_cur_channel(s_curr_channel);
#endif
        sleep(1);
        //take Pic && Qrcode Decode
        char *pStr = __tuya_linux_get_snap_qrcode();
        OPERATE_RET ret = __tuya_ipc_qrcode_direct_connect(pStr);
        if(ret == OPRT_OK)
        {
            s_enable_qrcode = 0;
        }
    }

    printf("Qrcode Proc Finish\r\n");
    return (void *)0;
}
static pthread_t sniffer_thId; // 抓包线程ID
static pthread_t qrcode_thId; // 二维码线程ID
OPERATE_RET hwl_wf_sniffer_set(IN CONST BOOL_T en,IN CONST SNIFFER_CALLBACK cb)
{
    if(en == TRUE)
    {
        IPC_APP_Notify_LED_Sound_Status_CB(IPC_START_WIFI_CFG);

        printf("Enable Sniffer\r\n");
        hwl_wf_wk_mode_set(WWM_SNIFFER);

        s_pSnifferCall = cb;
        s_enable_sniffer = 1;
        pthread_create(&sniffer_thId, NULL, func_Sniffer, NULL);

        printf("Enable Qrcode \r\n");
        s_enable_qrcode = 1;
        pthread_create(&qrcode_thId, NULL, func_Qrcode, NULL);
    }else
    {
        printf("Disable Sniffer\r\n");
        s_enable_sniffer = 0;
        pthread_join(sniffer_thId, NULL);

        hwl_wf_wk_mode_set(WWM_STATION);

        printf("Disable Qrcode\r\n");
        s_enable_qrcode = 0;
        pthread_join(qrcode_thId, NULL);

        sniffer_set_done = TRUE;
    }

    return OPRT_OK;
}


/*
wlan0	  Link encap:Ethernet  HWaddr 08:57:00:88:5c:16
          inet addr:192.168.22.3  Bcast:192.168.23.255	Mask:255.255.252.0
。。。。。
。。。。
*/
OPERATE_RET hwl_wf_get_ip(IN CONST WF_IF_E wf,OUT NW_IP_S *ip)
{
    if(NULL == ip)
    {
        return OPRT_INVALID_PARM;
    }

    if(wf == WF_AP)
    {/* AP模式下简单处理 */
        memcpy(ip->ip, "192.168.0.1", strlen("192.168.0.1"));
        memcpy(ip->gw, "192.168.0.1", strlen("192.168.0.1"));
        memcpy(ip->mask, "255.255.255.0", strlen("255.255.255.0"));
    }

    if(wf == WF_STATION)
    {
        FILE *pp = popen("ifconfig "WLAN_DEV, "r");

        if(pp == NULL)
        {
           return OPRT_COM_ERROR;
        }

        char tmp[256];
        memset(tmp, 0, sizeof(tmp));
        while (fgets(tmp, sizeof(tmp), pp) != NULL)
        {
            char *pIPStart = strstr(tmp, "inet ");
            if(pIPStart != NULL)
            {/* 目前都是一行里面包含ip gw mask 直接跳出  */
                break;
            }
        }

        {
            /* 查找ip  */
            char *pIPStart = strstr(tmp, "inet ");
            if(pIPStart != NULL)
            {
                sscanf(pIPStart + strlen("inet "), "%s", ip->ip);
            }
        }

        {
            /* 查找gw  */
            char *pGWStart = strstr(tmp, "broadcast ");
            if(pGWStart != NULL)
            {
                sscanf(pGWStart + strlen("broadcast "), "%s", ip->gw);
            }
        }

        {
            /* 查找mask */
            char *pMASKStart = strstr(tmp, "netmask ");
            if(pMASKStart != NULL)
            {
                sscanf(pMASKStart + strlen("netmask "), "%s", ip->mask);
            }
        }

        pclose(pp);
    }

//    printf("WIFI[%d] Get IP:%s\r\n", wf, ip->ip);
    return OPRT_OK;
}

/*
wlp3s0: flags=4163<UP,BROADCAST,RUNNING,MULTICAST>  mtu 1500
        inet 192.168.3.14  netmask 255.255.255.0  broadcast 192.168.3.255
        ether a4:02:b9:b1:99:50  txqueuelen 1000  (以太网)
。。。。。。
。。。。。。
*/
OPERATE_RET hwl_wf_get_mac(IN CONST WF_IF_E wf,INOUT NW_MAC_S *mac)
{
    if(NULL == mac)
    {
        return OPRT_INVALID_PARM;
    }

    FILE *pp = popen("ifconfig "WLAN_DEV, "r");
    if(pp == NULL)
    {
       return OPRT_COM_ERROR;
    }

    char tmp[256];
    memset(tmp, 0, sizeof(tmp));
    while (fgets(tmp, sizeof(tmp), pp) != NULL)
    {
        char *pMACStart = strstr(tmp, "ether ");
        if(pMACStart != NULL)
        {
            int x1,x2,x3,x4,x5,x6;
            sscanf(pMACStart + strlen("ether "), "%x:%x:%x:%x:%x:%x",&x1,&x2,&x3,&x4,&x5,&x6);
            mac->mac[0] = x1 & 0xFF;
            mac->mac[1] = x2 & 0xFF;
            mac->mac[2] = x3 & 0xFF;
            mac->mac[3] = x4 & 0xFF;
            mac->mac[4] = x5 & 0xFF;
            mac->mac[5] = x6 & 0xFF;

            break;
        }
    }
    pclose(pp);

    printf("WIFI Get MAC %02X-%02X-%02X-%02X-%02X-%02X\r\n",
           mac->mac[0],mac->mac[1],mac->mac[2],mac->mac[3],mac->mac[4],mac->mac[5]);

    return OPRT_OK;
}

OPERATE_RET hwl_wf_set_mac(IN CONST WF_IF_E wf,IN CONST NW_MAC_S *mac)
{
    if(NULL == mac)
    {
        return OPRT_INVALID_PARM;
    }
    printf("WIFI Set MAC\r\n");
    //reserved
    return OPRT_OK;
}

OPERATE_RET hwl_wf_wk_mode_set(IN CONST WF_WK_MD_E mode)
{
    char tmpCmd[100] = {0};

    snprintf(tmpCmd, 100, "ifconfig %s up", WLAN_DEV);
    exec_cmd(tmpCmd);

    switch (mode)
    {
        case WWM_LOWPOWER:
        {
            //linux系统不关心低功耗
            break;
        }
        case WWM_SNIFFER:
        {
#ifndef WIFI_CHIP_7601
            snprintf(tmpCmd, 100, "ifconfig %s down", WLAN_DEV);
            exec_cmd(tmpCmd);
#endif
            snprintf(tmpCmd, 100, "iwconfig %s mode Monitor", WLAN_DEV);
            exec_cmd(tmpCmd);
#ifndef WIFI_CHIP_7601
            snprintf(tmpCmd, 100, "ifconfig %s up", WLAN_DEV);
            exec_cmd(tmpCmd);
#endif
            break;
        }
        case WWM_STATION:
        {
#ifndef WIFI_CHIP_7601
            snprintf(tmpCmd, 100, "ifconfig %s down", WLAN_DEV);
            exec_cmd(tmpCmd);
#endif
            snprintf(tmpCmd, 100, "iwconfig %s mode Managed", WLAN_DEV);
            exec_cmd(tmpCmd);
#ifndef WIFI_CHIP_7601
            snprintf(tmpCmd, 100, "ifconfig %s up", WLAN_DEV);
            exec_cmd(tmpCmd);
#endif
            break;
        }
        case WWM_SOFTAP:
        {
#ifndef WIFI_CHIP_7601
            snprintf(tmpCmd, 100, "ifconfig %s down", WLAN_DEV);
            exec_cmd(tmpCmd);
#endif
            snprintf(tmpCmd, 100, "iwconfig %s mode Master", WLAN_DEV);
            exec_cmd(tmpCmd);
#ifndef WIFI_CHIP_7601
            snprintf(tmpCmd, 100, "ifconfig %s up", WLAN_DEV);
            exec_cmd(tmpCmd);
#endif
            break;
        }
        case WWM_STATIONAP:
        {//reserved
            break;
        }
        default:
        {
            break;
        }
    }
    printf("WIFI Set Mode %d\r\n", mode);

    return OPRT_OK;
}


/*
wlp3s0    IEEE 802.11  ESSID:"H"
          Mode:Managed  Frequency:2.452 GHz  Access Point: 58:7F:66:04:73:A8
          Bit Rate=130 Mb/s   Tx-Power=22 dBm
。。。。。。
。。。。。。
*/
OPERATE_RET hwl_wf_wk_mode_get(OUT WF_WK_MD_E *mode)
{
    if(NULL == mode)
    {
        return OPRT_INVALID_PARM;
    }

    FILE *pp = popen("iwconfig "WLAN_DEV, "r");
    if(pp == NULL)
    {
        printf("WIFI Get Mode Fail. Force Set Station \r\n");
        *mode = WWM_STATION;
        return OPRT_OK;
    }

    char scan_mode[10] = {0};
    char tmp[256] = {0};
    while (fgets(tmp, sizeof(tmp), pp) != NULL)
    {
        char *pModeStart = strstr(tmp, "Mode:");
        if(pModeStart != NULL)
        {
            int x1,x2,x3,x4,x5,x6;
            sscanf(pModeStart + strlen("Mode:"), "%s ", scan_mode);
            break;
        }
    }
    pclose(pp);

    *mode = WWM_STATION;
    if(strncasecmp(scan_mode, "Managed", strlen("Managed")) == 0)
    {
        *mode = WWM_STATION;
    }

    if(strncasecmp(scan_mode, "Master", strlen("Master")) == 0)
    {
        *mode = WWM_SOFTAP;
    }

    if(strncasecmp(scan_mode, "Monitor", strlen("Monitor")) == 0)
    {
        *mode = WWM_SNIFFER;
    }
//    printf("WIFI Get Mode [%s] %d\r\n", scan_mode, *mode);

    return OPRT_OK;
}


/***********************************************************
*  Function: wf_station_disconnect
*  Input: none
*  Output: none
*  Return: OPERATE_RET
***********************************************************/
OPERATE_RET hwl_wf_station_disconnect(VOID)
{
    printf("STA Disconn AP\r\n");
    //Reserved

    return OPRT_OK;
}

/*
wlp3s0    IEEE 802.11  ESSID:"HUAWEI-1234"
。。。。。。
          Link Quality=70/70  Signal level=-6 dBm
。。。。。。
*/
OPERATE_RET hwl_wf_station_get_conn_ap_rssi(OUT SCHAR_T *rssi)
{
    if(NULL == rssi)
    {
        return OPRT_INVALID_PARM;
    }
    *rssi = 0;

    FILE *pp = popen("iwconfig "WLAN_DEV, "r");
    if(pp == NULL)
    {
       return OPRT_COM_ERROR;
    }

    char tmp[128] = {0};
    while (fgets(tmp, sizeof(tmp), pp) != NULL)
    {
        /* 查找signal  */
        char *pSIGNALStart = strstr(tmp, "Quality=");
        if(pSIGNALStart != NULL)
        {
            int x = 0;
            int y = 0;
            sscanf(pSIGNALStart + strlen("Quality="), "%d/%d",&x,&y);
            *rssi = x * 100/ (y+1);
            break;
        }
    }
    pclose(pp);

    printf("Get Conn AP RSSI:%d\r\n", *rssi);

    return OPRT_OK;
}


OPERATE_RET hwl_wf_station_stat_get(OUT WF_STATION_STAT_E *stat)
{
    if(NULL == stat)
    {
        return OPRT_INVALID_PARM;
    }
    *stat = WSS_GOT_IP;
    //Reserved
    return OPRT_OK;
}


OPERATE_RET hwl_wf_ap_start(IN CONST WF_AP_CFG_IF_S *cfg)
{
    if(NULL == cfg)
    {
        return OPRT_INVALID_PARM;
    }

    printf("Start AP SSID:%s \r\n", cfg->ssid);
    //Reserved
    return OPRT_OK;
}

OPERATE_RET hwl_wf_ap_stop(VOID)
{
    printf("Stop AP \r\n");
    //Reserved
    return OPRT_OK;
}
