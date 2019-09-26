#ifndef _TYPE_2_TAG_H_
#define _TYPE_2_TAG_H_



#define T2T_SUPPORTED_MAJOR_VERSION 1   /* Supported major version of the Type 2 Tag specification. */

#define T2T_SUPPORTED_MINOR_VERSION 2   /* Supported minor version of the Type 2 Tag specification. */

#define T2T_BLOCK_SIZE              4   /* Type 2 tag block size in bytes */

#define T2T_CC_BLOCK_OFFSET         12  /* Offset of the Capability Container in Type 2 Tag. */

#define T2T_FIRST_DATA_BLOCK_OFFSET 16  /* Offset of the data area in the Type 2 Tag. */



/**
 * @brief Descriptor for the internal bytes of a Type 2 Tag.
 */
typedef struct
{
    uint8_t     manufacturer_id;        ///< Manufacturer ID (the most significant byte of the UID/serial number).
    uint16_t    serial_number_part_1;   ///< Bytes 5-4 of the tag UID.
    uint8_t     check_byte_0;           ///< First block check character byte (XOR of the cascade tag byte, manufacturer ID byte, and the serial_number_part_1 bytes).
    uint32_t    serial_number_part_2;   ///< Bytes 3-0 of the tag UID.
    uint8_t     check_byte_1;           ///< Second block check character byte (XOR of the serial_number_part_2 bytes).
    uint8_t     internal;               ///< Tag internal bytes.
} type_2_tag_serial_number_t;

/**
 * @brief Descriptor for the Capability Container (CC) bytes of a Type 2 Tag.
 */
typedef struct
{
    uint8_t     major_version;      ///< Major version of the supported Type 2 Tag specification.
    uint8_t     minor_version;      ///< Minor version of the supported Type 2 Tag specification.
    uint16_t    data_area_size;     ///< Size of the data area in bytes.
    uint8_t     read_access;        ///< Read access for the data area.
    uint8_t     write_access;       ///< Write access for the data area.
} type_2_tag_capability_container_t;

/**
 * @brief Type 2 Tag descriptor.
 */
typedef struct
{
    type_2_tag_serial_number_t          sn;                 ///< Values within the serial number area of the tag.
    uint16_t                            lock_bytes;         ///< Value of the lock bytes.
    type_2_tag_capability_container_t   cc;                 ///< Values within the Capability Container area of the tag.

    uint16_t                      const max_tlv_blocks;     ///< Maximum number of TLV blocks that can be stored.
    tlv_block_t                       * p_tlv_block_array;  ///< Pointer to the array for TLV blocks.
    uint16_t                            tlv_count;          ///< Number of TLV blocks stored in the Type 2 Tag.

} type_2_tag_t;


/**
 * @brief Function for clearing the @ref type_2_tag_t structure.
 *
 * @param[in,out] p_type_2_tag Pointer to the structure that should be cleared.
 *
 */
void type_2_tag_clear(type_2_tag_t * p_type_2_tag);

/**
 * @brief Function for parsing raw data read from a Type 2 Tag.
 *
 * This function parses the header and the following TLV blocks of a Type 2 Tag. The data is read
 * from a buffer and stored in a @ref type_2_tag_t structure.
 *
 * @param[out] p_type_2_tag     Pointer to the structure that will be filled with parsed data.
 * @param[in]  p_raw_data       Pointer to the buffer with raw data from the tag (should
 *                              point at the first byte of the first block of the tag).
 *
 * @retval     NRF_SUCCESS      If the data was parsed successfully.
 * @retval     NRF_ERROR_NO_MEM If there is not enough memory to store all of the TLV blocks.
 * @retval     Other            If an error occurred during the parsing operation.
 *
 */
ret_code_t type_2_tag_parse(type_2_tag_t * p_type_2_tag, uint8_t * p_raw_data);

/**
 * @brief Function for printing parsed contents of the Type 2 Tag.
 *
 * @param[in] p_type_2_tag Pointer to the structure that should be printed.
 *
 */
void type_2_tag_printout(type_2_tag_t * p_type_2_tag);


#endif