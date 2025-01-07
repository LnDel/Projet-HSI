#include <stdint.h>
#include <stdio.h>
#include "drv_api.h"
#include "bcgv_lib.h"

#include "decode.h"
#include "encode.h"

#include "fsm_high_beams.h"
#include "fsm_low_beams.h"
#include "fsm_position_light.h"
#include "fsm_left_turn_signal.h"
#include "fsm_right_turnsignal.h"
#include "fsm_windshield.h"
#include "fsm_warning.h"


/**
 * \file    app.c
 * \brief   Application principale pour la lecture des trames UDP.
 * \details Cette application interagit avec le driver pour recevoir et afficher les trames.
 * \author  Votre Nom
 */

int main(void) {
    uint16_t frameIndex;
    int32_t drvFd;
    int32_t readStatusUDP;
    int32_t readStatusSerial;
    uint8_t udpFrame[DRV_UDP_100MS_FRAME_SIZE];
    serial_frame_t serialFrame[DRV_MAX_FRAMES];
    is_valid_frame_t isUdpValid;
    uint32_t serialDataLen = 0;
    // Include les .h
    high_beams_state_t stateHighBeams = ST_INIT_high_beams;
    low_beams_state_t stateLowBeams = ST_INIT_low_beams;
    position_light_state_t statePositionLight = ST_INIT_position_light;
    //windshield_state_t stateWindshield = ST_INIT_WINDSHIELD;
    left_turn_signal_state_t stateLeftTurnSignal = ST_INIT_LEFT_TURNSIGNAL;
    right_turn_signal_state_t stateRightTurnSignal = ST_INIT_RIGHT_TURNSIGNAL;
    warning_state_t stateWarning = ST_INIT_WARNING;

    
    // Initialization of all fields
    init_BCGV_Data();

    // Open the driver
    drvFd = drv_open();
    if (drvFd < 0) {
        fprintf(stderr, "Error: Unable to open the driver.\n");
        return -1;
    }

    printf("Driver opened successfully. fd = %d\n", drvFd);

    while (1) {
        // Read UDP frame
        readStatusUDP = drv_read_udp_100ms(drvFd, udpFrame);

        if (readStatusUDP < 0) {
            fprintf(stderr, "Error: Unable to read the UDP frame.\n");
            break;
        }

        // Check frame's number
        printf("Received UDP frame:\n");
        for (frameIndex = 0; frameIndex < DRV_UDP_100MS_FRAME_SIZE; frameIndex++) {
            printf("%02X ", udpFrame[frameIndex]);
        }

        // Decode UDP or log error
        isUdpValid = decode_mux_to_bcgv(udpFrame);
        if (isUdpValid == INVALID){
            printf("Invalid frame receivedd");
        }

        // Read serial frame
        readStatusSerial = drv_read_ser(drvFd, serialFrame, &serialDataLen);
        if (readStatusSerial == DRV_ERROR) {
            fprintf(stderr, "Error: Unable to read serial frames.\n");
            break;
        }

        // Decode serial line
        //decode_comodo_to_bcgv(serialFrame);

        // Update state machine
        stateHighBeams = main_fsm_high_beams(stateHighBeams);
        stateLowBeams = main_fsm_low_beams(stateLowBeams);
        statePositionLight = main_fsm_position_light(statePositionLight);
        stateLeftTurnSignal = main_fsm_left_turnsignal(stateLeftTurnSignal);
        stateRightTurnSignal = main_fsm_right_turnsignal(stateRightTurnSignal);
        stateWarning = main_fsm_warning(stateWarning);
        //stateWindshield = main_fsm_windshield(stateWindshield);
        // Encode and write UDP

        // Encode and write serial line

        printf("\n");
    }

    return 0;
}
