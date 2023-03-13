/*
Serial Line Internet Protocol
 */

#ifndef SLIP_H
#define SLIP_H

#include "my_ctypes.h"
#include "slip_port.h"

typedef enum {
        SLIP_STATE_NORMAL = 0x00,
        SLIP_STATE_ESCAPED
} slip_state_t;

typedef enum {
        SLIP_NO_ERROR = 0x00,
        SLIP_ERROR_BUFFER_OVERFLOW,
        SLIP_ERROR_UNKNOWN_ESCAPED_BYTE,
        SLIP_ERROR_CRC_MISMATCH
} slip_error_t;

typedef struct {
        u8* const RX_buf;
        const reg RX_buf_size;
        const u16 crc_seed;
        
        void (* const recv_message)(u8* data, reg size);
        u8 (* const write_byte)(u8 byte);
} slip_descriptor_s;

typedef struct {
        slip_state_t state;
        reg size;
        u16 crc;
        const slip_descriptor_s* descriptor;
} slip_handler_s;

slip_error_t slip_init(slip_handler_s* const slip, const slip_descriptor_s* const descriptor);
slip_error_t slip_read_byte(slip_handler_s * const slip, u8 byte);
slip_error_t slip_send_message(slip_handler_s * const slip, const u8 * const data, const reg size);

#endif /* SLIP_H */

