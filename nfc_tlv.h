#ifndef _NFC_TLV_H_
#define _NFC_TLV_H_

#include <stdint.h>
#include <stddef.h>

#define TLV_TYPE_NULL               0x00U
#define TLV_TYPE_LOCK_CONTROL       0x01U
#define TLV_TYPE_MEM_CONTROL        0x02U
#define TLV_TYPE_NDEF_MESSAGE       0x03U
#define TLV_TYPE_PROPRIETARY        0xFDU
#define TLV_TYPE_TERMINATOR         0xFEU

#define TLV_LEN_LOCK_CONTROL        0x03U
#define TLV_LEN_MEM_CONTROL         0x03U

typedef enum {
    TLV_OK              = 0U,
    TLV_ERROR           = 1U,
    TLV_INVALID_ARGS    = 2U
} TLV_StatusTypeDef;


typedef struct {
    uint8_t  type;
    uint32_t length;
    uint8_t  *value;
} TLV_TypeDef;


TLV_StatusTypeDef NFC_TLV_ParseNDEF(uint8_t *buf, size_t len, TLV_TypeDef *tlv);
void NFC_TLV_PrintTLV(TLV_TypeDef *tlv);

#endif