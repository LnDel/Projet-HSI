#ifndef DECODE_FRAMES_H
#define DECODE_FRAMES_H

#include <stdint.h>
#include "bcgv_lib.h"

/**
 * @brief Decodes a COMODO -> BCGV frame and updates the corresponding application data.
 * 
 * @param frame A single byte representing the COMODO frame.
 */
void decode_comodo_to_bcgv(serial_frame_t);

/**
 * @brief Decodes a MUX -> BCGV frame and updates the corresponding application data.
 *        Ensures that the frame's CRC8 is valid before decoding.
 * 
 * @param frame An array of 15 bytes representing the MUX frame.
 */
is_valid_frame_t decode_mux_to_bcgv(uint8_t* frame);

#endif // DECODE_FRAMES_H
