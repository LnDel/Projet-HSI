#include <stdint.h>
#include <stdio.h>
#include "drv_api.h"
#include "bcgv_lib.h"

#include "fsm_high_beams.h"
#include "fsm_low_beams.h"
#include "fsm_position_light.h"


/**
 * \file    app.c
 * \brief   Application principale pour la lecture des trames UDP.
 * \details Cette application interagit avec le driver pour recevoir et afficher les trames.
 * \author  Votre Nom
 */

int main(void) {
    int32_t drvFd;
    uint8_t udpFrame[DRV_UDP_100MS_FRAME_SIZE];
    // Include les .h
    high_beams_state_t stateHighBeams = 0;
    low_beams_state_t stateLowBeams = 0;
    position_light_state_t statePositionLight = 0;
    // windshield_state_t stateWindshield = 0;
    // turn_signal_state_t stateTurnSignal = 0;

    

    init_BCGV_Data(); // Initialise tous les champs à 0.

    // Ouverture du driver
    drvFd = drv_open();
    if (drvFd < 0) {
        fprintf(stderr, "Error: Unable to open the driver.\n");
        return -1;
    }

    printf("Driver opened successfully. fd = %d\n", drvFd);

    while (1) {
        int32_t readStatus = drv_read_udp_100ms(drvFd, udpFrame);

        if (readStatus < 0) {
            fprintf(stderr, "Error: Unable to read the UDP frame.\n");
            break;
        }

        // Check frame's number
        printf("Received UDP frame:\n");
        for (int i = 0; i < DRV_UDP_100MS_FRAME_SIZE; i++) {
            printf("%02X ", udpFrame[i]);
        }
        // Decode UDP or log error

        // Decode serial line

        // Update state machine
        stateHighBeams = main_fsm_high_beams(stateHighBeams);
        stateLowBeams = main_fsm_low_beams(stateLowBeams);
        statePositionLight = main_fsm_position_light(statePositionLight);
        // Encode and write UDP

        // Encode and write serial line

        printf("\n");
    }

    return 0;
}
