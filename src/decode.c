/**
 * \file    decode.c
 * \brief   Decoding functions for COMODO and MUX frames to BCGV
 * \details This file contains functions related to decoding frames received
 * \author  Hélène Delran-Garric
 */

#include <stdint.h>
#include <stdio.h>
#include "checksum.h"
#include "bcgv_lib.h"

void decode_comodo_to_bcgv(uint8_t*);
is_valid_frame_t decode_mux_to_bcgv(uint8_t*);

void decode_comodo_to_bcgv(uint8_t* frame) {

    set_cmdWarning((frame[0] >> 0) & 0x01);
    set_cmdPositionLights((frame[0] >> 1) & 0x01);
    set_cmdLowBeams((frame[0] >> 2) & 0x01);
    set_cmdHighBeams((frame[0] >> 3) & 0x01);
    set_cmdRightTurnSignal((frame[0] >> 4) & 0x01);
    set_cmdLeftTurnSignal((frame[0] >> 5) & 0x01);
    set_cmdWindShieldWiper((frame[0] >> 6) & 0x01);
    set_cmdWindShieldWasher((frame[0] >> 7) & 0x01);
}

is_valid_frame_t decode_mux_to_bcgv(uint8_t* frame) {

    crc_t received_crc;
    crc_t calculated_crc;
    chassis_pb_t chassisProblem;
    motor_pb_t motorProblem;
    mileage_t mileage;
    rpm_t rpm;
    battery_pb_t batteryProblem;

    // Check if the CRC8 is valid
    received_crc = frame[14];
    calculated_crc = crc_8(frame, 14);
    if (received_crc != calculated_crc) {
        printf("CRC8 mismatch: received 0x%02X, calculated 0x%02X\n", received_crc, calculated_crc);
        return INVALID;
    }

    set_frameNumber(frame[0]);

    mileage = (frame[1] << 24) | (frame[2] << 16) | (frame[3] << 8) | frame[4];
    set_mileage(mileage);

    set_speed(frame[5]);

    chassisProblem = frame[6];
    set_problemChassis(frame[6]);
    if (chassisProblem == PRESSURE_TIRES)
    {
        set_indicatorTirePressure(1);
    }
    else if (chassisProblem == CAR_BRAKE)
    {
        set_indicatorBrakeFailure(1);
    }
    else
    {
        set_problemChassis(0);
        set_indicatorTirePressure(0);
        set_indicatorBrakeFailure(0);
    }

    motorProblem = frame[7];
    set_problemMotor(frame[7]);
    if (motorProblem == PRESSURE_DEFECT)
    {
        set_indicatorMotorPressure(1);
    }
    else if (motorProblem == COOLANT)
    {
        set_indicatorTempLDR(1);
    }
    else if (motorProblem == OVERHEATING_OIL)
    {
        set_indicatorOilOverheating(1);
    }
    else{
        set_indicatorMotorPressure(0);
        set_indicatorTempLDR(0);
        set_indicatorOilOverheating(0);
    }

    set_tankLevel(frame[8]);

    rpm = (frame[9] << 24) | (frame[10] << 16) | (frame[11] << 8) | frame[12];
    set_rpm(rpm);

    batteryProblem = frame[13];
    set_problemBattery(frame[13]);
    if (batteryProblem == DISCHARGED)
    {
        set_indicatorDischargedBattery(1);
    }
    else if (batteryProblem == FAILURE)
    {
        set_indicatorBatteryFailure(1);
    }
    else{
        set_indicatorDischargedBattery(0);
        set_indicatorBatteryFailure(0);
    }

    set_receivedCrc8(received_crc);

    return VALID;
}
