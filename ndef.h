#ifndef _NDEF_H_
#define _NDEF_H_

#include <stdint.h>
#include <stddef.h>

/* NDEF Record Header Flags */
#define NDEF_RECORD_FLAG_MB             (1 << 7)    /* MB: Message Begin */
#define NDEF_RECORD_FLAG_ME             (1 << 6)    /* ME: Message End */
#define NDEF_RECORD_FLAG_CF             (1 << 5)    /* CF: Chunk Flag */   
#define NDEF_RECORD_FLAG_SR             (1 << 4)    /* SR: Short Record */
#define NDEF_RECORD_FLAG_IL             (1 << 3)    /* IL: ID Length field present*/
#define NDEF_RECORD_FLAG_TNF_Msk        (7 << 0)    /* TNF Mask */
#define NDEF_RECORD_FLAG_TNF_EMPTY      (0 << 0)    /* TNF: Empty */
#define NDEF_RECORD_FLAG_TNF_WELLKNOWN  (1 << 0)    /* TNF: NFC Forum well-known type [NFC RTD] */
#define NDEF_RECORD_FLAG_TNF_MEDIA      (2 << 0)    /* TNF: Media-type as defined in RFC 2046 */
#define NDEF_RECORD_FLAG_TNF_URI        (3 << 0)    /* TNF: Absolute URI as defined in RFC 3986 */
#define NDEF_RECORD_FLAG_TNF_EXTERNAL   (4 << 0)    /* TNF: NFC Forum external type [NFC RTD] */
#define NDEF_RECORD_FLAG_TNF_UNKNOWN    (5 << 0)    /* TNF: Unknown */
#define NDEF_RECORD_FLAG_TNF_UNCHANGED  (6 << 0)    /* TNF: Unchanged */
#define NDEF_RECORD_FLAG_TNF_RESERVED   (7 << 0)    /* TNF: Reserved */

#define NDEF_RECORD_GET_FLAG(__BYTE__, __FLAG__)     ((((__BYTE__) & (__FLAG__)) == (__FLAG__)) ? 1 : 0)

/* Function Returd Codes */
typedef enum {
    NDEF_OK             = 0U,
    NDEF_ERROR          = 1U,
    NDEF_INVALID_ARGS   = 2U
} NDEF_StatusTypeDef;

/* NDEF Record structure as defined by NFC NDEF Spec */
typedef struct {

    uint8_t  header;
    uint8_t  typeLength;
    uint32_t payloadLength;
    uint8_t  idLength;
    uint8_t  *type;
    uint8_t  *id;
    uint8_t  *payload;
    /* Extra field for driver */
    size_t   totalLength;
    
} NDEF_RecordType;

typedef struct {

    size_t noOfRecords;
    NDEF_RecordType rec[10];
    /* TIP: Should use a linked-list to store records */

} NDEF_MessageType;


NDEF_StatusTypeDef NDEF_ParseRecord(uint8_t *buf, size_t size, NDEF_RecordType *rec);
NDEF_StatusTypeDef NDEF_ParseMessage(uint8_t *buf, size_t size, NDEF_MessageType *msg);

uint8_t* NDEF_BuildRecord(uint8_t flags, uint8_t *type, uint8_t typeLength, uint8_t *id, uint8_t idLength, uint8_t *payload, uint32_t payloadLength);

void NDEF_PrintRecord(NDEF_RecordType *rec);
void NDEF_PrintMessage(NDEF_MessageType *msg);



// void NDEF_DestroyMessage(NDEF_MessageType *msg);
// void NDEF_DestoryRecord(NDEF_RecordType *rec);


#endif