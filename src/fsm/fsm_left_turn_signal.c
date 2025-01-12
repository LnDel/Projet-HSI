/**
 * \file        fsm_left_turn_signal.c
 * \author      Alexis Daley
 * \version     0.4
 * \date        08 October 2023
 * \brief       Finite State Machine for the left turn signal.
 */

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <time.h>
#include "../bcgv_lib.h"
#include "fsm_left_turn_signal.h"
#define TRANS_COUNT_LEFT_TURNSIGNAL (sizeof(trans) / sizeof(*trans))
static unsigned long lastTimerSeconds = 0; 

/* Callback functions called on transitions */
static int callback1(void) { // ST_OFF_LEFT_TURNSIGNAL
    set_activationLeftTurnSignal(0);
    return 0;
}

static int callback2(void) { // ST_ACTIVATED_AND_ON_LEFT_TURNSIGNAL
    printf("Gauche activé et allumé \n");
    set_activationLeftTurnSignal(1);
    set_indicatorLeftTurnSignal(1);
    return 0;
}

static int callback3(void) { // ST_ACQUITTED_LEFT_TURNSIGNAL
    printf("Gauche acquitté\n");
    set_indicatorLeftTurnSignal(1);
    return 0;
}

static int callback4(void) { // ST_ACTIVATED_AND_OFF_LEFT_TURNSIGNAL
    printf("Gauche activé et éteint \n");
    set_activationLeftTurnSignal(0);
    set_indicatorLeftTurnSignal(0);
    return 0;
}

static int FsmError(void) { // ST_ERROR_LEFT_TURNSIGNAL
    printf("ERROR\n");
    return -1;
}

/* Transition structure */
static left_turnsignal_transition_t trans[] = {
    { ST_INIT_LEFT_TURNSIGNAL, EV_ANY_LEFT_TURNSIGNAL, &callback1, ST_OFF_LEFT_TURNSIGNAL },
    { ST_OFF_LEFT_TURNSIGNAL, EV_CMD1_LEFT_TURNSIGNAL, &callback2, ST_ACTIVATED_AND_ON_LEFT_TURNSIGNAL },
    { ST_OFF_LEFT_TURNSIGNAL, EV_CMD0_LEFT_TURNSIGNAL, &callback1, ST_OFF_LEFT_TURNSIGNAL },
    { ST_ACTIVATED_AND_ON_LEFT_TURNSIGNAL, EV_ACQ_RECEIVED_LEFT_TURNSIGNAL, &callback3, ST_ACQUITTED_LEFT_TURNSIGNAL },
    { ST_ACTIVATED_AND_ON_LEFT_TURNSIGNAL, EV_ACQ_TIMEOUT_LEFT_TURNSIGNAL, &FsmError, ST_ERROR_LEFT_TURNSIGNAL },
    { ST_ACTIVATED_AND_ON_LEFT_TURNSIGNAL, EV_CMD1_LEFT_TURNSIGNAL, &callback2, ST_ACTIVATED_AND_ON_LEFT_TURNSIGNAL },
    { ST_ACTIVATED_AND_ON_LEFT_TURNSIGNAL, EV_CMD0_LEFT_TURNSIGNAL, &callback1, ST_OFF_LEFT_TURNSIGNAL },
    { ST_ACQUITTED_LEFT_TURNSIGNAL, EV_CMD0_LEFT_TURNSIGNAL, &callback1, ST_OFF_LEFT_TURNSIGNAL },
    { ST_ACQUITTED_LEFT_TURNSIGNAL, EV_CMD1_LEFT_TURNSIGNAL, &callback3, ST_ACQUITTED_LEFT_TURNSIGNAL },
    { ST_ACQUITTED_LEFT_TURNSIGNAL_OFF, EV_CMD1_LEFT_TURNSIGNAL, &callback3, ST_ACQUITTED_LEFT_TURNSIGNAL_OFF },
    { ST_ACQUITTED_LEFT_TURNSIGNAL_OFF, EV_TIME1_LEFT_TURNSIGNAL, &callback2, ST_ACTIVATED_AND_ON_LEFT_TURNSIGNAL },
    { ST_ACTIVATED_AND_OFF_LEFT_TURNSIGNAL, EV_ACQ_TIMEOUT_LEFT_TURNSIGNAL, &FsmError, ST_ERROR_LEFT_TURNSIGNAL },
    { ST_ACTIVATED_AND_OFF_LEFT_TURNSIGNAL, EV_ACQ_RECEIVED_LEFT_TURNSIGNAL, &callback3, ST_ACQUITTED_LEFT_TURNSIGNAL_OFF },
    { ST_ACTIVATED_AND_OFF_LEFT_TURNSIGNAL, EV_CMD1_LEFT_TURNSIGNAL, &callback4, ST_ACTIVATED_AND_OFF_LEFT_TURNSIGNAL },
    { ST_ACTIVATED_AND_OFF_LEFT_TURNSIGNAL, EV_CMD0_LEFT_TURNSIGNAL, &callback1, ST_OFF_LEFT_TURNSIGNAL },
    { ST_ACQUITTED_LEFT_TURNSIGNAL, EV_TIME1_LEFT_TURNSIGNAL, &callback4, ST_ACTIVATED_AND_OFF_LEFT_TURNSIGNAL },
    { ST_ACQUITTED_LEFT_TURNSIGNAL_OFF, EV_CMD0_LEFT_TURNSIGNAL, &callback1, ST_OFF_LEFT_TURNSIGNAL },
    { ST_ANY_LEFT_TURNSIGNAL, EV_ERR_LEFT_TURNSIGNAL, &FsmError, ST_TERM_LEFT_TURNSIGNAL }
};

left_turn_signal_event_t get_next_event_left_turnsignal(left_turn_signal_state_t currentState, unsigned long currentTimeSeconds) {
    left_turn_signal_event_t event = EV_NONE_LEFT_TURNSIGNAL;
    cmd_t cmd = get_cmdLeftTurnSignal(); // Get the cmd parameter
    indicator_t acq = get_indicatorLeftTurnSignal(); // Get the acq parameter

    time_t timer = time(NULL);
    unsigned long timerSeconds = difftime(timer, 0);

    if (lastTimerSeconds == 0) {
        lastTimerSeconds = timerSeconds; // Initialise au premier appel
    }

    printf("\n lastTimerSeconds : %ld\n timerSeconds : %ld\n timer: %ld\n",lastTimerSeconds, timerSeconds, timerSeconds - lastTimerSeconds);

    if (cmd == 0){
        event = EV_CMD0_LEFT_TURNSIGNAL;
    } else if (currentState == ST_ACQUITTED_LEFT_TURNSIGNAL || currentState == ST_ACQUITTED_LEFT_TURNSIGNAL_OFF){
        if ((timerSeconds - lastTimerSeconds) >= 1){
            event = EV_TIME1_LEFT_TURNSIGNAL;
            lastTimerSeconds = timerSeconds;
        } else {
            event = EV_CMD1_LEFT_TURNSIGNAL;
        }
    } else if (currentState == ST_ACTIVATED_AND_ON_LEFT_TURNSIGNAL || currentState == ST_ACTIVATED_AND_OFF_LEFT_TURNSIGNAL){
        if (acq == 1){
            event = EV_ACQ_RECEIVED_LEFT_TURNSIGNAL;
        } else if ((timerSeconds - lastTimerSeconds) > 1){
            event = EV_ACQ_TIMEOUT_LEFT_TURNSIGNAL;
        } else {
            event = EV_CMD1_LEFT_TURNSIGNAL;
        }
    } else if (cmd == 1){
        event = EV_CMD1_LEFT_TURNSIGNAL;
    } else {
        event = EV_ERR_LEFT_TURNSIGNAL;
    }
    return event;
}

left_turn_signal_state_t main_fsm_left_turnsignal(left_turn_signal_state_t currentState) {
    left_turn_signal_state_t state = currentState;
    left_turn_signal_event_t event = get_next_event_left_turnsignal(state, difftime(time(NULL),0));
    printf("\n\nGaucheÉtat actuel : %d,\nÉvénement : %d\n", currentState, event);


    // for the initialisation
    if (state == ST_INIT_LEFT_TURNSIGNAL){
        return ST_OFF_LEFT_TURNSIGNAL;
    }

    /* Process transitions */
    for (long unsigned int i = 0; i < TRANS_COUNT_LEFT_TURNSIGNAL; i++) {
        if ((state == trans[i].state) || (ST_ANY_LEFT_TURNSIGNAL == trans[i].state)) {
            if ((event == trans[i].event) || (EV_ANY_LEFT_TURNSIGNAL == trans[i].event)) {
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
