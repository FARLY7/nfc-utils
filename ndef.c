#include "ndef.h"
#include "logger.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

//static void* byteCopy(void *dst, const void *src, size_t len);
//static uint8_t* createBuffer(size_t size, const uint8_t *src);
static uint32_t ntohl(uint8_t *ptr);

/* Parse NFC NDEF Record according to NDEF Specification */
NDEF_StatusTypeDef NDEF_ParseRecord(uint8_t *buf, size_t size, NDEF_RecordType *rec)
{
    if (size < 2)
        return NDEF_ERROR;

    uint8_t *pos = buf;

    rec->header = *pos++;
    rec->typeLength = *pos++;

    if ((rec->header & NDEF_RECORD_FLAG_SR) == NDEF_RECORD_FLAG_SR)
    {
        /* NDEF record is a 'short' sized record, copy the 1-byte 'Paylod Length' field */
        if(size < 3)
            return NDEF_ERROR;
        rec->payloadLength = *pos++;
    }
    else
    {
        /* NDEF record is a 'normal' sized record, copy the 4-byte 'Payload Length' field */
        if(size < 6)
            return NDEF_ERROR;

        rec->payloadLength = ntohl( pos );
        pos += sizeof(uint32_t);
    }

    if((rec->header & NDEF_RECORD_FLAG_IL) == NDEF_RECORD_FLAG_IL)
    {
        if(size < (pos - buf + 1))
            return NDEF_ERROR;

        rec->idLength = *pos++;
    } else
        rec->idLength = 0;

    rec->type = rec->typeLength == 0 ? NULL : pos;
    pos += rec->typeLength;

    rec->id = rec->idLength == 0 ? NULL : pos;
    pos += rec->idLength;

    rec->payload = rec->payloadLength == 0 ? NULL : pos;
    pos += rec->payloadLength;

    rec->totalLength = pos - buf;
    if(rec->totalLength > size)
        return NDEF_ERROR;
    
    return NDEF_OK;
}


NDEF_StatusTypeDef NDEF_ParseMessage(uint8_t *buf, size_t size, NDEF_MessageType *msg)
{
    uint8_t *pos = buf;
    size_t length = size;

    /* check for Message Begin flag in first byte */
    if ((*pos & NDEF_RECORD_FLAG_MB) != NDEF_RECORD_FLAG_MB)
    {
        /* ERROR: MB flag is not set in first record */
        return NDEF_ERROR;
    }

    NDEF_RecordType tempRec;
    msg->noOfRecords = 0;

    do{

        memset(&tempRec, 0, sizeof(tempRec));

        if(NDEF_ParseRecord(pos, size, &tempRec) >= 0)
        {
            //NDEF_PrintRecord(&tempRec);

            memcpy(&(msg->rec[msg->noOfRecords]), &tempRec, sizeof(tempRec));
            msg->noOfRecords++;
        }
        else
        {
            Log_info("NDEF: Error. Failed to parse record");
        }        
        pos += tempRec.totalLength;
        size -= tempRec.totalLength;

    } while(((tempRec.header & NDEF_RECORD_FLAG_ME) != NDEF_RECORD_FLAG_ME) && ((pos - buf) < length));

    if((tempRec.header & NDEF_RECORD_FLAG_ME) != NDEF_RECORD_FLAG_ME)
    {
        Log_info("NDEF: Error. No Message END flag found");
        return NDEF_ERROR;
    }

    return NDEF_OK;
}


uint8_t* NDEF_BuildRecord(uint8_t flags, uint8_t *type, uint8_t typeLength, uint8_t *id, uint8_t idLength, uint8_t *payload, uint32_t payloadLength)
{
    size_t totalLength;
    uint8_t shortRecord; /* bool */
    uint8_t header;
    uint8_t *pos;

    shortRecord = payloadLength < 256 ? 1 : 0;

    /* ==== Calculate total length of record */
    totalLength = 2; /* Flag + type length */
    totalLength += shortRecord ? sizeof(uint8_t) : sizeof(uint32_t); /* Payload length */
    if(idLength > 0)
        totalLength += 1;
    totalLength += typeLength + idLength + payloadLength;
    /* ===================================== */

    //NDEF_RecordType *rec = malloc(sizeof(NDEF_RecordType));
    uint8_t *buf = malloc(totalLength);
    if(buf == NULL)
    {
        Log_info("NDEF: Failed to allocate record for build");
        return NULL;
    }
    pos = buf; /* Set pointer to beginning of NDEF Record */

    /* ==== Ensure correct flags are set in record */
    header = flags;

    if(idLength > 0)
        header |= NDEF_RECORD_FLAG_IL;
    if(shortRecord)
        header |= NDEF_RECORD_FLAG_SR;
    
    *pos++ = header;
    *pos++ = typeLength;

    if(shortRecord)
    {
        *pos++ = (uint8_t) payloadLength;
    }
    else
    {
        memcpy(pos, (uint8_t*) &payloadLength, sizeof(payloadLength));
        pos += sizeof(payloadLength);
    }
    
    if(idLength > 0)
        *pos++ = idLength;

    if((type != NULL) && (typeLength > 0))
    {
        memcpy(pos, type, typeLength);
        pos += sizeof(typeLength);
    }
    if(id != NULL && (idLength > 0))
    {
        memcpy(pos, id, idLength);
        pos += sizeof(idLength);
    }
    if(payload != NULL && (payloadLength > 0))
    {
        memcpy(pos, payload, payloadLength);
        pos += sizeof(payloadLength);
    }
    
    return buf;
}


/* Print our NDEF Record in similar styling to http://www.ndefparser.net */
void NDEF_PrintRecord(NDEF_RecordType *rec)
{
    Log_info("======== NDEF RECORD =======");
    Log_info("HEADER: 0x%02X (MB:%d, ME:%d, CF:%d, SR:%d, IL:%d, TNF:%d)",
             rec->header,
             NDEF_RECORD_GET_FLAG(rec->header, NDEF_RECORD_FLAG_MB),
             NDEF_RECORD_GET_FLAG(rec->header, NDEF_RECORD_FLAG_ME),
             NDEF_RECORD_GET_FLAG(rec->header, NDEF_RECORD_FLAG_CF),
             NDEF_RECORD_GET_FLAG(rec->header, NDEF_RECORD_FLAG_SR),
             NDEF_RECORD_GET_FLAG(rec->header, NDEF_RECORD_FLAG_IL),
             (rec->header & NDEF_RECORD_FLAG_TNF_Msk));

    Log_info("TYPE LENGTH: 0x%02X",  rec->typeLength);
    Log_info("PAYLOAD LENGTH: 0x%X", rec->payloadLength);

    if(rec->idLength != 0)
        Log_info("ID LENGTH: 0x%02X", rec->idLength);
    else
        Log_info("ID LENGTH: No ID Length field");
    
    if (rec->typeLength != 0)
        Log_info("TYPE: \"%.*s\"", rec->typeLength, rec->type);
    else
        Log_info("TYPE: No Type field");

    if (rec->idLength != 0)
        Log_info("ID: \"%.*s\"", rec->idLength, rec->id);
    else
        Log_info("ID: No ID field");

    if (rec->payloadLength != 0)
    {
        char buf[(rec->payloadLength*2) + 15];
        char *ptr = &buf[0];

        ptr += sprintf(ptr, "PAYLOAD: 0x");
        for(size_t i = 0 ; i < rec->payloadLength ; i++)
        {
            ptr += sprintf(ptr, "%02X", rec->payload[i]);
        }
        Log_info(buf);

        Log_print("Payload as ASCII: \"", 19);
        Log_print((char*) rec->payload, rec->payloadLength);
        Log_println("\"", 1);
    }
    else
    {
        Log_info("Payload: No Payload");
    }

    Log_info("============================");
}

void NDEF_PrintMessage(NDEF_MessageType *msg)
{
    size_t noOfRecords = msg->noOfRecords;
    NDEF_RecordType *rec = msg->rec;

    while(noOfRecords--)
    {
        NDEF_PrintRecord(rec++);
    }   
}


// void NDEF_DestroyMessage(NDEF_MessageType *msg)
// {
//     while(msg->noOfRecords--)
//     {
//         NDEF_DestroyRecord(msg->rec);
//     }
// }

// void NDEF_DestoryRecord(NDEF_RecordType *rec)
// {
//     if(rec->id != NULL)
//         free(rec->id);

//     if(rec->type != NULL)
//         free(rec->type);

//     if(rec->payload != NULL)
//         free(rec->payload);
// }

// static void* byteCopy(void *dst, const void *src, size_t len)
// {
//     uint8_t *d = dst;
//     const uint8_t *s = src;
//     while(len--)
//     {
//         *d++ = *s++;
//     }
//     return dst;
// }

// static uint8_t* createBuffer(size_t size, const uint8_t *src)
// {
//     uint8_t *ptr = malloc(size);
//     if(ptr != NULL)
//     {
//         byteCopy(ptr, src, size);
//     }
//     return ptr;
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
    
