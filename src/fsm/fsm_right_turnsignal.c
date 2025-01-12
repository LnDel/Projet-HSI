/**
 * \file        fsm_right_turn_signal.c
 * \author      Warren Anderson
 * \version     0.4
 * \date        08 October 2023
 * \brief       Finite State Machine for the right turn signal.
 */

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <time.h>
#include "../bcgv_lib.h"
#include "fsm_right_turnsignal.h"
#define TRANS_COUNT_RIGHT_TURN_SIGNAL (sizeof(trans) / sizeof(*trans))
static unsigned long lastTimerSeconds = 0; 

/* Callback functions called on transitions */
static int callback1(void) { // ST_OFF_RIGHT_TURNSIGNAL
    set_activationRightTurnSignal(0);
    set_indicatorRightTurnSignal(0);
    return 0;
}

static int callback2(void) { // ST_ACTIVATED_AND_ON_RIGHT_TURNSIGNAL
    printf("Droit activé et allumé \n");
    set_activationRightTurnSignal(1);
    set_indicatorRightTurnSignal(1);
    return 0;
}

static int callback3(void) { // ST_ACQUITTED_RIGHT_TURNSIGNAL
    printf("Droit acquitté\n");
    set_indicatorRightTurnSignal(1);
    return 0;
}

static int callback4(void) { // ST_ACTIVATED_AND_OFF_RIGHT_TURNSIGNAL
    printf("Droit activé et éteint \n");
    set_activationRightTurnSignal(0);
    set_indicatorRightTurnSignal(0);
    return 0;
}

static int FsmError(void) { // ST_ERROR_RIGHT_TURNSIGNAL
    printf("ERROR\n");
    return -1;
}

/* Transition structure */
static right_turnsignal_transition_t trans[] = {
    { ST_INIT_RIGHT_TURNSIGNAL, EV_ANY_RIGHT_TURNSIGNAL, &callback1, ST_OFF_RIGHT_TURNSIGNAL },
    { ST_OFF_RIGHT_TURNSIGNAL, EV_CMD1_RIGHT_TURNSIGNAL, &callback2, ST_ACTIVATED_AND_ON_RIGHT_TURNSIGNAL },
    { ST_OFF_RIGHT_TURNSIGNAL, EV_CMD0_RIGHT_TURNSIGNAL, &callback1, ST_OFF_RIGHT_TURNSIGNAL },
    { ST_ACTIVATED_AND_ON_RIGHT_TURNSIGNAL, EV_ACQ_RECEIVED_RIGHT_TURNSIGNAL, &callback3, ST_ACQUITTED_RIGHT_TURNSIGNAL },
    { ST_ACTIVATED_AND_ON_RIGHT_TURNSIGNAL, EV_ACQ_TIMEOUT_RIGHT_TURNSIGNAL, &FsmError, ST_ERROR_RIGHT_TURNSIGNAL },
    { ST_ACTIVATED_AND_ON_RIGHT_TURNSIGNAL, EV_CMD1_RIGHT_TURNSIGNAL, &callback2, ST_ACTIVATED_AND_ON_RIGHT_TURNSIGNAL },
    { ST_ACTIVATED_AND_ON_RIGHT_TURNSIGNAL, EV_CMD0_RIGHT_TURNSIGNAL, &callback1, ST_OFF_RIGHT_TURNSIGNAL },
    { ST_ACQUITTED_RIGHT_TURNSIGNAL, EV_CMD0_RIGHT_TURNSIGNAL, &callback1, ST_OFF_RIGHT_TURNSIGNAL },
    { ST_ACQUITTED_RIGHT_TURNSIGNAL, EV_CMD1_RIGHT_TURNSIGNAL, &callback3, ST_ACQUITTED_RIGHT_TURNSIGNAL },
    { ST_ACQUITTED_RIGHT_TURNSIGNAL_OFF, EV_CMD1_RIGHT_TURNSIGNAL, &callback3, ST_ACQUITTED_RIGHT_TURNSIGNAL_OFF },
    { ST_ACQUITTED_RIGHT_TURNSIGNAL_OFF, EV_TIME1_RIGHT_TURNSIGNAL, &callback2, ST_ACTIVATED_AND_ON_RIGHT_TURNSIGNAL },
    { ST_ACTIVATED_AND_OFF_RIGHT_TURNSIGNAL, EV_ACQ_TIMEOUT_RIGHT_TURNSIGNAL, &FsmError, ST_ERROR_RIGHT_TURNSIGNAL },
    { ST_ACTIVATED_AND_OFF_RIGHT_TURNSIGNAL, EV_ACQ_RECEIVED_RIGHT_TURNSIGNAL, &callback3, ST_ACQUITTED_RIGHT_TURNSIGNAL_OFF },
    { ST_ACTIVATED_AND_OFF_RIGHT_TURNSIGNAL, EV_CMD1_RIGHT_TURNSIGNAL, &callback4, ST_ACTIVATED_AND_OFF_RIGHT_TURNSIGNAL },
    { ST_ACTIVATED_AND_OFF_RIGHT_TURNSIGNAL, EV_CMD0_RIGHT_TURNSIGNAL, &callback1, ST_OFF_RIGHT_TURNSIGNAL },
    { ST_ACQUITTED_RIGHT_TURNSIGNAL, EV_TIME1_RIGHT_TURNSIGNAL, &callback4, ST_ACTIVATED_AND_OFF_RIGHT_TURNSIGNAL },
    { ST_ACQUITTED_RIGHT_TURNSIGNAL_OFF, EV_CMD0_RIGHT_TURNSIGNAL, &callback1, ST_OFF_RIGHT_TURNSIGNAL },
    { ST_ANY_RIGHT_TURNSIGNAL, EV_ERR_RIGHT_TURNSIGNAL, &FsmError, ST_TERM_RIGHT_TURNSIGNAL }
};


right_turn_signal_event_t get_next_event_right_turnsignal(right_turn_signal_state_t currentState, unsigned long currentTimeSeconds) {
    right_turn_signal_event_t event = EV_NONE_RIGHT_TURNSIGNAL;
    cmd_t cmd = get_cmdRightTurnSignal(); // Get the cmd parameter
    indicator_t acq = get_indicatorRightTurnSignal(); // Get the acq parameter

    time_t timer = time(NULL);
    unsigned long timerSeconds = difftime(timer, 0);

    if (lastTimerSeconds == 0) {
        lastTimerSeconds = timerSeconds; // Initialise au premier appel
    }

    printf("\n lastTimerSeconds : %ld\n timerSeconds : %ld\n timer: %ld\n",lastTimerSeconds, timerSeconds, timerSeconds - lastTimerSeconds);

    if (cmd == 0){
        event = EV_CMD0_RIGHT_TURNSIGNAL;
    } else if (currentState == ST_ACQUITTED_RIGHT_TURNSIGNAL || currentState == ST_ACQUITTED_RIGHT_TURNSIGNAL_OFF){

        if ((timerSeconds - lastTimerSeconds) >= 1){
            event = EV_TIME1_RIGHT_TURNSIGNAL;
            lastTimerSeconds = timerSeconds;
        } else {
            event = EV_CMD1_RIGHT_TURNSIGNAL;
        }
    } else if (currentState == ST_ACTIVATED_AND_ON_RIGHT_TURNSIGNAL || currentState == ST_ACTIVATED_AND_OFF_RIGHT_TURNSIGNAL){
        if (acq == 1){
            event = EV_ACQ_RECEIVED_RIGHT_TURNSIGNAL;
        } else if ((timerSeconds - lastTimerSeconds) > 1){
            event = EV_ACQ_TIMEOUT_RIGHT_TURNSIGNAL;
        } else {
            event = EV_CMD1_RIGHT_TURNSIGNAL;
        }
    } else if (cmd == 1){
        event = EV_CMD1_RIGHT_TURNSIGNAL;
    } else {
        event = EV_ERR_RIGHT_TURNSIGNAL;
    }
    return event;
}
right_turn_signal_state_t main_fsm_right_turnsignal(right_turn_signal_state_t currentState) {
    right_turn_signal_state_t state = currentState;
    right_turn_signal_event_t event = get_next_event_right_turnsignal(state, difftime(time(NULL),0));
    printf("DroitÉtat actuel : %d,\n Événement : %d\n", currentState, event);
    // for the initialisation
    if (state == ST_INIT_RIGHT_TURNSIGNAL){
        return ST_OFF_RIGHT_TURNSIGNAL;
    }

    /* Process transitions */
    for (long unsigned int i = 0; i < TRANS_COUNT_RIGHT_TURN_SIGNAL; i++) {
        if ((state == trans[i].state) || (ST_ANY_RIGHT_TURNSIGNAL == trans[i].state)) {
            if ((event == trans[i].event) || (EV_ANY_RIGHT_TURNSIGNAL == trans[i].event)) {
                state = trans[i].next_state;
                if (trans[i].callback != NULL) {
                    trans[i].callback();
                }
                break;
            }
        }
    }
    return state; // return the new state
}