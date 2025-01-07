#include <stdint.h>
#include "bcgv_lib.h"

#define FRAME_SIZE 2

void encode_bcgv_to_bgf1(uint8_t* frame) {

    frame[0] = get_idMsgBcgvToBgf1();

    activation_t activationPositionLights = get_activationPositionLights();
    frame[1] = (activationPositionLights == ON) ? 0x01 : 0x00;
}

void encode_bcgv_to_bgf2(uint8_t* frame) {

    frame[0] = get_idMsgBcgvToBgf2();

    activation_t activationLowBeams = get_activationLowBeams();
    frame[1] = (activationLowBeams == ON) ? 0x01 : 0x00;
}

void encode_bcgv_to_bgf3(uint8_t* frame) {

    frame[0] = get_idMsgBcgvToBgf3();

    activation_t activationHighBeams = get_activationHighBeams();
    frame[1] = (activationHighBeams == ON) ? 0x01 : 0x00;
}

void encode_bcgv_to_bgf4(uint8_t* frame) {

    frame[0] = get_idMsgBcgvToBgf4();

    activation_t activationRightTurnSignal = get_activationRightTurnSignal();
    frame[1] = (activationRightTurnSignal == ON) ? 0x01 : 0x00;
}

void encode_bcgv_to_bgf5(uint8_t* frame) {

    frame[0] = get_idMsgBcgvToBgf5();

    activation_t activationLeftTurnSignal = get_activationLeftTurnSignal();
    frame[1] = (activationLeftTurnSignal == ON) ? 0x01 : 0x00;
}

int verify_acknowledgment(const uint8_t* sent_frame, const uint8_t* received_frame) {

    for (int i = 0; i < FRAME_SIZE; i++) {
        if (sent_frame[i] != received_frame[i]) {
            return 0;
        }
    }
    return 1;
}


