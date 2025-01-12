/**
 * \file encode.h
 * \brief Encoding functions for COMODO and MUX frames to BCGV.
 * \details This file contains declarations for functions related to encoding frames.
 * \author Hélène Delran-Garric
 */

#ifndef ENCODE_FRAMES_H
#define ENCODE_FRAMES_H

#include <stdint.h>
#include "bcgv_lib.h"

/**
 * \brief Encodes BCGV data into BGF1 frame format.
 * \details This function populates a frame in the BGF1 format based on BCGV data.
 * \param frame : Pointer to the frame to be populated.
 */
void encode_bcgv_to_bgf1(uint8_t* frame);

/**
 * \brief Encodes BCGV data into BGF2 frame format.
 * \details This function populates a frame in the BGF2 format based on BCGV data.
 * \param frame : Pointer to the frame to be populated.
 */
void encode_bcgv_to_bgf2(uint8_t* frame);

/**
 * \brief Encodes BCGV data into BGF3 frame format.
 * \details This function populates a frame in the BGF3 format based on BCGV data.
 * \param frame : Pointer to the frame to be populated.
 */
void encode_bcgv_to_bgf3(uint8_t* frame);

/**
 * \brief Encodes BCGV data into BGF4 frame format.
 * \details This function populates a frame in the BGF4 format based on BCGV data.
 * \param frame : Pointer to the frame to be populated.
 */
void encode_bcgv_to_bgf4(uint8_t* frame);

/**
 * \brief Encodes BCGV data into BGF5 frame format.
 * \details This function populates a frame in the BGF5 format based on BCGV data.
 * \param frame : Pointer to the frame to be populated.
 */
void encode_bcgv_to_bgf5(uint8_t* frame);

/**
 * \brief Verifies acknowledgment by comparing sent and received frames.
 * \details Compares a sent frame with a received acknowledgment frame to ensure validity.
 * \param sent_frame : Pointer to the frame that was sent.
 * \param received_frame : Pointer to the frame that was received.
 * \return int : Returns 1 if acknowledgment is valid, 0 otherwise.
 */
int verify_acknowledgment(const uint8_t* sent_frame, const uint8_t* received_frame);

/**
 * \brief Encodes BCGV data into MUX frame format.
 * \details This function populates a frame in the MUX format based on BCGV data.
 * \param frame : Pointer to the frame to be populated.
 */
void encode_bcgv_to_mux(uint8_t* frame);

#endif // ENCODE_FRAMES_H
