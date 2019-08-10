#include "nfc_tlv.h"



/* How do you avoid malloc? */

tlv_status_t tlv_parse(uint8_t *buf, size_t len)
{
    int8_t rslt;

    tlv_t tlv[10];
    uint8_t tlv_cnt = 0;

    if((buf == NULL) || (len == 0))
    {
        rslt = TLV_E_INVALID_ARGS;
        goto error;
    } 

    while (len--)
    {
        switch (*buf)
        {
            case TLV_TYPE_NULL:
            {
                tlv[tlv_cnt].type = *buf++;
                tlv[tlv_cnt].length = 0;
                tlv[tlv_cnt].value = 0;
                break;
            }
            case TLV_TYPE_LOCK_CONTROL:
            {
                if (*buf != TLV_LEN_LOCK_CONTROL)
                {
                    rslt = TLV_E_FORMAT;
                    goto error;
                }

                tlv[tlv_cnt].type = *buf++;
                tlv[tlv_cnt].length = *buf++;
                tlv[tlv_cnt].value = buf;

                buf += tlv[tlv_cnt].length;
                break;
            }
            case TLV_TYPE_MEM_CONTROL:
            {
                if (*buf != TLV_LEN_MEM_CONTROL)
                {
                    rslt = TLV_E_FORMAT;
                    goto error;
                }

                tlv[tlv_cnt].type = *buf++;
                tlv[tlv_cnt].length = *buf++;
                tlv[tlv_cnt].value = buf;

                buf += tlv[tlv_cnt].length;
                break;
            }
            case TLV_TYPE_NDEF_MESSAGE:
            {
                tlv[tlv_cnt].type = *buf++;

                if (*buf == 0xFF)
                {
                    /* Long record? */
                    tlv[tlv_cnt].length = ((*(buf + 1) << 8) & 0xFF00) | ((*(buf + 2) << 0) & 0x00FF);
                    buf += 3; /* Replace with #define */
                }
                else
                {
                    /* Short record */
                    tlv[tlv_cnt].length = *buf++;
                }

                tlv->value  = buf++;
                break;
            }
            case TLV_TYPE_PROPRIETARY:
            {
                tlv[tlv_cnt].type = *buf++;

                if (*buf == 0xFF)
                {
                    /* Long record? */
                    tlv[tlv_cnt].length = ((*(buf + 1) << 8) & 0xFF00) | ((*(buf + 2) << 0) & 0x00FF);
                    buf += 3; /* Replace with #define */
                }
                else
                {
                    /* Short record */
                    tlv[tlv_cnt].length = *buf++;
                }

                tlv->value  = buf++;
                break;
            }
            case TLV_TYPE_TERMINATOR:
            {
                tlv[tlv_cnt].type = *buf++;
                tlv[tlv_cnt].length = 0;
                tlv[tlv_cnt].value = 0;

                /* Now finished parsing */
                rslt = TLV_OK;
                goto error;
            }
            default:
            {
                rslt = TLV_E_FORMAT;
                goto error;
            }

            tlv_cnt++;
        }

error:
    return rslt;
}

TLV_StatusTypeDef NFC_TLV_ParseNDEF(uint8_t *buf, size_t len, TLV_TypeDef *tlv)
{
    //uint8_t *ptr = buf;
    uint32_t length = 0;

    //size_t tempLen = len;


    }

    return TLV_ERROR;
}

// void NFC_TLV_PrintTLV(TLV_TypeDef *tlv)
// {
//     Log_info("===== TLV =====");
//     Log_info("Type: %02X", tlv->type);
//     Log_info("Length: %d", tlv->length);
//     Log_print("Value: ", 7);
//     //Log_println(tlv->value, tlv->length);
//     for(size_t i = 0 ; i < tlv->length ; i++)
//     {
//         Log_info("%c", tlv->value[i]);
//     }
// }

