#ifndef _NFC_TLV_H_
#define _NFC_TLV_H_

#include <stdint.h>
#include <stddef.h>

/* TLV Types */
#define TLV_TYPE_NULL               0x00U
#define TLV_TYPE_LOCK_CONTROL       0x01U
#define TLV_TYPE_MEM_CONTROL        0x02U
#define TLV_TYPE_NDEF_MESSAGE       0x03U
#define TLV_TYPE_PROPRIETARY        0xFDU
#define TLV_TYPE_TERMINATOR         0xFEU

/* TLV lengths */
#define TLV_LEN_LOCK_CONTROL        0x03U
#define TLV_LEN_MEM_CONTROL         0x03U

typedef enum {
    TLV_OK,
    TLV_E_INVALID_ARGS,
    TLV_E_FORMAT,
} tlv_status_t;

typedef struct {
    uint8_t  type;
    uint32_t length;
    uint8_t  *value;
} tlv_t;


tlv_status_t tlv_parse(uint8_t *buf, size_t len);


// TLV_StatusTypeDef NFC_TLV_ParseNDEF(uint8_t *buf, size_t len, TLV_TypeDef *tlv);
// void NFC_TLV_PrintTLV(TLV_TypeDef *tlv);

#endif