#include <stdint.h>
#include <stdio.h>
#include "checksum.h"
#include "bcgv_lib.h" 

void decode_comodo_to_bcgv(uint8_t*);
void decode_mux_to_bcgv(uint8_t*);

void decode_comodo_to_bcgv(uint8_t* frame) {

    set_cmdWarning(frame[0]);               
    set_cmdPositionLights(frame[1]);
    set_cmdLowBeams(frame[2]);
    set_cmdHighBeams(frame[3]);      
    set_cmdRightTurnSignal(frame[4]);
    set_cmdLeftTurnSignal(frame[5]);
    set_cmdWindShieldWiper(frame[6]);
    set_cmdWindShieldWasher(frame[7]);
}

void decode_mux_to_bcgv(uint8_t* frame) {

    // Check if the CRC8 is valid
    crc_t received_crc = frame[14];
    crc_t calculated_crc = crc_8(frame, 14);

    if (received_crc != calculated_crc) {
        printf("CRC8 mismatch: received 0x%02X, calculated 0x%02X\n", received_crc, calculated_crc);
        return;
    }

    set_frameNumber(frame[0]);

    mileage_t mileage = (frame[1] << 24) | (frame[2] << 16) | (frame[3] << 8) | frame[4];
    set_mileage(mileage);

    set_speed(frame[5]);

    set_problemChassis(frame[6]);

    set_problemMotor(frame[7]);

    set_tankLevel(frame[8]);

    rpm_t rpm =  (frame[9] << 24) | (frame[10] << 16) | (frame[11] << 8) | frame[12];
    set_rpm(rpm);

    set_problemBattery(frame[13]);

    set_receivedCrc8(received_crc);
}
