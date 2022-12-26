/*
Serial Line Internet Protocol
 */

#include "slip.h"
#include "byte_order.h"

#define SLIP_SPECIAL_BYTE_END           0xC0U
#define SLIP_SPECIAL_BYTE_ESC           0xDBU

#define SLIP_ESCAPED_BYTE_END           0xDCU
#define SLIP_ESCAPED_BYTE_ESC           0xDDU


static const u16 crc_table[256] = {
    0x0000U, 0x1021U, 0x2042U, 0x3063U, 0x4084U, 0x50A5U, 0x60C6U, 0x70E7U,
    0x8108U, 0x9129U, 0xA14AU, 0xB16BU, 0xC18CU, 0xD1ADU, 0xE1CEU, 0xF1EFU,
    0x1231U, 0x0210U, 0x3273U, 0x2252U, 0x52B5U, 0x4294U, 0x72F7U, 0x62D6U,
    0x9339U, 0x8318U, 0xB37BU, 0xA35AU, 0xD3BDU, 0xC39CU, 0xF3FFU, 0xE3DEU,
    0x2462U, 0x3443U, 0x0420U, 0x1401U, 0x64E6U, 0x74C7U, 0x44A4U, 0x5485U,
    0xA56AU, 0xB54BU, 0x8528U, 0x9509U, 0xE5EEU, 0xF5CFU, 0xC5ACU, 0xD58DU,
    0x3653U, 0x2672U, 0x1611U, 0x0630U, 0x76D7U, 0x66F6U, 0x5695U, 0x46B4U,
    0xB75BU, 0xA77AU, 0x9719U, 0x8738U, 0xF7DFU, 0xE7FEU, 0xD79DU, 0xC7BCU,
    0x48C4U, 0x58E5U, 0x6886U, 0x78A7U, 0x0840U, 0x1861U, 0x2802U, 0x3823U,
    0xC9CCU, 0xD9EDU, 0xE98EU, 0xF9AFU, 0x8948U, 0x9969U, 0xA90AU, 0xB92BU,
    0x5AF5U, 0x4AD4U, 0x7AB7U, 0x6A96U, 0x1A71U, 0x0A50U, 0x3A33U, 0x2A12U,
    0xDBFDU, 0xCBDCU, 0xFBBFU, 0xEB9EU, 0x9B79U, 0x8B58U, 0xBB3BU, 0xAB1AU,
    0x6CA6U, 0x7C87U, 0x4CE4U, 0x5CC5U, 0x2C22U, 0x3C03U, 0x0C60U, 0x1C41U,
    0xEDAEU, 0xFD8FU, 0xCDECU, 0xDDCDU, 0xAD2AU, 0xBD0BU, 0x8D68U, 0x9D49U,
    0x7E97U, 0x6EB6U, 0x5ED5U, 0x4EF4U, 0x3E13U, 0x2E32U, 0x1E51U, 0x0E70U,
    0xFF9FU, 0xEFBEU, 0xDFDDU, 0xCFFCU, 0xBF1BU, 0xAF3AU, 0x9F59U, 0x8F78U,
    0x9188U, 0x81A9U, 0xB1CAU, 0xA1EBU, 0xD10CU, 0xC12DU, 0xF14EU, 0xE16FU,
    0x1080U, 0x00A1U, 0x30C2U, 0x20E3U, 0x5004U, 0x4025U, 0x7046U, 0x6067U,
    0x83B9U, 0x9398U, 0xA3FBU, 0xB3DAU, 0xC33DU, 0xD31CU, 0xE37FU, 0xF35EU,
    0x02B1U, 0x1290U, 0x22F3U, 0x32D2U, 0x4235U, 0x5214U, 0x6277U, 0x7256U,
    0xB5EAU, 0xA5CBU, 0x95A8U, 0x8589U, 0xF56EU, 0xE54FU, 0xD52CU, 0xC50DU,
    0x34E2U, 0x24C3U, 0x14A0U, 0x0481U, 0x7466U, 0x6447U, 0x5424U, 0x4405U,
    0xA7DBU, 0xB7FAU, 0x8799U, 0x97B8U, 0xE75FU, 0xF77EU, 0xC71DU, 0xD73CU,
    0x26D3U, 0x36F2U, 0x0691U, 0x16B0U, 0x6657U, 0x7676U, 0x4615U, 0x5634U,
    0xD94CU, 0xC96DU, 0xF90EU, 0xE92FU, 0x99C8U, 0x89E9U, 0xB98AU, 0xA9ABU,
    0x5844U, 0x4865U, 0x7806U, 0x6827U, 0x18C0U, 0x08E1U, 0x3882U, 0x28A3U,
    0xCB7DU, 0xDB5CU, 0xEB3FU, 0xFB1EU, 0x8BF9U, 0x9BD8U, 0xABBBU, 0xBB9AU,
    0x4A75U, 0x5A54U, 0x6A37U, 0x7A16U, 0x0AF1U, 0x1AD0U, 0x2AB3U, 0x3A92U,
    0xFD2EU, 0xED0FU, 0xDD6CU, 0xCD4DU, 0xBDAAU, 0xAD8BU, 0x9DE8U, 0x8DC9U,
    0x7C26U, 0x6C07U, 0x5C64U, 0x4C45U, 0x3CA2U, 0x2C83U, 0x1CE0U, 0x0CC1U,
    0xEF1FU, 0xFF3EU, 0xCF5DU, 0xDF7CU, 0xAF9BU, 0xBFBAU, 0x8FD9U, 0x9FF8U,
    0x6E17U, 0x7E36U, 0x4E55U, 0x5E74U, 0x2E93U, 0x3EB2U, 0x0ED1U, 0x1EF0U
};

static u16 calc_crc_ccitt(const u8 byte, const u16 crc_old)
{
    const u8 index = (u8)(byte ^ (crc_old >> 8U));
    const u16 crc = (u16)(crc_table[index] ^ (crc_old << 8U));

    return crc;
}

static void reset_rx(slip_handler_s* const slip)
{
    assert(slip != NULL);

    slip->state = SLIP_STATE_NORMAL;
    slip->size  = 0;
    slip->crc   = slip->descriptor->crc_seed;
}

slip_error_t slip_init(slip_handler_s* const slip, const slip_descriptor_s* const descriptor)
{
    assert(slip != NULL);
    assert(descriptor != NULL);
    assert(descriptor->RX_buf != NULL);
    assert(descriptor->recv_message != NULL);
    assert(descriptor->write_byte != NULL);

    slip->descriptor = descriptor;
    reset_rx(slip);

    return SLIP_NO_ERROR;
}

static slip_error_t put_byte_to_buffer(slip_handler_s* const slip, const u8 byte)
{
    slip_error_t error = SLIP_NO_ERROR;

    assert(slip != NULL);

    if (slip->size >= slip->descriptor->RX_buf_size) {
        error = SLIP_ERROR_BUFFER_OVERFLOW;
        reset_rx(slip);
    } else {
        slip->descriptor->RX_buf[slip->size++] = byte;
        slip->crc = calc_crc_ccitt(byte, slip->crc);
        slip->state = SLIP_STATE_NORMAL;
    }

    return error;
}

slip_error_t slip_read_byte(slip_handler_s* const slip, u8 byte)
{
    slip_error_t error = SLIP_NO_ERROR;

    assert(slip != NULL);

    switch (slip->state) {
    case SLIP_STATE_NORMAL:
        switch (byte) {
        case SLIP_SPECIAL_BYTE_END:
            if (slip->size >= 2U) {
                if (slip->crc == 0U) {
                    slip->descriptor->recv_message(slip->descriptor->RX_buf, (slip->size - 2U));
                } else {
                    error = SLIP_ERROR_CRC_MISMATCH;
                }
            }
            reset_rx(slip);
            break;
        case SLIP_SPECIAL_BYTE_ESC:
            slip->state = SLIP_STATE_ESCAPED;
            break;
        default:
            error = put_byte_to_buffer(slip, byte);
            break;
        }
        break;

    case SLIP_STATE_ESCAPED:
        switch (byte) {
        case SLIP_ESCAPED_BYTE_END:
            byte = SLIP_SPECIAL_BYTE_END;
            break;
        case SLIP_ESCAPED_BYTE_ESC:
            byte = SLIP_SPECIAL_BYTE_ESC;
            break;
        default:
            error = SLIP_ERROR_UNKNOWN_ESCAPED_BYTE;
            reset_rx(slip);
            break;
        }

        if (error != SLIP_NO_ERROR) {
            break;
        }

        error = put_byte_to_buffer(slip, byte);
        break;
    }

    return error;
}

static u8 write_encoded_byte(slip_handler_s* const slip, u8 byte)
{
    u8 escape = 0;

    assert(slip != NULL);

    switch (byte) {
    case SLIP_SPECIAL_BYTE_END:
        byte = SLIP_ESCAPED_BYTE_END;
        escape = 1;
        break;
    case SLIP_SPECIAL_BYTE_ESC:
        byte = SLIP_ESCAPED_BYTE_ESC;
        escape = 1;
        break;
    }

    if (escape != 0) {
        if (slip->descriptor->write_byte(SLIP_SPECIAL_BYTE_ESC) == 0) {
            return 0;
        }
    }

    if (slip->descriptor->write_byte(byte) == 0) {
        return 0;
    }

    return 1;
}

slip_error_t slip_send_message(slip_handler_s* const slip, const u8* const data, const reg size)
{
    u8 byte;
    u16 crc;

    assert(data != NULL);
    assert(slip != NULL);

    if (slip->descriptor->write_byte(SLIP_SPECIAL_BYTE_END) == 0) {
        return SLIP_ERROR_BUFFER_OVERFLOW;
    }

    crc = slip->descriptor->crc_seed;

    for (reg i = 0; i < size; ++i) {
        byte = data[i];
        if (write_encoded_byte(slip, byte) == 0) {
            return SLIP_ERROR_BUFFER_OVERFLOW;
        }
        crc = calc_crc_ccitt(byte, crc);
    }

    crc = LittleEndianU16(crc);

    if (write_encoded_byte(slip, (u8) (crc >> 8U)) == 0) {
        return SLIP_ERROR_BUFFER_OVERFLOW;
    }

    if (write_encoded_byte(slip, (u8) (crc & 0xFFU)) == 0) {
        return SLIP_ERROR_BUFFER_OVERFLOW;
    }

    if (slip->descriptor->write_byte(SLIP_SPECIAL_BYTE_END) == 0) {
        return SLIP_ERROR_BUFFER_OVERFLOW;
    }

    return SLIP_NO_ERROR;
}


#undef SLIP_SPECIAL_BYTE_END
#undef SLIP_SPECIAL_BYTE_ESC

#undef SLIP_ESCAPED_BYTE_END
#undef SLIP_ESCAPED_BYTE_ESC
