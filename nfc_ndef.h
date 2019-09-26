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
 * File        ndef.h
 * Created by  Sean Farrelly
 * Version     1.0
 * 
 */

/*! @file ndef.h
 * @brief Utilities for NDEF format.
 */

/*!
 * @defgroup NDEF API
 */
#ifndef _NDEF_H_
#define _NDEF_H_

/*! CPP guard */
#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stddef.h>

/*!
 * @brief NDEF record header flags.
 */
#define NDEF_RECORD_FLAG_MB             (1 << 7)    /* MB: Message Begin */
#define NDEF_RECORD_FLAG_ME             (1 << 6)    /* ME: Message End */
#define NDEF_RECORD_FLAG_CF             (1 << 5)    /* CF: Chunk Flag */   
#define NDEF_RECORD_FLAG_SR             (1 << 4)    /* SR: Short Record */
#define NDEF_RECORD_FLAG_IL             (1 << 3)    /* IL: ID Length field present*/
#define NDEF_RECORD_FLAG_TNF_Msk        (7 << 0)    /* TNF Mask */
// #define NDEF_RECORD_FLAG_TNF_EMPTY      (0 << 0)    /* TNF: Empty */
// #define NDEF_RECORD_FLAG_TNF_WELLKNOWN  (1 << 0)    /* TNF: NFC Forum well-known type [NFC RTD] */
// #define NDEF_RECORD_FLAG_TNF_MEDIA      (2 << 0)    /* TNF: Media-type as defined in RFC 2046 */
// #define NDEF_RECORD_FLAG_TNF_URI        (3 << 0)    /* TNF: Absolute URI as defined in RFC 3986 */
// #define NDEF_RECORD_FLAG_TNF_EXTERNAL   (4 << 0)    /* TNF: NFC Forum external type [NFC RTD] */
// #define NDEF_RECORD_FLAG_TNF_UNKNOWN    (5 << 0)    /* TNF: Unknown */
// #define NDEF_RECORD_FLAG_TNF_UNCHANGED  (6 << 0)    /* TNF: Unchanged */
// #define NDEF_RECORD_FLAG_TNF_RESERVED   (7 << 0)    /* TNF: Reserved */

/*!
 * @brief NDEF Record flag check macro.
 */
#define NDEF_RECORD_GET_FLAG(__BYTE__, __FLAG__)     ((((__BYTE__) & (__FLAG__)) == (__FLAG__)) ? 1 : 0)

/*!
 * @brief NDEF API status code.
 */
typedef enum
{
    NDEF_OK,             /* API execution succes            */
    NDEF_E_NOT_FOUND,     /* Parser did not find NDEF record */
    NDEF_E_INVALID_ARGS /* Invalid fucntion parameters     */
} ndef_status_t;

typedef enum
{
    TNF_EMPTY,          /* The value indicates that there is no type or payload associated with this record. */
    TNF_WELL_KNOWN,     /* NFC Forum well-known type [NFC RTD].                                              */
    TNF_MEDIA_TYPE,     /* Media-type as defined in RFC 2046 [RFC 2046].                                     */
    TNF_ABSOLUTE_URI,   /* Absolute URI as defined in RFC 3986 [RFC 3986].                                   */
    TNF_EXTERNAL_TYPE,  /* NFC Forum external type [NFC RTD].                                                */
    TNF_UNKNOWN_TYPE,   /* The value indicates that there is no type associated with this record.            */
    TNF_UNCHANGED,      /* The value is used for the record chunks used in chunked payload.                  */
    TNF_RESERVED        /* The value is reserved for future use.                                             */
} ndef_record_tnf_t;

/*!
 * @brief NDEF record structure.
 */
typedef struct
{
    uint8_t  header;
    uint8_t  type_len;
    size_t   payload_len;
    uint8_t  id_len;
    uint8_t  *type;
    uint8_t  *id;
    uint8_t  *payload;
    /* Extra field for driver */
    size_t   total_length;
} ndef_record_t;

/*
 * @brief This API parses the next NDEF record found in the buffer.
 * 
 * @param[in]     buf : Pointer to byte buffer containing ndef message(s).
 * @param[out]    rec : Pointer to NDEF record(s) buffer in which to save parsed NDEF record.
 * @param[in] rec_cnt : Number of bytes read from bufer.
 *
 * @return API status code.
 */
ndef_status_t ndef_parse_next_rec(uint8_t *buf, ndef_record_t *rec, size_t *br);

#ifdef __cplusplus
}
#endif /* End of CPP guard */
#endif /* NDEF_H_ */
/** @}*/