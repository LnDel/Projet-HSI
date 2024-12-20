#include <stdint.h>
#include <stdio.h>
#include "drv_api.h"

/**
 * \file    app.c
 * \brief   Application principale pour la lecture des trames UDP.
 * \details Cette application interagit avec le driver pour recevoir et afficher les trames.
 * \author  Votre Nom
 */

int main(void) {
    int32_t drvFd;
    uint8_t udpFrame[DRV_UDP_100MS_FRAME_SIZE];

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

        printf("Received UDP frame:\n");
        for (int i = 0; i < DRV_UDP_100MS_FRAME_SIZE; i++) {
            printf("%02X ", udpFrame[i]);
        }
        printf("\n");
    }

    return 0;
}
