#ifndef __ES_MP3_TO_TS_H__
#define __ES_MP3_TO_TS_H__

#include <time.h>

//#include "slog.h"
//#include "frame_buf.h"
typedef enum {
	EVTAG_START_TIME = 0x00,
	EVTAG_SCALE = 0x01,
	EVTAG_CHN_STATUS = 0x02,
	EVTAG_END = 0xFF,
} TS_EVENT_TAG;

typedef enum {
	CHST_NORMAL = 0x00,
	CHST_WAITING = 0x01,
	CHST_NOBANDWIDTH = 0x02,
	CHST_END = 0x03,
} TS_CHN_STATUS;

//#define FRAME_SIZE_MAX		(512* 1024)/*主码流(2 * 1024 * 1024 + 40000) 可以缩小*/
//#define FRAME_SIZE_VGA		(128* 1024)
#define FRAME_SIZE_MAX		(1 * 1024 * 1024)
#define FRAME_SIZE_VGA		(512 * 1024)
#define FRAME_SIZE_AUDIO		(64* 1024)/*音频的TS buf*/

#define TS_PKT_SIZE			188
#define TS_PKT_SYNC_BYTE	0x47
#define TS_MAX_PROGRAM_NUM     32
#define MPEGTS_MAX_STREAMS      4

#define NALU_TYPE_IFRAME 		0x05
#define NALU_TYPE_PFRAME 		0x01
#define NALU_TYPE_SLICE 		0x01
#define NALU_TYPE_DPA 			0x02
#define NALU_TYPE_DPB 			0x03
#define NALU_TYPE_DPC 			0x04
#define NALU_TYPE_IDR 			0x05
#define NALU_TYPE_SEI 			0x06
#define NALU_TYPE_SPS 			0x07
#define NALU_TYPE_PPS 			0x08
#define NALU_TYPE_AUD 			0x09
#define NALU_TYPE_EOSEQ 		0x0A
#define NALU_TYPE_EOSTREAM 		0x0B
#define NALU_TYPE_FILL 			0x0C


#define H264_NALU_PFRAME 		0x01
#define H264_NALU_DPA 			0x02
#define H264_NALU_DPB 			0x03
#define H264_NALU_DPC 			0x04
#define H264_NALU_IFRAME 		0x05
#define H264_NALU_SEI 			0x06
#define H264_NALU_SPS 			0x07
#define H264_NALU_PPS 			0x08
#define H264_NALU_AUD 			0x09
#define H264_NALU_EOSEQ 		0x0A
#define H264_NALU_EOSTREAM 		0x0B
#define H264_NALU_FILL 			0x0C

#define H265_NALU_PFRAME_1 		0x00
#define H265_NALU_PFRAME_2 		0x01 //notice:the p frame nalu value of h265 is equal to h264
#define H265_NALU_IFRAME_1 		0x13
#define H265_NALU_IFRAME_2 		0x14
#define H265_NALU_VPS			0x20
#define H265_NALU_SPS 			0x21
#define H265_NALU_PPS 			0x22

#define PES_PROGRAM_STREAM_MAP          0xbc
#define PES_PRIVATE_STREAM_1            0xbd
#define PES_PADDING                     0xbe
#define PES_PRIVATE_STREAM_2            0xbf
#define PES_ECM                         0xb0
#define PES_EMM                         0xb1
#define PES_PROGRAM_STREAM_DIRECTORY    0xff
#define PES_DSMCC_STREAM                0xf2
#define PES_ITU_T_H222_1_TYPE_E_STREAM  0xf8
#define __MIN(a, b) ( ((a) < (b)) ? (a) : (b) )

#define UNKNOWN_ES      0x00
#define VIDEO_ES        0x01
#define AUDIO_ES        0x02
#define SPU_ES          0x03
#define NAV_ES          0x04

#define PACK_START_CODE             ((unsigned int)0x000001ba)
#define SYSTEM_HEADER_START_CODE    ((unsigned int)0x000001bb)
#define SEQUENCE_END_CODE           ((unsigned int)0x000001b7)
#define PACKET_START_CODE_MASK      ((unsigned int)0xffffff00)
#define PACKET_START_CODE_PREFIX    ((unsigned int)0x00000100)
#define ISO_11172_END_CODE          ((unsigned int)0x000001b9)

/* mpeg2 */
#define PROGRAM_STREAM_MAP 0x1bc
#define PRIVATE_STREAM_1   0x1bd
#define PADDING_STREAM     0x1be
#define PRIVATE_STREAM_2   0x1bf


#define AUDIO_ID 0xc0
#define VIDEO_ID 0xe0
#define AC3_ID   0x80
#define DTS_ID   0x8a
#define LPCM_ID  0xa0
#define SUB_ID   0x20

#define STREAM_TYPE_VIDEO_MPEG1     0x01
#define STREAM_TYPE_VIDEO_MPEG2     0x02
#define STREAM_TYPE_AUDIO_MPEG1     0x03
#define STREAM_TYPE_AUDIO_MPEG2     0x04
#define STREAM_TYPE_PRIVATE_SECTION 0x05
#define STREAM_TYPE_PRIVATE_DATA    0x06
#define STREAM_TYPE_AUDIO_AAC_ADTS       0x0f
#define STREAM_TYPE_AUDIO_AAC_LATM       0x10
#define STREAM_TYPE_AUDIO_G711A     0x11 //新增g711a格式
#define STREAM_TYPE_VIDEO_MPEG4     0x10
#define STREAM_TYPE_VIDEO_H264      0x1b
#define STREAM_TYPE_VIDEO_H265      0x24

#define STREAM_TYPE_AUDIO_AC3       0x81
#define STREAM_TYPE_AUDIO_DTS       0x8a

#define MAX_BOUND_HEAD_LEN         200

#define PAT_START_PID				0x12
#define PMT_START_PID				0x44

typedef signed char 		int8_t;
typedef unsigned char   	uint8_t;
typedef short  				int16_t;
typedef unsigned short  	uint16_t;
typedef int  				int32_t;
typedef unsigned   			uint32_t;
//typedef long long  			int64_t;

typedef enum 
{
    MEDIA_TYPE_VIDEO = 0,
    MEDIA_TYPE_SOUND
}STREAMTYPR;


typedef enum 
{
    VIDEO_TYPE_H264 = 0,
    VIDEO_TYPE_H265
}VIDEOSUBSTREAMTYPR;

typedef enum 
{
	AUDIO_TYPE_PCM = 0,
    AUDIO_TYPE_G711A,
    AUDIO_TYPE_G711U,
    AUDIO_TYPE_G711,
    AUDIO_TYPE_AMR,
    AUDIO_TYPE_AAC,
    AUDIO_TYPE_G726,
    AUDIO_TYPE_MP3,
    AUDIO_TYPE_OGG,
    AUDIO_TYPE_ADPCM,

    AUDIO_TYPE_NUM
}AUDIOSUBSTREAMTYPR;

typedef enum 
{
    MPEGTS_MUX_CONFIG_AUDIO_ONLY = 1,
    MPEGTS_MUX_CONFIG_VIDEO_ONLY,
    MPEGTS_MUX_CONFIG_AUDIO_VIDEO
}MUXCONFIG;


typedef unsigned long long				UDWORD;

/* for mux */
typedef struct buffer_t
{
    unsigned char			*p_buffer;
    int						i_size;
    UDWORD					i_dts;
    UDWORD					i_pts;
    unsigned char 			pes_header[30];
    int  					has_pes_header;
    int  					header_length;
    struct buffer_t*		p_next;
}
PES_PARA,ES_PARA,TS_PARA,PS_PARA;

typedef struct TSParamer
{
    int					i_pid;
    int					i_stream_type;
    int					i_stream_id;
    UDWORD				i_pes_dts;
    UDWORD				i_pes_length;
} TSPARAMER;

typedef struct PSParamer
{
    int					i_pid;
    int					i_stream_type;
    int					i_stream_id;
    UDWORD				i_pes_dts;
    UDWORD				i_pes_length;
} PSPARAMER;

typedef struct pcr_t
{
    unsigned char		buffer[10];
    int 		bSetPcr;
    int			i_size;
    UDWORD		i_pcr;
} TPCR_PARA;

struct ts_stream_t
{
    int             i_pid;
    int             i_stream_type;
    int             i_stream_id;
    int             i_continuity_counter;

    /* Specific to mpeg4 in mpeg2ts */
    int             i_es_id;

    /* for TS building */
//     sout_buffer_chain_t chain_ts;
};
typedef struct ts_stream_t ts_stream_t;

typedef struct _Ts_Para_Info
{
	int	pat_continuity_counter;
	int	pmt_continuity_counter;
	ES_PARA esv;
	PES_PARA pesv;
	TS_PARA tsv;
	TSPARAMER tsparav;
	TPCR_PARA pcr;
	int mts_ptrv;
}TsParaInfo;


typedef struct _Program_Info
{
	int iMuxConfig;				//该节目中所包含码流类型
	int iStreamType;				 //码流类型音频/视频/其他
	int iSubStreamType; 			//子码流类型例如视频中的h264 h265
	int iNaluType;				//当码流类型为视频时有该帧信息,i帧前面会有pat/pmt/aux等表  
	int iFrameSize;				//帧长度
	int iPrevIDR;					//上一个i帧的位置(为定位包服务)
	int iOffset2PrevIDR;			//距离上一个aux的位置(为定位包服务)
	long long iFrameTime;			//时间信息，可以是UTC时间可以是其它时间根据需求
	unsigned long long iPtsTime;				//PTS时间戳信息
	int 	pes_continuity_counterv;
	int 	pes_continuity_countera;
	int   anchor_continuity_counterv;
}ProgramInfo;

typedef struct _Pat_Info
{
	int	iTotalProgramNum;                	 //共有的节目个数
	int	iChannelId[TS_MAX_PROGRAM_NUM];
}PatInfo;

typedef struct _Stream_Info
{
	PatInfo patinfo;
	int	iCurrentChannelId;        	 //当前正在封装的通道号
	int	iNumPacketsGened;      	 //整个ts流中ts包的个数
	unsigned long  iFrameCnt;   //计数，用于控制何时发送PMT,PCR包
	ProgramInfo	iProgramInfo[TS_MAX_PROGRAM_NUM];		//节目信息     
	TsParaInfo	iTsParaInfo;		//封装信息
}StreamInfo;


typedef struct _Message_Info
{
	unsigned char event_tag[32];
	time_t start_time;				//自1970年1月1日以来的秒数，占32位
	unsigned char direction;		//播放方向，占8位，0x0代表正向，0x1代表反向
	unsigned char numerator;		//代表scale的字符串"n/m"中的n，占8位
	unsigned char denominator;	//代表scale的字符串"n/m"中的m，占8位
	unsigned char channel_id;		//通道号，占8位
	unsigned char channel_status;  //通道状态，占8位，
	int status_code;				//NVR状态码，用于通知NVR当前的一些状态，如带宽不足，占32位
}MessageInfo;

struct bits_buffer
{
    int     i_size;
    int     i_data;
    unsigned char i_mask;
    unsigned char *p_data;
} ; 

unsigned int ConvertEStoTS( unsigned char * pucEsBuf, StreamInfo *stream_info, unsigned char* pucTsOutBuf);
#endif  /* __ES_MP3_TO_TS_H__ */

