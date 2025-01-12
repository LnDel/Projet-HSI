/**
 * \file fsm_left_turn_signal.c
 * \brief Finite State Machine for the left turn signal.
 * \details Implementation of state transitions and events for the left turn signal FSM.
 * \author Warren Anderson
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
/**
 * \brief Callback for turning off the left turn signal.
 * \details Deactivates the left turn signal.
 * \return int : 0 on success.
 */
static int callback1(void) {
    set_activationLeftTurnSignal(0);
    return 0;
}

/**
 * \brief Callback for activating and turning on the left turn signal.
 * \details Activates and lights up the left turn signal.
 * \return int : 0 on success.
 */
static int callback2(void) {
    set_activationLeftTurnSignal(1);
    set_indicatorLeftTurnSignal(1);
    return 0;
}

/**
 * \brief Callback for acquitting the left turn signal.
 * \details Sets the left turn signal indicator.
 * \return int : 0 on success.
 */
static int callback3(void) {
    set_indicatorLeftTurnSignal(1);
    return 0;
}

/**
 * \brief Callback for activating and turning off the left turn signal.
 * \details Activates but turns off the left turn signal indicator.
 * \return int : 0 on success.
 */
static int callback4(void) {
    set_activationLeftTurnSignal(0);
    set_indicatorLeftTurnSignal(0);
    return 0;
}

/**
 * \brief Callback for FSM error handling.
 * \details Prints an error message.
 * \return int : -1 on error.
 */
static int FsmError(void) {
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

left_turn_signal_event_t get_next_event_left_turnsignal(left_turn_signal_state_t currentState) {
    left_turn_signal_event_t event = EV_NONE_LEFT_TURNSIGNAL;
    cmd_t cmd = get_cmdLeftTurnSignal();
    indicator_t acq = get_indicatorLeftTurnSignal();

    time_t timer = time(NULL);
    unsigned long timerSeconds = difftime(timer, 0);

    if (lastTimerSeconds == 0) {
        lastTimerSeconds = timerSeconds;
    }

    if (cmd == 0) {
        event = EV_CMD0_LEFT_TURNSIGNAL;
    } else if (currentState == ST_ACQUITTED_LEFT_TURNSIGNAL || currentState == ST_ACQUITTED_LEFT_TURNSIGNAL_OFF) {
        if ((timerSeconds - lastTimerSeconds) >= 1) {
            event = EV_TIME1_LEFT_TURNSIGNAL;
            lastTimerSeconds = timerSeconds;
        } else {
            event = EV_CMD1_LEFT_TURNSIGNAL;
        }
    } else if (currentState == ST_ACTIVATED_AND_ON_LEFT_TURNSIGNAL || currentState == ST_ACTIVATED_AND_OFF_LEFT_TURNSIGNAL) {
        if (acq == 1) {
            event = EV_ACQ_RECEIVED_LEFT_TURNSIGNAL;
        } else if ((timerSeconds - lastTimerSeconds) > 1) {
            event = EV_ACQ_TIMEOUT_LEFT_TURNSIGNAL;
        } else {
            event = EV_CMD1_LEFT_TURNSIGNAL;
        }
    } else if (cmd == 1) {
        event = EV_CMD1_LEFT_TURNSIGNAL;
    } else {
        event = EV_ERR_LEFT_TURNSIGNAL;
    }
    return event;
}

left_turn_signal_state_t main_fsm_left_turnsignal(left_turn_signal_state_t currentState) {
    left_turn_signal_state_t state = currentState;
    left_turn_signal_event_t event = get_next_event_left_turnsignal(state);

    if (state == ST_INIT_LEFT_TURNSIGNAL) {
        return ST_OFF_LEFT_TURNSIGNAL;
    }

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
    return state;
}