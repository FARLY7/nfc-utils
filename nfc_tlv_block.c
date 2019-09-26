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
#include "nfc_tlv_block.h"

/*
 * @brief This API parses the next TLV block found in the byte-buffer.
 */
tlv_status_t t2t_parse_next_tlv(uint8_t *buf, tlv_t *tlv, size_t *br)
{
    if((buf == NULL) || (tlv == NULL) || (br == NULL))
        return TLV_E_INVALID_ARGS;

    uint8_t *buf_start = buf;

    if((*buf == TLV_NULL) || (*buf == TLV_TERMINATOR))
    {
        tlv->type   = *buf++;
        tlv->length = 0;
        tlv->value  = 0;            
    }
    else if((*buf == TLV_LOCK_CONTROL) || (*buf == TLV_MEMORY_CONTROL))
    {
        /* Length field should equal 3 for LOCK CONTROL and MEMORY CONTROL blocks. */
        if(buf[1] != TLV_LOCK_MEMORY_CTRL_LEN)
            return TLV_E_NOT_FOUND;

        tlv->type   = *buf++;
        tlv->length = *buf++;
        tlv->value  = buf;
    }
    else if((*buf == TLV_NDEF_MESSAGE) || (*buf == TLV_PROPRIETARY))
    {
        tlv->type = *buf++;

        if (*buf == 0xFF)
        {
            /* Long record? */
            tlv->length = ((*(buf + 1) << 8) & 0xFF00) | *(buf + 2);
            buf += 3;
        }
        else
        {
            /* Short record */
            tlv->length = *buf++;
        }

        tlv->value = buf;
    }
    else
    {
        *br = 1;
        return TLV_E_NOT_FOUND;
    }
    
    buf += tlv->length;
    *br = buf_start - buf;
    
    return TLV_OK;
}