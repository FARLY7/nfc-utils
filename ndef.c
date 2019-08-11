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
 * File        ndef.c
 * Created by  Sean Farrelly
 * Version     1.0
 * 
 */

/*! @file ndef.c
 * @brief Utility tools for NDEF format.
 */
#include "ndef.h"


/*
 * @brief Convert from network to local endianness.
 */
static uint32_t ntohl(uint8_t *ptr);


/*
 * @brief This API accepts a raw byte buffer and parses into NDEF records.
 */
ndef_status_t ndef_parse(uint8_t *buf, size_t len, ndef_record_t *rec, size_t rec_cnt)
{
    ndef_status_t rslt = NDEF_OK;

    /* Check parameters are valid */
    if((buf == NULL) || (len == 0) || (rec == NULL) || (rec_cnt == 0))
    {
        rslt = NDEF_E_INVALID_ARGS;
        goto error;
    }

    /* Check the first record is valid with a Message Begin flag set */
    if (NDEF_RECORD_GET_FLAG(*buf, NDEF_RECORD_FLAG_MB) == 0)
    {
        rslt = NDEF_E_FORMAT;
        goto error;
    }

    uint8_t *buf_start = buf;
    size_t cnt = 0;

    while((buf - buf_start) < len)
    {
        rec->header  = *buf++;
        rec->type_len = *buf++;
        
        if (NDEF_RECORD_GET_FLAG(rec->header, NDEF_RECORD_FLAG_SR))
        {
            /* NDEF record is a 'short' sized record, copy the 1-byte 'Paylod Length' field */
            rec->payload_len = *buf++;
        }
        else
        {
            /* NDEF record is a 'normal' sized record, copy the 4-byte 'Payload Length' field */
            rec->payload_len = ntohl(buf);
            buf += sizeof(uint32_t);
        }

        if(NDEF_RECORD_GET_FLAG(rec->header, NDEF_RECORD_FLAG_IL))
            rec->id_len = *buf++;
        else
            rec->id_len = 0;


        rec->type = (rec->type_len == 0) ? NULL : buf;
        buf += rec->type_len;

        rec->id = (rec->id_len == 0) ? NULL : buf;
        buf += rec->id_len;

        rec->payload = (rec->payload_len == 0) ? NULL : buf;
        buf += rec->payload_len;

        rec->totalLength = buf - buf_start;

        /* Found Message Begin in the middle of record */
        if(NDEF_RECORD_GET_FLAG(rec->header, NDEF_RECORD_FLAG_MB) && (cnt > 0))
        {
            rslt = NDEF_E_FORMAT;
            goto error;
        }

        /* Is record last in message with Message Eng flag */
        if (NDEF_RECORD_GET_FLAG(rec->header, NDEF_RECORD_FLAG_ME))
        {
            rslt = NDEF_OK;
            goto error;
        }

        rec++;
        cnt++;

        if(cnt > rec_cnt)
        {
            rslt = NDEF_E_FORMAT;
            goto error;
        }
    }

    if(NDEF_RECORD_GET_FLAG(rec->header, NDEF_RECORD_FLAG_ME))
    {
        rslt = NDEF_OK;
        goto error;
    }

error:
    return rslt;
}


// uint8_t* NDEF_BuildRecord(uint8_t flags, uint8_t *type, uint8_t typeLength, uint8_t *id, uint8_t idLength, uint8_t *payload, uint32_t payloadLength)
// {
//     size_t totalLength;
//     uint8_t shortRecord; /* bool */
//     uint8_t header;
//     uint8_t *pos;

//     shortRecord = payloadLength < 256 ? 1 : 0;

//     /* ==== Calculate total length of record */
//     totalLength = 2; /* Flag + type length */
//     totalLength += shortRecord ? sizeof(uint8_t) : sizeof(uint32_t); /* Payload length */
//     if(idLength > 0)
//         totalLength += 1;
//     totalLength += typeLength + idLength + payloadLength;
//     /* ===================================== */

//     //NDEF_RecordType *rec = malloc(sizeof(NDEF_RecordType));
//     uint8_t *buf = malloc(totalLength);
//     if(buf == NULL)
//     {
//         Log_info("NDEF: Failed to allocate record for build");
//         return NULL;
//     }
//     pos = buf; /* Set pointer to beginning of NDEF Record */

//     /* ==== Ensure correct flags are set in record */
//     header = flags;

//     if(idLength > 0)
//         header |= NDEF_RECORD_FLAG_IL;
//     if(shortRecord)
//         header |= NDEF_RECORD_FLAG_SR;
    
//     *pos++ = header;
//     *pos++ = typeLength;

//     if(shortRecord)
//     {
//         *pos++ = (uint8_t) payloadLength;
//     }
//     else
//     {
//         memcpy(pos, (uint8_t*) &payloadLength, sizeof(payloadLength));
//         pos += sizeof(payloadLength);
//     }
    
//     if(idLength > 0)
//         *pos++ = idLength;

//     if((type != NULL) && (typeLength > 0))
//     {
//         memcpy(pos, type, typeLength);
//         pos += sizeof(typeLength);
//     }
//     if(id != NULL && (idLength > 0))
//     {
//         memcpy(pos, id, idLength);
//         pos += sizeof(idLength);
//     }
//     if(payload != NULL && (payloadLength > 0))
//     {
//         memcpy(pos, payload, payloadLength);
//         pos += sizeof(payloadLength);
//     }
    
//     return buf;
// }


// /* Print our NDEF Record in similar styling to http://www.ndefparser.net */
// void NDEF_PrintRecord(NDEF_RecordType *rec)
// {
//     Log_info("======== NDEF RECORD =======");
//     Log_info("HEADER: 0x%02X (MB:%d, ME:%d, CF:%d, SR:%d, IL:%d, TNF:%d)",
//              rec->header,
//              NDEF_RECORD_GET_FLAG(rec->header, NDEF_RECORD_FLAG_MB),
//              NDEF_RECORD_GET_FLAG(rec->header, NDEF_RECORD_FLAG_ME),
//              NDEF_RECORD_GET_FLAG(rec->header, NDEF_RECORD_FLAG_CF),
//              NDEF_RECORD_GET_FLAG(rec->header, NDEF_RECORD_FLAG_SR),
//              NDEF_RECORD_GET_FLAG(rec->header, NDEF_RECORD_FLAG_IL),
//              (rec->header & NDEF_RECORD_FLAG_TNF_Msk));

//     Log_info("TYPE LENGTH: 0x%02X",  rec->typeLength);
//     Log_info("PAYLOAD LENGTH: 0x%X", rec->payloadLength);

//     if(rec->idLength != 0)
//         Log_info("ID LENGTH: 0x%02X", rec->idLength);
//     else
//         Log_info("ID LENGTH: No ID Length field");
    
//     if (rec->typeLength != 0)
//         Log_info("TYPE: \"%.*s\"", rec->typeLength, rec->type);
//     else
//         Log_info("TYPE: No Type field");

//     if (rec->idLength != 0)
//         Log_info("ID: \"%.*s\"", rec->idLength, rec->id);
//     else
//         Log_info("ID: No ID field");

//     if (rec->payloadLength != 0)
//     {
//         char buf[(rec->payloadLength*2) + 15];
//         char *ptr = &buf[0];

//         ptr += sprintf(ptr, "PAYLOAD: 0x");
//         for(size_t i = 0 ; i < rec->payloadLength ; i++)
//         {
//             ptr += sprintf(ptr, "%02X", rec->payload[i]);
//         }
//         Log_info(buf);

//         Log_print("Payload as ASCII: \"", 19);
//         Log_print((char*) rec->payload, rec->payloadLength);
//         Log_println("\"", 1);
//     }
//     else
//     {
//         Log_info("Payload: No Payload");
//     }

//     Log_info("============================");
// }

static uint32_t ntohl(uint8_t *ptr) 
{
    uint32_t temp = 0;

    temp |= ((*ptr++ << 24) & 0xFF000000);
    temp |= ((*ptr++ << 16) & 0x00FF0000);
    temp |= ((*ptr++ << 8)  & 0x0000FF00);
    temp |= ((*ptr++ << 0)  & 0x000000FF);

    return temp;
}    
    
