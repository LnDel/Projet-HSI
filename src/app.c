/**
 * \file app.c
 * \brief Main BCGV application.
 * \details This application interacts with the driver to receive frames, update status, and send return frames.
 * \author Warren Anderson, Samy Belbouab, Hélène Delran--Garric
 */

#include <stdint.h>
#include <stdio.h>

#include "drv_api.h"
#include "bcgv_lib.h"

#include "decode.h"
#include "encode.h"

#include "fsm/fsm_high_beams.h"
#include "fsm/fsm_low_beams.h"
#include "fsm/fsm_position_light.h"
#include "fsm/fsm_left_turn_signal.h"
#include "fsm/fsm_right_turnsignal.h"
#include "fsm/fsm_windshield.h"
#include "fsm/fsm_warning.h"

#define NUMBER_SERIAL_FRAMES_SEND 5
#define SER_PORT_TRANSMITTER 11
#define SER_PORT_RECEIVED 12

/**
 * \brief Main entry point of the application.
 * \details Initializes the application, interacts with the driver to process frames, and updates state machines.
 * \return int : Returns 0 on success, or -1 on failure.
 */
int main(void) {

    uint16_t frameIndex;
    uint16_t byteIndex;
    uint32_t serialDataLen = 0;

    int32_t drvFd;
    int32_t readStatusUDP;
    int32_t readStatusSerial;

    uint8_t udpFrame[DRV_UDP_100MS_FRAME_SIZE];
    uint8_t udpWriteFrame[DRV_UDP_200MS_FRAME_SIZE];
    serial_frame_t serialFrames[DRV_MAX_FRAMES];
    is_valid_frame_t isUdpValid;

    uint8_t serialFrameBGF1[SER_MAX_FRAME_SIZE];
    uint8_t serialFrameBGF2[SER_MAX_FRAME_SIZE];
    uint8_t serialFrameBGF3[SER_MAX_FRAME_SIZE];
    uint8_t serialFrameBGF4[SER_MAX_FRAME_SIZE];
    uint8_t serialFrameBGF5[SER_MAX_FRAME_SIZE];
    uint8_t* framesSend[NUMBER_SERIAL_FRAMES_SEND];
    serial_frame_t serialFramesBGF[DRV_MAX_FRAMES];

    // Initialize state machines
    high_beams_state_t stateHighBeams = ST_INIT_high_beams;
    low_beams_state_t stateLowBeams = ST_INIT_low_beams;
    position_light_state_t statePositionLight = ST_INIT_position_light;
    windshield_state_t stateWindshield = ST_INIT_WINDSHIELD;
    left_turn_signal_state_t stateLeftTurnSignal = ST_INIT_LEFT_TURNSIGNAL;
    right_turn_signal_state_t stateRightTurnSignal = ST_INIT_RIGHT_TURNSIGNAL;
    warning_state_t stateWarning = ST_INIT_WARNING;

    // Initialize BCGV data
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

        // Display received UDP frame
        printf("Received UDP frame:\n");
        for (frameIndex = 0; frameIndex < DRV_UDP_100MS_FRAME_SIZE; frameIndex++) {
            printf("%02X ", udpFrame[frameIndex]);
        }

        // Decode UDP frame or log error
        isUdpValid = decode_mux_to_bcgv(udpFrame);
        if (isUdpValid == INVALID) {
            printf("Invalid frame received\n");
        }

        // Read serial frame
        readStatusSerial = drv_read_ser(drvFd, serialFrames, &serialDataLen);
        if (readStatusSerial == DRV_ERROR) {
            fprintf(stderr, "Error: Unable to read serial frames.\n");
            break;
        }

        // Process received serial frames
        if (serialFrames[0].serNum == SER_PORT_RECEIVED) {
            printf("\nReceived serial frames:\n");
            for (frameIndex = 0; frameIndex < serialDataLen; frameIndex++) {
                printf("Frame %u (serNum: %u, frameSize: %zu): ", frameIndex, serialFrames[frameIndex].serNum, serialFrames[frameIndex].frameSize);

                for (byteIndex = 0; byteIndex < serialFrames[frameIndex].frameSize; byteIndex++) {
                    printf("%02X ", serialFrames[frameIndex].frame[byteIndex]);
                }
                printf("\n");
                if (serialFrames[frameIndex].serNum == SER_PORT_RECEIVED) {
                    decode_comodo_to_bcgv(serialFrames[frameIndex].frame);
                }
            }
        }

        // Update state machines
        stateHighBeams = main_fsm_high_beams(stateHighBeams);
        stateLowBeams = main_fsm_low_beams(stateLowBeams);
        statePositionLight = main_fsm_position_light(statePositionLight);
        stateLeftTurnSignal = main_fsm_left_turnsignal(stateLeftTurnSignal);
        stateRightTurnSignal = main_fsm_right_turnsignal(stateRightTurnSignal);
        stateWarning = main_fsm_warning(stateWarning);
        stateWindshield = main_fsm_windshield(stateWindshield);

        // Encode and write UDP frame
        encode_bcgv_to_mux(udpWriteFrame);
        isUdpValid = drv_write_udp_200ms(drvFd, udpWriteFrame);
        if (isUdpValid != DRV_SUCCESS) {
            fprintf(stderr, "Error: Failed to write UDP frame to driver\n");
            break;
        }

        // Encode and write serial frames
        encode_bcgv_to_bgf1(serialFrameBGF1);
        encode_bcgv_to_bgf2(serialFrameBGF2);
        encode_bcgv_to_bgf3(serialFrameBGF3);
        encode_bcgv_to_bgf4(serialFrameBGF4);
        encode_bcgv_to_bgf5(serialFrameBGF5);

        framesSend[0] = serialFrameBGF1;
        framesSend[1] = serialFrameBGF2;
        framesSend[2] = serialFrameBGF3;
        framesSend[3] = serialFrameBGF4;
        framesSend[4] = serialFrameBGF5;

        for (frameIndex = 0; frameIndex < NUMBER_SERIAL_FRAMES_SEND; frameIndex++) {
            serialFramesBGF[frameIndex].serNum = SER_PORT_TRANSMITTER;
            serialFramesBGF[frameIndex].frameSize = SER_MAX_FRAME_SIZE;
            for (byteIndex = 0; byteIndex < SER_MAX_FRAME_SIZE; byteIndex++) {
                serialFramesBGF[frameIndex].frame[byteIndex] = framesSend[frameIndex][byteIndex];
            }
        }

        drv_write_ser(drvFd, serialFramesBGF, NUMBER_SERIAL_FRAMES_SEND);
        drv_read_ser(drvFd, serialFramesBGF, &serialDataLen);

        printf("\n");
    }

    return 0;
}
