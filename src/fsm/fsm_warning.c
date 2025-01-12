/**
 * \file        fsm_warning.c
 * \author      Alexis Daley
 * \version     0.4
 * \date        08 January 2025
 * \brief       Finite State Machine for the warning signal.
 */

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <time.h>
#include "../bcgv_lib.h"
#include "fsm_warning.h"

#define TRANS_COUNT_WARNING (sizeof(trans) / sizeof(*trans))

/* Callback functions called on transitions */
static int callback1(void) { // ST_OFF_WARNING
    printf("Warning signal off\n");
    set_activationRightTurnSignal(0);
    set_activationLeftTurnSignal(0);
    set_indicatorWarning(0);
    return 0;
}

static int callback2(void) { // ST_ACTIVATED_AND_ON_WARNING
    printf("Warning signal on\n");
    set_activationRightTurnSignal(1);
    set_activationLeftTurnSignal(1);
    set_indicatorWarning(1);
    return 0;
}

static int callback3(void) { // ST_ACQUITTED_WARNING
    printf("Warning signal acquited\n");
    return 0;
}

static int callback4(void) { // ST_ACTIVATED_AND_OFF_WARNING
    set_activationRightTurnSignal(0);
    set_activationLeftTurnSignal(0);
    set_indicatorWarning(0);
    return 0;
}


static int FsmError(void) { // ST_ERROR_WARNING
    printf("ERROR\n");
    return -1;
}

/* Transition structure */
static warning_transition_t trans[] = {
    { ST_INIT_WARNING, EV_ANY_WARNING, &callback1, ST_OFF_WARNING },
    { ST_OFF_WARNING, EV_CMD0_WARNING, &callback1, ST_OFF_WARNING },
    { ST_OFF_WARNING, EV_CMD1_WARNING, &callback2, ST_ACTIVATED_AND_ON_WARNING },
    { ST_ACTIVATED_AND_ON_WARNING, EV_CMD0_WARNING, &callback1, ST_OFF_WARNING },
    { ST_ACTIVATED_AND_ON_WARNING, EV_CMD1_WARNING, &callback2, ST_ACTIVATED_AND_ON_WARNING },
    { ST_ACTIVATED_AND_ON_WARNING, EV_ACQ_RECEIVED_WARNING, &callback3, ST_ACQUITTED_WARNING },
    { ST_ACTIVATED_AND_ON_WARNING, EV_ACQ_TIMEOUT_WARNING, &FsmError, ST_ERROR_WARNING },
    { ST_ACQUITTED_WARNING, EV_CMD0_WARNING, &callback1, ST_OFF_WARNING },
    { ST_ACQUITTED_WARNING, EV_CMD1_WARNING, &callback3, ST_ACQUITTED_WARNING },
    { ST_ACQUITTED_WARNING, EV_TIME1_WARNING, &callback4, ST_ACTIVATED_AND_OFF_WARNING },
    { ST_ACTIVATED_AND_OFF_WARNING, EV_ACQ_TIMEOUT_WARNING, &FsmError, ST_ERROR_WARNING },
    { ST_ACTIVATED_AND_OFF_WARNING, EV_ACQ_RECEIVED_WARNING, &callback3, ST_ACQUITTED_WARNING_OFF },
    { ST_ACTIVATED_AND_OFF_WARNING, EV_CMD1_WARNING, &callback4, ST_ACTIVATED_AND_OFF_WARNING },
    { ST_ACTIVATED_AND_OFF_WARNING, EV_CMD0_WARNING, &callback1, ST_OFF_WARNING },
    { ST_ACQUITTED_WARNING_OFF, EV_TIME1_WARNING, &callback2, ST_ACTIVATED_AND_ON_WARNING },
    { ST_ACQUITTED_WARNING_OFF, EV_CMD0_WARNING, &callback1, ST_OFF_WARNING },
    { ST_ACQUITTED_WARNING_OFF, EV_CMD1_WARNING, &callback3, ST_ACQUITTED_WARNING_OFF },
    { ST_ANY_WARNING, EV_ERR_WARNING, &FsmError, ST_TERM_WARNING }
};

warning_event_t get_next_event_warning(warning_state_t currentState, unsigned long currentTimeSeconds) {
    warning_event_t event = EV_NONE_WARNING;
    cmd_t cmd = get_cmdWarning(); // Get the cmd parameter
    indicator_t acq = get_indicatorWarning(); // Get the acq parameter

    time_t timer = time(NULL);
    unsigned long timerSeconds = difftime(timer, 0);

    if (cmd == 0){
        event = EV_CMD0_WARNING;
    } else if (currentState == ST_ACQUITTED_WARNING || currentState == ST_ACQUITTED_WARNING_OFF){
        if ((timerSeconds - currentTimeSeconds) == 1){
            event = EV_TIME1_WARNING;
        } else {
            event = EV_CMD1_WARNING;
        }
    } else if (currentState == ST_ACTIVATED_AND_ON_WARNING || currentState == ST_ACTIVATED_AND_OFF_WARNING){
        if (acq == 1){
            event = EV_ACQ_RECEIVED_WARNING;
        } else if ((timerSeconds - currentTimeSeconds) > 1){
            event = EV_ACQ_TIMEOUT_WARNING;
        } else {
            event = EV_CMD1_WARNING;
        }
    } else if (cmd == 1){
        event = EV_CMD1_WARNING;
    } else {
        event = EV_ERR_WARNING;
    }
    return event;
}

warning_state_t main_fsm_warning(warning_state_t currentState) {
    warning_state_t state = currentState;
    warning_event_t event = get_next_event_warning(state, difftime(time(NULL), 0));

  // for the initialisation
  if (state == ST_INIT_WARNING){
    printf("Warning signal initialisation\n");
    return ST_OFF_WARNING;
  }

    /* Process transitions */
    for (long unsigned int i = 0; i < TRANS_COUNT_WARNING; i++) {
        if ((state == trans[i].state) || (ST_ANY_WARNING == trans[i].state)) {
            if ((event == trans[i].event) || (EV_ANY_WARNING == trans[i].event)) {
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
