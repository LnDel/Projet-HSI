/**
 * \file    decode.h
 * \brief   Decoding functions for COMODO and MUX frames to BCGV
 * \details This file contains declarations for functions related to decoding frames received
 * \author  Hélène Delran-Garric
 */

#ifndef DECODE_FRAMES_H
#define DECODE_FRAMES_H

#include <stdint.h>
#include "bcgv_lib.h"

/**
 * \brief Decodes the COMODO -> BCGV frame and updates the corresponding application data.
 * \param[in] frame A pointer to the COMODO frame.
 */
void decode_comodo_to_bcgv(uint8_t* frame);

/**
 * \brief Decodes a MUX -> BCGV frame and updates the corresponding application data.
 *        Ensures that the frame's CRC8 is valid before decoding.
 * \param[in] frame A pointer to the MUX frame.
*  \return is_valid_frame_t Returns VALID if the frame is valid (correct CRC), otherwise INVALID.
 */
is_valid_frame_t decode_mux_to_bcgv(uint8_t* frame);

#endif // DECODE_FRAMES_H
