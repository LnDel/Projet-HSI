/**
 * \file    encode.h
 * \brief   Encoding functions for COMODO and MUX frames to BCGV
 * \details This file contains declarations for functions related to encoding frames
 * \author  Hélène Delran-Garric
*/

#ifndef ENCODE_FRAMES_H
#define ENCODE_FRAMES_H

#include <stdint.h>
#include "bcgv_lib.h"

void encode_bcgv_to_bgf1(uint8_t* frame);

void encode_bcgv_to_bgf2(uint8_t* frame);

void encode_bcgv_to_bgf3(uint8_t* frame);

void encode_bcgv_to_bgf4(uint8_t* frame);

void encode_bcgv_to_bgf5(uint8_t* frame);

int verify_acknowledgment(const uint8_t* sent_frame, const uint8_t* received_frame);

void encode_bcgv_to_mux(uint8_t* frame);

#endif // ENCODE_FRAMES_H
