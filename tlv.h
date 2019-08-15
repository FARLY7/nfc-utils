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
#ifndef _TLV_H_
#define _TLV_H_

/*! CPP guard */
#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stddef.h>

/* TLV type values */
#define TLV_TYPE_NULL               0x00U
#define TLV_TYPE_LOCK_CONTROL       0x01U
#define TLV_TYPE_MEM_CONTROL        0x02U
#define TLV_TYPE_NDEF_MESSAGE       0x03U
#define TLV_TYPE_PROPRIETARY        0xFDU
#define TLV_TYPE_TERMINATOR         0xFEU

/* TLV length values */
#define TLV_LEN_CONTROL_TYPE        0x03U

/*!
 * @brief TLV API status codes.
 */
typedef enum 
{
    TLV_OK,             /* Success                    */
    TLV_E_INVALID_ARGS, /* Invalid function arguments */
    TLV_E_FORMAT,       /* Error in parsing tlv       */
    TLV_E_NOT_FOUND     /* No space in TLV buffer     */
} tlv_status_t;

/*!
 * @brief TLV structure.
 */
typedef struct
{
    uint8_t type;   /* TLV Type field   */
    size_t  length; /* TLV length field */
    uint8_t *value; /* TLV value data   */
} tlv_t;


/*
 * @brief This API accepts a raw byte buffer and parses into TLV structure representation.
 * 
 * @param[in]     buf : Pointer to byte buffer containing TLV data.
 * @param[in]     len : Length of byte buffer.
 * @param[out]    tlv : Pointer to TLV(s) buffer in which to save parsed TLVs.
 * @param[in] tlv_cnt : Number of TLV(s) available in tlv buffer.
 *
 * @return API status code.
 */
tlv_status_t tlv_get_next(uint8_t *buf, size_t len, tlv_t *tlv, size_t *br)

/*
 * @brief This API converts a TLV structure representation into raw byte buffer.
 * 
 * @param[in]     buf : Pointer to byte buffer to save TLV.
 * @param[in]    type : Value type.
 * @param[in]  length : Length of value.
 * @param[in]   value : Pointer to value data.
 *
 * @return API status code.
 */
tlv_status_t tlv_encode(uint8_t *buf, uint8_t type, uint32_t length, uint8_t *value);


// TLV_StatusTypeDef NFC_TLV_ParseNDEF(uint8_t *buf, size_t len, TLV_TypeDef *tlv);
// void NFC_TLV_PrintTLV(TLV_TypeDef *tlv);

#ifdef __cplusplus
}
#endif /* End of CPP guard */
#endif /* TLV_H_ */
/** @}*/