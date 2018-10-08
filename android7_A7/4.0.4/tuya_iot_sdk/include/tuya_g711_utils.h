#ifndef TUYA_G711_UTILS_H
#define TUYA_G711_UTILS_H

#ifdef __cplusplus
extern "C" {
#endif


#define		TUYA_G711_A_LAW     (0)
#define		TUYA_G711_MU_LAW    (1)


int tuya_g711_encode(unsigned char type, unsigned short *in_pcm_buf, unsigned int pcm_len_short, unsigned char *out_g711_buf, unsigned int *p_g711_len_byte);

int tuya_g711_decode(unsigned char type, unsigned char *in_g711_buf, unsigned int g711_len_byte, unsigned short *out_pcm_buf, unsigned int *p_pcm_len_short);


#ifdef __cplusplus
}
#endif

#endif // TUYA_G711_UTILS_H
