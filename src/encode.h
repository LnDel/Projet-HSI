/**
 * \file    encode.h
 * \brief   Decoding functions for COMODO and MUX frames to BCGV
 * \details This file contains declarations for functions related to decoding frames received
 * \author  Hélène Delran-Garric
 */

#ifndef ENCODE_FRAMES_H
#define ENCODE_FRAMES_H

#include <stdint.h>
#include "bcgv_lib.h"


void encode_bcgv_to_mux(uint8_t* frame);

#endif // DECODE_FRAMES_H

