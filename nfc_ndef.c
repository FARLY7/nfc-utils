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
#include "nfc_ndef.h"

/*
 * @brief This API parses the next NDEF record found in the buffer.
 */
ndef_status_t ndef_parse_next_rec(uint8_t *buf, ndef_record_t *rec, size_t *br)
{
    /* Check parameters are valid */
    if((buf == NULL) || (rec == NULL) || (br == 0))
        return NDEF_E_INVALID_ARGS;

    /* Check the NDEF record has at least the MB or ME set for validity */
    if (NDEF_RECORD_GET_FLAG(*buf, NDEF_RECORD_FLAG_MB) == 0 &&
        NDEF_RECORD_GET_FLAG(*buf, NDEF_RECORD_FLAG_ME) == 0)
        return NDEF_E_NOT_FOUND;

    uint8_t *buf_start = buf;

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

    rec->total_length = buf - buf_start;

    *br = rec->total_length;

    return NDEF_OK;
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



    
    
