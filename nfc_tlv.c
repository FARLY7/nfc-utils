#include "nfc_tlv.h"
#include "logger.h"

TLV_StatusTypeDef NFC_TLV_ParseNDEF(uint8_t *buf, size_t len, TLV_TypeDef *tlv)
{
    //uint8_t *ptr = buf;
    uint32_t length = 0;

    //size_t tempLen = len;

    while (len--)
    {
        switch (*buf++)
        {
        case TLV_TYPE_NULL:
        {
            /* Skip NULL TLV */
            break;
        }

        case TLV_TYPE_LOCK_CONTROL:
        {
            if (*buf != TLV_LEN_LOCK_CONTROL) /* Length MUST = 0x3 */
                return TLV_ERROR;
            buf += TLV_LEN_LOCK_CONTROL; /* Skip Lock Control TLV */
            break;
        }
        case TLV_TYPE_MEM_CONTROL:
        {
            if (*buf != TLV_LEN_MEM_CONTROL) /* Length MUST = 0x3 */
                return TLV_ERROR;
            buf += TLV_LEN_MEM_CONTROL; /* Skip Memory Control TLV */
            break;
        }
        case TLV_TYPE_NDEF_MESSAGE:
        {
            if (*buf == 0xFF)
            {
                length = ((*(buf + 1) << 8) & 0xFF00) |
                         ((*(buf + 2) << 0) & 0x00FF);
                buf += 3;
            }
            else
                length = *buf++;

            /* Parse NDEF Message */
            tlv->type   = TLV_TYPE_NDEF_MESSAGE;
            tlv->length = length;
            tlv->value  = buf;

            return TLV_OK;

            break;
        }
        case TLV_TYPE_PROPRIETARY:
        {
            if (*buf == 0xFF)
            {
                length = ((*(buf + 1) << 8) & 0xFF00) |
                         ((*(buf + 2) << 0) & 0x00FF);
                buf += 3;
            }
            else
                length = *buf++;

            buf += length; /* Skip Proprietary TLV */

            break;
        }
        case TLV_TYPE_TERMINATOR:
        {
            /* Do nothing */
            break;
        }
        default:
        {
            break;
        }
        }
    }

    return TLV_ERROR;
}

void NFC_TLV_PrintTLV(TLV_TypeDef *tlv)
{
    Log_info("===== TLV =====");

    Log_info("Type: %02X", tlv->type);
    Log_info("Length: %d", tlv->length);

    Log_print("Value: ", 7);
    //Log_println(tlv->value, tlv->length);
    for(size_t i = 0 ; i < tlv->length ; i++)
    {
        Log_info("%c", tlv->value[i]);
    }
}
// void NFC_TLV_WriteTLV(uint16_t address, uint16_t offset, TLV_TypeDef *tlv, uint32_t timeout)
// {

//     NFC_WriteMemory(&hnfc, address, byteOffset, , timeout);
// }