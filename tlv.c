/*
 * MIT License
 * 
 * Copyright (c) 2019 Sean Farrelly
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 * 
 * File        tlv.c
 * Created by  Sean Farrelly
 * Version     1.0
 * 
 */

/*! @file tlv.c
 * @brief Utility tools for TLV format.
 */
#include "tlv.h"

/*
 * @brief This API accepts a raw byte buffer and parses into TLV structure representation.
 */
tlv_status_t tlv_decode(uint8_t *buf, size_t len, tlv_t *tlv, size_t tlv_cnt)
{
    int8_t rslt = TLV_OK;

    tlv_t tlv[10];
    uint8_t cnt = 0;

    if((buf == NULL) || (len == 0))
    {
        rslt = TLV_E_INVALID_ARGS;
        goto error;
    } 

    uint8_t *buf_start = buf;

    while ((buf - buf_start) < len)
    {
        switch (*buf)
        {
            case TLV_TYPE_NULL:
            {
                tlv->type = *buf++;
                tlv->length = 0;
                tlv->value = 0;
                break;
            }
            case TLV_TYPE_LOCK_CONTROL:
            {
                if (*buf != TLV_LEN_LOCK_CONTROL)
                {
                    rslt = TLV_E_FORMAT;
                    goto error;
                }

                tlv->type = *buf++;
                tlv->length = *buf++;
                tlv->value = buf;

                buf += tlv->length;
                break;
            }
            case TLV_TYPE_MEM_CONTROL:
            {
                if (*buf != TLV_LEN_MEM_CONTROL)
                {
                    rslt = TLV_E_FORMAT;
                    goto error;
                }

                tlv->type = *buf++;
                tlv->length = *buf++;
                tlv->value = buf;

                buf += tlv->length;
                break;
            }
            case TLV_TYPE_NDEF_MESSAGE:
            {
                tlv->type = *buf++;

                if (*buf == 0xFF)
                {
                    /* Long record? */
                    tlv->length = ((*(buf + 1) << 8) & 0xFF00) | ((*(buf + 2) << 0) & 0x00FF);
                    buf += 3; /* Replace with #define */
                }
                else
                {
                    /* Short record */
                    tlv->length = *buf++;
                }

                tlv->value  = buf++;
                break;
            }
            case TLV_TYPE_PROPRIETARY:
            {
                tlv->type = *buf++;

                if (*buf == 0xFF)
                {
                    /* Long record? */
                    tlv->length = ((*(buf + 1) << 8) & 0xFF00) | ((*(buf + 2) << 0) & 0x00FF);
                    buf += 3; /* Replace with #define */
                }
                else
                {
                    /* Short record */
                    tlv->length = *buf++;
                }

                tlv->value  = buf++;
                break;
            }
            case TLV_TYPE_TERMINATOR:
            {
                tlv->type = *buf++;
                tlv->length = 0;
                tlv->value = 0;

                /* Now finished parsing */
                rslt = TLV_OK;
                goto error;
            }
            default:
            {
                rslt = TLV_E_FORMAT;
                goto error;
            }

            cnt++;
            tlv++;

            if(cnt > tlv_cnt)
            {
                rslt = TLV_E_NO_SPACE;
                goto error;
            }
        }
    }

error:
    return rslt;
}


/*
 * @brief This API converts a TLV structure representation into raw byte buffer.
 */ 
tlv_status_t tlv_encode(uint8_t *buf, uint8_t type, uint32_t length, uint8_t *value, uint32_t len)
{
    tlv_status_t rslt = TLV_OK;

error:
    return rslt;
}
