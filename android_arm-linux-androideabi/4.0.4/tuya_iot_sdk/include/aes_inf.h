/***********************************************************
*  File: aes_inf.h 
*  Author: nzy
*  Date: 20161027

封装好的aes加解密头文件
***********************************************************/
#ifndef _AES_INF_H
#define _AES_INF_H

#include "adapter_platform.h"
#include "tuya_cloud_error_code.h"

#ifdef __cplusplus
extern "C" {
#endif

#define AES_ENCRYPT_KEY_LEN  16

typedef OPERATE_RET (*AES128_ECB_ENC_BUF)(IN CONST BYTE_T *input, IN CONST UINT_T input_len,
                                          OUT BYTE_T *output, OUT UINT_T *output_len,
                                          IN CONST BYTE_T *key, IN CONST UINT_T key_len);

typedef OPERATE_RET (*AES128_ECB_DEC_BUF)(IN CONST BYTE_T *input, IN CONST UINT_T input_len,
                                          OUT BYTE_T *output, OUT UINT_T *output_len,
                                          IN CONST BYTE_T *key, IN CONST UINT_T key_len);

typedef OPERATE_RET (*AES128_CBC_ENC_BUF)(IN CONST BYTE_T *input, IN CONST UINT_T input_len,
                                          OUT BYTE_T *output, OUT UINT_T *output_len,
                                          IN CONST BYTE_T *key, IN CONST UINT_T key_len, IN CONST BYTE_T *iv, IN CONST UINT_T iv_len);

typedef OPERATE_RET (*AES128_CBC_DEC_BUF)(IN CONST BYTE_T *input, IN CONST UINT_T input_len,
                                          OUT BYTE_T *output, OUT UINT_T *output_len,
                                          IN CONST BYTE_T *key, IN CONST UINT_T key_len, IN CONST BYTE_T *iv, IN CONST UINT_T iv_len);

typedef struct
{
    AES128_ECB_ENC_BUF ecb_enc;
    AES128_ECB_DEC_BUF ecb_dec;
    AES128_CBC_ENC_BUF cbc_enc;
    AES128_CBC_DEC_BUF cbc_dec;
}AES_METHOD_REG_S;

OPERATE_RET aes_method_register(IN CONST AES_METHOD_REG_S *aes);

OPERATE_RET aes_method_unregister(VOID);

OPERATE_RET aes128_ecb_encode(IN CONST BYTE_T *data, IN CONST UINT_T len,
                              OUT BYTE_T **ec_data, OUT UINT_T *ec_len,
                              IN CONST BYTE_T *key, IN UINT_T key_len);

OPERATE_RET aes128_ecb_decode(IN CONST BYTE_T *data, IN CONST UINT_T len,
                              OUT BYTE_T **dec_data, OUT UINT_T *dec_len,
                              IN CONST BYTE_T *key, IN UINT_T key_len);


OPERATE_RET aes128_ecb_encode_raw(IN CONST BYTE_T *data, IN CONST UINT_T len,
                                  OUT BYTE_T *ec_data,
                                  IN CONST BYTE_T *key, IN UINT_T key_len);

OPERATE_RET aes128_ecb_decode_raw(IN CONST BYTE_T *data, IN CONST UINT_T len,
                                  OUT BYTE_T *dec_data,
                                  IN CONST BYTE_T *key, IN UINT_T key_len);



//not used
OPERATE_RET aes128_cbc_encode(IN CONST BYTE_T *data, IN CONST UINT_T len,
                              OUT BYTE_T **ec_data, OUT UINT_T *ec_len,
                              IN CONST BYTE_T *key, IN UINT_T key_len, IN CONST BYTE_T *iv, IN UINT_T iv_len);

OPERATE_RET aes128_cbc_decode(IN CONST BYTE_T *data,IN CONST UINT_T len,
                              OUT BYTE_T **dec_data,OUT UINT_T *dec_len,
                              IN CONST BYTE_T *key, IN UINT_T key_len, IN CONST BYTE_T *iv, IN UINT_T iv_len);

OPERATE_RET aes128_free_data(IN BYTE_T *data);


#ifdef __cplusplus
}
#endif
#endif

