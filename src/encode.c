#include <stdint.h>
#include "bcgv_lib.h"
#include <stdio.h>

#define FRAME_SIZE 2
#define BCGV_MUX_FRAME_SIZE 10

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

void encode_bcgv_to_mux(uint8_t* frame) {

    mileage_t mileage;
    speed_t speed;
    tank_level_t tankLevel;
    rpm_t rpm;

    for (int i = 0; i < BCGV_MUX_FRAME_SIZE; i++) {
        frame[i] = 0x00;
    }

    // first byte
    frame[0] |= (get_indicatorPositionLights() & 0x01) << 7;
    frame[0] |= (get_indicatorLowBeams() & 0x01) << 6;
    frame[0] |= (get_indicatorHighBeams() & 0x01) << 5;
    frame[0] |= (get_indicatorFuel() & 0x01) << 4;
    frame[0] |= (get_indicatorMotorPb() & 0x01) << 3;
    frame[0] |= (get_indicatorTirePressure() & 0x01) << 2;
    // Bits inutilisés (0 par défaut)
    frame[0] |= (get_indicatorDischargedBattery() & 0x01);

    // Second byte
    frame[1] |= (get_indicatorWarning() & 0x01) << 7;
    frame[1] |= (get_indicatorBatteryFailure() & 0x01) << 6;
    frame[1] |= (get_indicatorTempLDR() & 0x01) << 5;
    frame[1] |= (get_indicatorMotorPressure() & 0x01) << 4;
    frame[1] |= (get_indicatorOilOverheating() & 0x01) << 3;
    frame[1] |= (get_indicatorBrakeFailure() & 0x01) << 2;
    frame[1] |= (get_activationShieldWiper() & 0x01) << 1;
    frame[1] |= (get_activationShieldWasher() & 0x01);

    mileage = get_mileage();
    frame[2] = (mileage >> 24) & 0xFF;
    frame[3] = (mileage >> 16) & 0xFF;
    frame[4] = (mileage >> 8) & 0xFF;
    frame[5] = mileage & 0xFF;

    speed = get_speed();
    frame[6] = speed;

    tankLevel = get_tankLevel();
    frame[7] = tankLevel;

    rpm = get_rpm();
    frame[8] = (rpm >> 8) & 0xFF;
    frame[9] = rpm & 0xFF;     
}
