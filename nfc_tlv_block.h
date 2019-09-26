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
 * File        tlv.h
 * Created by  Sean Farrelly
 * Version     1.0
 * 
 */

/*! @file tlv.h
 * @brief Utilities for TLV format.
 */

/*!
 * @defgroup TLV API
 */
#ifndef _NFC_TLV_H_
#define _NFC_TLV_H_

/*! CPP guard */
#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stddef.h>

#define TLV_T_LENGTH                1       /* Length of tag field.                                             */
#define TLV_L_SHORT_LENGTH          1       /* Length of a short length field.                                  */
#define TLV_L_LONG_LENGTH           3       /* Length of an extended length field.                              */
#define TLV_L_FORMAT_FLAG           0xFF    /* Value indicating the use of an extended length field.            */
#define TLV_NULL_TERMINATOR_LEN     0       /* Predefined length of the NULL and TERMINATOR TLV blocks.         */
#define TLV_LOCK_MEMORY_CTRL_LEN    3       /* Prefefined length of the LOCK CONTROL and MEMORY CONTROL blocks. */

/*!
 * @brief TLV API status codes.
 */
typedef enum 
{
    TLV_OK,             /* Success                    */
    TLV_E_NOT_FOUND,     /* No TLV found by parser     */
    TLV_E_INVALID_ARGS, /* Invalid function arguments */
} tlv_status_t;

/*!
 * @brief TLV block types
 */
typedef enum
{
    TLV_NULL            = 0x00, /* Might be used for padding of memory areas. */
    TLV_LOCK_CONTROL    = 0x01, /* Defines details of the lock bits.          */     
    TLV_MEMORY_CONTROL  = 0x02, /* Identifies reserved memory areas.          */
    TLV_NDEF_MESSAGE    = 0x03, /* Contains an NDEF message.                  */
    TLV_PROPRIETARY     = 0xFD, /* Tag proprietary information.               */
    TLV_TERMINATOR      = 0xFE  /* Last TLV block in the data area.           */
} tlv_types_t;

/*!
 * @brief TLV structure.
 */
typedef struct
{
    uint8_t type;   /* Type of the TLV block  */
    size_t  length; /* Length of the value field */
    uint8_t *value; /* Pointer to the value field (NULL if no value field is present) */
} tlv_t;

/*
 * @brief This API parses the next TLV block found in the byte-buffer.
 * 
 * @param[in]  buf : Pointer to byte buffer containing TLV data.
 * @param[out] tlv : Pointer to structure that will be filled with parsed data.
 * @param[out] br  : Pointer to value which will store number of bytes read.
 *
 * @return API status code.
 */
tlv_status_t t2t_parse_next_tlv(uint8_t *buf, tlv_t *tlv, size_t *br);


void t2t_print_tlv(tlv_t *tlv);


#ifdef __cplusplus
}
#endif /* End of CPP guard */
#endif /* TLV_H_ */
/** @}*/