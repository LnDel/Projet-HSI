#include <stdint.h>

#ifndef BCGV_LIB_H
#define BCGV_LIB_H

typedef uint8_t cmd_t;

typedef uint8_t frame_num_t;

typedef uint32_t mileage_t;

typedef uint8_t speed_t;

typedef enum { NO_CHASSIS_PROBLEM = 0, PRESSURE_TIRES = 1, CAR_BRAKE = 2 } chassis_pb_t;

typedef enum { NO_MOTOR_PROBLEM = 0, PRESSURE_DEFECT = 1, COOLANT = 2, OVERHEATING_OIL = 3 } motor_pb_t;

typedef uint8_t tank_level_t;

typedef uint32_t rpm_t;

typedef enum { NO_BATTERY_PROBLEM = 0, DISCHARGED = 1, FAILURE = 2 } battery_pb_t;

typedef uint8_t crc_t;

typedef uint8_t id_msg_t;

typedef enum { OFF = 0, ON = 1 } activation_t;

typedef uint8_t indicator_t;

typedef uint32_t display_mileage_t;

typedef uint32_t display_speed_t;

typedef uint32_t display_tank_level_t;

typedef uint32_t display_rpm_t;

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
    tank_level_t  tankLevel;
    rpm_t  rpm;
    battery_pb_t problemBattery;
    crc_t crc8;
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
} BCGV_Data_t;

cmd_t get_cmdWarning(void);

cmd_t get_cmdPositionLights(void);

cmd_t get_cmdLowBeams(void);

cmd_t get_cmdHighBeams(void);

cmd_t get_cmdRightTurnSignal(void);

cmd_t get_cmdLeftTurnSignal(void);

cmd_t get_cmdWindShieldWiper(void);

cmd_t get_cmdWindShieldWasher(void);

frame_num_t get_frameNumber(void);

mileage_t get_mileage(void);

speed_t get_speed(void);

chassis_pb_t get_problemChassis(void);

motor_pb_t get_problemMotor(void);

tank_level_t  get_tankLevel(void);

rpm_t  get_rpm(void);

battery_pb_t get_problemBattery(void);

crc_t get_crc8(void);

id_msg_t get_idMsgBcgvToBgf1(void);

id_msg_t get_idMsgBcgvToBgf2(void);

id_msg_t get_idMsgBcgvToBgf3(void);

id_msg_t get_idMsgBcgvToBgf4(void);

id_msg_t get_idMsgBcgvToBgf5(void);

activation_t get_activationPositionLights(void);

activation_t get_activationLowBeams(void);

activation_t get_activationHighBeams(void);

activation_t get_activationRightTurnSignal(void);

activation_t get_activationLeftTurnSignal(void);

indicator_t get_indicatorPositionLights(void);

indicator_t get_indicatorLowBeams(void);

indicator_t get_indicatorHighBeams(void);

indicator_t get_indicatorFuel(void);

indicator_t get_indicatorMotorPb(void);

indicator_t get_indicatorTirePressure(void);

indicator_t get_indicatorDischargedBattery(void);

indicator_t get_indicatorWarning(void);

indicator_t get_indicatorBatteryFailure(void);

indicator_t get_indicatorTempLDR(void);

indicator_t get_indicatorMotorPressure(void);

indicator_t get_indicatorOilOverheating(void);

indicator_t get_indicatorBrakeFailure(void);

activation_t get_activationShieldWiper(void);

activation_t get_activationShieldWasher(void);

display_mileage_t get_displayMileage(void);

display_speed_t get_displaySpeed(void);

display_tank_level_t get_displayTankLevel(void);

display_rpm_t get_displayRpm(void);

short int set_cmdWarning(cmd_t value);

short int set_cmdPositionLights(cmd_t value);

short int set_cmdLowBeams(cmd_t value);

short int set_cmdHighBeams(cmd_t value);

short int set_cmdRightTurnSignal(cmd_t value);

short int set_cmdLeftTurnSignal(cmd_t value);

short int set_cmdWindShieldWiper(cmd_t value);

short int set_cmdWindShieldWasher(cmd_t value);

short int set_frameNumber(frame_num_t value);

short int set_mileage(mileage_t value);

short int set_speed(speed_t value);

short int set_problemChassis(chassis_pb_t value);

short int set_problemMotor(motor_pb_t value);

short int set_tankLevel(tank_level_t  value);

short int set_rpm(rpm_t  value);

short int set_problemBattery(battery_pb_t value);

short int set_crc8(crc_t value);

short int set_idMsgBcgvToBgf1(id_msg_t value);

short int set_idMsgBcgvToBgf2(id_msg_t value);

short int set_idMsgBcgvToBgf3(id_msg_t value);

short int set_idMsgBcgvToBgf4(id_msg_t value);

short int set_idMsgBcgvToBgf5(id_msg_t value);

short int set_activationPositionLights(activation_t value);

short int set_activationLowBeams(activation_t value);

short int set_activationHighBeams(activation_t value);

short int set_activationRightTurnSignal(activation_t value);

short int set_activationLeftTurnSignal(activation_t value);

short int set_indicatorPositionLights(indicator_t value);

short int set_indicatorLowBeams(indicator_t value);

short int set_indicatorHighBeams(indicator_t value);

short int set_indicatorFuel(indicator_t value);

short int set_indicatorMotorPb(indicator_t value);

short int set_indicatorTirePressure(indicator_t value);

short int set_indicatorDischargedBattery(indicator_t value);

short int set_indicatorWarning(indicator_t value);

short int set_indicatorBatteryFailure(indicator_t value);

short int set_indicatorTempLDR(indicator_t value);

short int set_indicatorMotorPressure(indicator_t value);

short int set_indicatorOilOverheating(indicator_t value);

short int set_indicatorBrakeFailure(indicator_t value);

short int set_activationShieldWiper(activation_t value);

short int set_activationShieldWasher(activation_t value);

short int set_displayMileage(display_mileage_t value);

short int set_displaySpeed(display_speed_t value);

short int set_displayTankLevel(display_tank_level_t value);

short int set_displayRpm(display_rpm_t value);

void init_BCGV_Data(void);

#endif // BCGV_LIB_H