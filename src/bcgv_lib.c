/**
 * \file bcgv_lib.c
 * \brief Main BCGV application.
 * \details This file contains the library functions like getters/setters
 * \author Warren Anderson, Samy Belbouab, Hélène Delran--Garric
 */



#include <stdint.h>

typedef uint8_t cmd_t;

typedef uint8_t frame_num_t;

typedef uint32_t mileage_t;

typedef uint8_t speed_t;

typedef uint8_t tank_level_t;

typedef enum 
 { NO_CHASSIS_PROBLEM = 0, PRESSURE_TIRES = 1, CAR_BRAKE = 2 } chassis_pb_t;

typedef enum 
 { NO_MOTOR_PROBLEM = 0, PRESSURE_DEFECT = 1, COOLANT = 2, OVERHEATING_OIL = 3 } motor_pb_t;

typedef uint32_t rpm_t;

typedef enum 
 { NO_BATTERY_PROBLEM = 0, DISCHARGED = 1, FAILURE = 2 } battery_pb_t;

typedef uint8_t crc_t;

typedef uint8_t id_msg_t;

typedef enum 
 { OFF = 0, ON = 1 } activation_t;

typedef uint8_t indicator_t;

typedef uint32_t display_mileage_t;

typedef uint32_t display_speed_t;

typedef uint32_t display_tank_level_t;

typedef uint32_t display_rpm_t;

typedef enum 
 { INVALID = 0, VALID = 1 } is_valid_frame_t;

typedef struct {
    cmd_t cmdWarning;
    cmd_t cmdPositionLights;
    cmd_t cmdLowBeams;
    cmd_t cmdHighBeams;
    cmd_t cmdRightTurnSignal;
    cmd_t cmdLeftTurnSignal;
    cmd_t cmdWindShieldWiper;
    cmd_t cmdWindShieldWasher;
    frame_num_t frameNumber;
    mileage_t mileage;
    speed_t speed;
    chassis_pb_t problemChassis;
    motor_pb_t problemMotor;
    tank_level_t tankLevel;
    rpm_t rpm;
    battery_pb_t problemBattery;
    crc_t receivedCrc8;
    id_msg_t idMsgBcgvToBgf1;
    id_msg_t idMsgBcgvToBgf2;
    id_msg_t idMsgBcgvToBgf3;
    id_msg_t idMsgBcgvToBgf4;
    id_msg_t idMsgBcgvToBgf5;
    activation_t activationPositionLights;
    activation_t activationLowBeams;
    activation_t activationHighBeams;
    activation_t activationRightTurnSignal;
    activation_t activationLeftTurnSignal;
    indicator_t indicatorPositionLights;
    indicator_t indicatorLowBeams;
    indicator_t indicatorHighBeams;
    indicator_t indicatorFuel;
    indicator_t indicatorMotorPb;
    indicator_t indicatorTirePressure;
    indicator_t indicatorDischargedBattery;
    indicator_t indicatorWarning;
    indicator_t indicatorBatteryFailure;
    indicator_t indicatorTempLDR;
    indicator_t indicatorMotorPressure;
    indicator_t indicatorOilOverheating;
    indicator_t indicatorBrakeFailure;
    activation_t activationShieldWiper;
    activation_t activationShieldWasher;
    display_mileage_t displayMileage;
    display_speed_t displaySpeed;
    display_tank_level_t displayTankLevel;
    display_rpm_t displayRpm;
    indicator_t indicatorRightTurnSignal;
    indicator_t indicatorLeftTurnSignal;
    indicator_t indicatorShieldWiper;
    indicator_t indicatorShieldWasher;
} BCGV_Data_t;

static BCGV_Data_t bcgv_data;

cmd_t get_cmdWarning(void) { return bcgv_data.cmdWarning; }

cmd_t get_cmdPositionLights(void) { return bcgv_data.cmdPositionLights; }

cmd_t get_cmdLowBeams(void) { return bcgv_data.cmdLowBeams; }

cmd_t get_cmdHighBeams(void) { return bcgv_data.cmdHighBeams; }

cmd_t get_cmdRightTurnSignal(void) { return bcgv_data.cmdRightTurnSignal; }

cmd_t get_cmdLeftTurnSignal(void) { return bcgv_data.cmdLeftTurnSignal; }

cmd_t get_cmdWindShieldWiper(void) { return bcgv_data.cmdWindShieldWiper; }

cmd_t get_cmdWindShieldWasher(void) { return bcgv_data.cmdWindShieldWasher; }

frame_num_t get_frameNumber(void) { return bcgv_data.frameNumber; }

mileage_t get_mileage(void) { return bcgv_data.mileage; }

speed_t get_speed(void) { return bcgv_data.speed; }

chassis_pb_t get_problemChassis(void) { return bcgv_data.problemChassis; }

motor_pb_t get_problemMotor(void) { return bcgv_data.problemMotor; }

tank_level_t get_tankLevel(void) { return bcgv_data.tankLevel; }

rpm_t get_rpm(void) { return bcgv_data.rpm; }

battery_pb_t get_problemBattery(void) { return bcgv_data.problemBattery; }

crc_t get_receivedCrc8(void) { return bcgv_data.receivedCrc8; }

id_msg_t get_idMsgBcgvToBgf1(void) { return bcgv_data.idMsgBcgvToBgf1; }

id_msg_t get_idMsgBcgvToBgf2(void) { return bcgv_data.idMsgBcgvToBgf2; }

id_msg_t get_idMsgBcgvToBgf3(void) { return bcgv_data.idMsgBcgvToBgf3; }

id_msg_t get_idMsgBcgvToBgf4(void) { return bcgv_data.idMsgBcgvToBgf4; }

id_msg_t get_idMsgBcgvToBgf5(void) { return bcgv_data.idMsgBcgvToBgf5; }

activation_t get_activationPositionLights(void) { return bcgv_data.activationPositionLights; }

activation_t get_activationLowBeams(void) { return bcgv_data.activationLowBeams; }

activation_t get_activationHighBeams(void) { return bcgv_data.activationHighBeams; }

activation_t get_activationRightTurnSignal(void) { return bcgv_data.activationRightTurnSignal; }

activation_t get_activationLeftTurnSignal(void) { return bcgv_data.activationLeftTurnSignal; }

indicator_t get_indicatorPositionLights(void) { return bcgv_data.indicatorPositionLights; }

indicator_t get_indicatorLowBeams(void) { return bcgv_data.indicatorLowBeams; }

indicator_t get_indicatorHighBeams(void) { return bcgv_data.indicatorHighBeams; }

indicator_t get_indicatorFuel(void) { return bcgv_data.indicatorFuel; }

indicator_t get_indicatorMotorPb(void) { return bcgv_data.indicatorMotorPb; }

indicator_t get_indicatorTirePressure(void) { return bcgv_data.indicatorTirePressure; }

indicator_t get_indicatorDischargedBattery(void) { return bcgv_data.indicatorDischargedBattery; }

indicator_t get_indicatorWarning(void) { return bcgv_data.indicatorWarning; }

indicator_t get_indicatorBatteryFailure(void) { return bcgv_data.indicatorBatteryFailure; }

indicator_t get_indicatorTempLDR(void) { return bcgv_data.indicatorTempLDR; }

indicator_t get_indicatorMotorPressure(void) { return bcgv_data.indicatorMotorPressure; }

indicator_t get_indicatorOilOverheating(void) { return bcgv_data.indicatorOilOverheating; }

indicator_t get_indicatorBrakeFailure(void) { return bcgv_data.indicatorBrakeFailure; }

activation_t get_activationShieldWiper(void) { return bcgv_data.activationShieldWiper; }

activation_t get_activationShieldWasher(void) { return bcgv_data.activationShieldWasher; }

display_mileage_t get_displayMileage(void) { return bcgv_data.displayMileage; }

display_speed_t get_displaySpeed(void) { return bcgv_data.displaySpeed; }

display_tank_level_t get_displayTankLevel(void) { return bcgv_data.displayTankLevel; }

display_rpm_t get_displayRpm(void) { return bcgv_data.displayRpm; }

indicator_t get_indicatorRightTurnSignal(void) { return bcgv_data.indicatorRightTurnSignal; }

indicator_t get_indicatorLeftTurnSignal(void) { return bcgv_data.indicatorLeftTurnSignal; }

indicator_t get_indicatorShieldWiper(void) { return bcgv_data.indicatorShieldWiper; }

indicator_t get_indicatorShieldWasher(void) { return bcgv_data.indicatorShieldWasher; }

short int set_cmdWarning(cmd_t value) {
    if (value <= 1) {
        bcgv_data.cmdWarning = value;
        return 1;
    }
    return 0;
}

short int set_cmdPositionLights(cmd_t value) {
    if (value <= 1) {
        bcgv_data.cmdPositionLights = value;
        return 1;
    }
    return 0;
}

short int set_cmdLowBeams(cmd_t value) {
    if (value <= 1) {
        bcgv_data.cmdLowBeams = value;
        return 1;
    }
    return 0;
}

short int set_cmdHighBeams(cmd_t value) {
    if (value <= 1) {
        bcgv_data.cmdHighBeams = value;
        return 1;
    }
    return 0;
}

short int set_cmdRightTurnSignal(cmd_t value) {
    if (value <= 1) {
        bcgv_data.cmdRightTurnSignal = value;
        return 1;
    }
    return 0;
}

short int set_cmdLeftTurnSignal(cmd_t value) {
    if (value <= 1) {
        bcgv_data.cmdLeftTurnSignal = value;
        return 1;
    }
    return 0;
}

short int set_cmdWindShieldWiper(cmd_t value) {
    if (value <= 1) {
        bcgv_data.cmdWindShieldWiper = value;
        return 1;
    }
    return 0;
}

short int set_cmdWindShieldWasher(cmd_t value) {
    if (value <= 1) {
        bcgv_data.cmdWindShieldWasher = value;
        return 1;
    }
    return 0;
}

short int set_frameNumber(frame_num_t value) {
    if (value <= 100) {
        bcgv_data.frameNumber = value;
        return 1;
    }
    return 0;
}

short int set_mileage(mileage_t value) {
    if (value <= 500000) {
        bcgv_data.mileage = value;
        return 1;
    }
    return 0;
}

short int set_speed(speed_t value) {
    if (value <= 254) {
        bcgv_data.speed = value;
        return 1;
    }
    return 0;
}

short int set_problemChassis(chassis_pb_t value) {
    if (value <= 2) {
        bcgv_data.problemChassis = value;
        return 1;
    }
    return 0;
}

short int set_problemMotor(motor_pb_t value) {
    if (value <= 3) {
        bcgv_data.problemMotor = value;
        return 1;
    }
    return 0;
}

short int set_tankLevel(tank_level_t value) {
    if (value <= 40) {
        bcgv_data.tankLevel = value;
        return 1;
    }
    return 0;
}

short int set_rpm(rpm_t value) {
    if (value <= 10000) {
        bcgv_data.rpm = value;
        return 1;
    }
    return 0;
}

short int set_problemBattery(battery_pb_t value) {
    if (value <= 2) {
        bcgv_data.problemBattery = value;
        return 1;
    }
    return 0;
}

short int set_receivedCrc8(crc_t value) {
    if (value <= 254) {
        bcgv_data.receivedCrc8 = value;
        return 1;
    }
    return 0;
}

short int set_idMsgBcgvToBgf1(id_msg_t value) {
    if (value >= 1 && value <= 5) {
        bcgv_data.idMsgBcgvToBgf1 = value;
        return 1;
    }
    return 0;
}

short int set_idMsgBcgvToBgf2(id_msg_t value) {
    if (value >= 1 && value <= 5) {
        bcgv_data.idMsgBcgvToBgf2 = value;
        return 1;
    }
    return 0;
}

short int set_idMsgBcgvToBgf3(id_msg_t value) {
    if (value >= 1 && value <= 5) {
        bcgv_data.idMsgBcgvToBgf3 = value;
        return 1;
    }
    return 0;
}

short int set_idMsgBcgvToBgf4(id_msg_t value) {
    if (value >= 1 && value <= 5) {
        bcgv_data.idMsgBcgvToBgf4 = value;
        return 1;
    }
    return 0;
}

short int set_idMsgBcgvToBgf5(id_msg_t value) {
    if (value >= 1 && value <= 5) {
        bcgv_data.idMsgBcgvToBgf5 = value;
        return 1;
    }
    return 0;
}

short int set_activationPositionLights(activation_t value) {
    if (value <= 1) {
        bcgv_data.activationPositionLights = value;
        return 1;
    }
    return 0;
}

short int set_activationLowBeams(activation_t value) {
    if (value <= 1) {
        bcgv_data.activationLowBeams = value;
        return 1;
    }
    return 0;
}

short int set_activationHighBeams(activation_t value) {
    if (value <= 1) {
        bcgv_data.activationHighBeams = value;
        return 1;
    }
    return 0;
}

short int set_activationRightTurnSignal(activation_t value) {
    if (value <= 1) {
        bcgv_data.activationRightTurnSignal = value;
        return 1;
    }
    return 0;
}

short int set_activationLeftTurnSignal(activation_t value) {
    if (value <= 1) {
        bcgv_data.activationLeftTurnSignal = value;
        return 1;
    }
    return 0;
}

short int set_indicatorPositionLights(indicator_t value) {
    if (value <= 1) {
        bcgv_data.indicatorPositionLights = value;
        return 1;
    }
    return 0;
}

short int set_indicatorLowBeams(indicator_t value) {
    if (value <= 1) {
        bcgv_data.indicatorLowBeams = value;
        return 1;
    }
    return 0;
}

short int set_indicatorHighBeams(indicator_t value) {
    if (value <= 1) {
        bcgv_data.indicatorHighBeams = value;
        return 1;
    }
    return 0;
}

short int set_indicatorFuel(indicator_t value) {
    if (value <= 1) {
        bcgv_data.indicatorFuel = value;
        return 1;
    }
    return 0;
}

short int set_indicatorMotorPb(indicator_t value) {
    if (value <= 1) {
        bcgv_data.indicatorMotorPb = value;
        return 1;
    }
    return 0;
}

short int set_indicatorTirePressure(indicator_t value) {
    if (value <= 1) {
        bcgv_data.indicatorTirePressure = value;
        return 1;
    }
    return 0;
}

short int set_indicatorDischargedBattery(indicator_t value) {
    if (value <= 1) {
        bcgv_data.indicatorDischargedBattery = value;
        return 1;
    }
    return 0;
}

short int set_indicatorWarning(indicator_t value) {
    if (value <= 1) {
        bcgv_data.indicatorWarning = value;
        return 1;
    }
    return 0;
}

short int set_indicatorBatteryFailure(indicator_t value) {
    if (value <= 1) {
        bcgv_data.indicatorBatteryFailure = value;
        return 1;
    }
    return 0;
}

short int set_indicatorTempLDR(indicator_t value) {
    if (value <= 1) {
        bcgv_data.indicatorTempLDR = value;
        return 1;
    }
    return 0;
}

short int set_indicatorMotorPressure(indicator_t value) {
    if (value <= 1) {
        bcgv_data.indicatorMotorPressure = value;
        return 1;
    }
    return 0;
}

short int set_indicatorOilOverheating(indicator_t value) {
    if (value <= 1) {
        bcgv_data.indicatorOilOverheating = value;
        return 1;
    }
    return 0;
}

short int set_indicatorBrakeFailure(indicator_t value) {
    if (value <= 1) {
        bcgv_data.indicatorBrakeFailure = value;
        return 1;
    }
    return 0;
}

short int set_activationShieldWiper(activation_t value) {
    if (value <= 1) {
        bcgv_data.activationShieldWiper = value;
        return 1;
    }
    return 0;
}

short int set_activationShieldWasher(activation_t value) {
    if (value <= 1) {
        bcgv_data.activationShieldWasher = value;
        return 1;
    }
    return 0;
}

short int set_displayMileage(display_mileage_t value) {
    if (value <= 500000) {
        bcgv_data.displayMileage = value;
        return 1;
    }
    return 0;
}

short int set_displaySpeed(display_speed_t value) {
    if (value <= 200) {
        bcgv_data.displaySpeed = value;
        return 1;
    }
    return 0;
}

short int set_displayTankLevel(display_tank_level_t value) {
    if (value <= 500) {
        bcgv_data.displayTankLevel = value;
        return 1;
    }
    return 0;
}

short int set_displayRpm(display_rpm_t value) {
    if (value <= 10000) {
        bcgv_data.displayRpm = value;
        return 1;
    }
    return 0;
}

short int set_indicatorRightTurnSignal(indicator_t value) {
    if (value <= 1) {
        bcgv_data.indicatorRightTurnSignal = value;
        return 1;
    }
    return 0;
}

short int set_indicatorLeftTurnSignal(indicator_t value) {
    if (value <= 1) {
        bcgv_data.indicatorLeftTurnSignal = value;
        return 1;
    }
    return 0;
}

short int set_indicatorShieldWiper(indicator_t value) {
    if (value <= 1) {
        bcgv_data.indicatorShieldWiper = value;
        return 1;
    }
    return 0;
}

short int set_indicatorShieldWasher(indicator_t value) {
    if (value <= 1) {
        bcgv_data.indicatorShieldWasher = value;
        return 1;
    }
    return 0;
}

void init_BCGV_Data(void) {
    bcgv_data.cmdWarning = 0;
    bcgv_data.cmdPositionLights = 0;
    bcgv_data.cmdLowBeams = 0;
    bcgv_data.cmdHighBeams = 0;
    bcgv_data.cmdRightTurnSignal = 0;
    bcgv_data.cmdLeftTurnSignal = 0;
    bcgv_data.cmdWindShieldWiper = 0;
    bcgv_data.cmdWindShieldWasher = 0;
    bcgv_data.frameNumber = 0;
    bcgv_data.mileage = 0;
    bcgv_data.speed = 0;
    bcgv_data.problemChassis = 0;
    bcgv_data.problemMotor = 0;
    bcgv_data.tankLevel = 0;
    bcgv_data.rpm = 0;
    bcgv_data.problemBattery = 0;
    bcgv_data.receivedCrc8 = 0;
    bcgv_data.idMsgBcgvToBgf1 = 1;
    bcgv_data.idMsgBcgvToBgf2 = 2;
    bcgv_data.idMsgBcgvToBgf3 = 3;
    bcgv_data.idMsgBcgvToBgf4 = 4;
    bcgv_data.idMsgBcgvToBgf5 = 5;
    bcgv_data.activationPositionLights = 0;
    bcgv_data.activationLowBeams = 0;
    bcgv_data.activationHighBeams = 0;
    bcgv_data.activationRightTurnSignal = 0;
    bcgv_data.activationLeftTurnSignal = 0;
    bcgv_data.indicatorPositionLights = 0;
    bcgv_data.indicatorLowBeams = 0;
    bcgv_data.indicatorHighBeams = 0;
    bcgv_data.indicatorFuel = 0;
    bcgv_data.indicatorMotorPb = 0;
    bcgv_data.indicatorTirePressure = 0;
    bcgv_data.indicatorDischargedBattery = 0;
    bcgv_data.indicatorWarning = 0;
    bcgv_data.indicatorBatteryFailure = 0;
    bcgv_data.indicatorTempLDR = 0;
    bcgv_data.indicatorMotorPressure = 0;
    bcgv_data.indicatorOilOverheating = 0;
    bcgv_data.indicatorBrakeFailure = 0;
    bcgv_data.activationShieldWiper = 0;
    bcgv_data.activationShieldWasher = 0;
    bcgv_data.displayMileage = 0;
    bcgv_data.displaySpeed = 0;
    bcgv_data.displayTankLevel = 0;
    bcgv_data.displayRpm = 0;
    bcgv_data.indicatorRightTurnSignal = 0;
    bcgv_data.indicatorLeftTurnSignal = 0;
    bcgv_data.indicatorShieldWiper = 0;
    bcgv_data.indicatorShieldWasher = 0;
}