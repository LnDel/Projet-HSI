/**
 * \file        fsm_windshield.c
 * \author      Warren Anderson, Samy Belbouab
 * \brief       This is a file to manage the state machine of windshield wiper and washer
 * \details     The FSM manages the windshield wiper and windshield washer functionalities.
 */

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <time.h>
#include "../bcgv_lib.h"
#include "fsm_windshield.h"

/* Callback functions called on transitions */

/**
 * \brief Callback for turning off the windshield wiper and washer.
 * \return Always returns 0.
 */
static int callback1(void) { // ST_ALLOFF
    set_activationShieldWasher(0);
    set_indicatorShieldWasher(0);
    set_activationShieldWiper(0);
    set_indicatorShieldWiper(0);
    return 0;
}

/**
 * \brief Callback for turning on the windshield wiper.
 * \return Always returns 0.
 */
static int callback2(void) { // ST_WINDSHIELDWIPER_ON
    set_activationShieldWiper(1);
    set_indicatorShieldWiper(1);
    return 0;
}

/**
 * \brief Callback for turning on the windshield wiper and washer.
 * \return Always returns 0.
 */
static int callback3(void) { // ST_WIPER_AND_WASHER_ON
    set_activationShieldWiper(1);
    set_indicatorShieldWiper(1);
    set_activationShieldWasher(1);
    set_indicatorShieldWasher(1);
    return 0;
}

/**
 * \brief Callback for turning off the washer and setting the timer for the wiper.
 * \return Always returns 0.
 */
static int callback4(void) { // ST_TIMERWIPER_AND_WASHEROFF
    set_activationShieldWasher(0);
    set_indicatorShieldWasher(0);
    return 0;
}

/**
 * \brief Callback for handling FSM errors.
 * \return Always returns -1.
 */
static int FsmError(void) {   
    printf("ERREUR\n");
    return -1;
}

/* Transition table */
static windshield_transition_t trans_windshield[] = {
    { ST_INIT_WINDSHIELD, EV_ANY_WINDSHIELD, &callback1, ST_ALLOFF_WINDSHIELD },
    { ST_ALLOFF_WINDSHIELD, EV_NONE_WINDSHIELD, &callback1, ST_ALLOFF_WINDSHIELD },
    { ST_ALLOFF_WINDSHIELD, EV_CMD_WI1_WINDSHIELD, &callback2, ST_WINDSHIELDWIPER_ON_WINDSHIELD },
    { ST_ALLOFF_WINDSHIELD, EV_CMD_WA1_WINDSHIELD, &callback3, ST_WIPER_AND_WASHER_ON_WINDSHIELD },
    { ST_WINDSHIELDWIPER_ON_WINDSHIELD, EV_CMD_WI0_WINDSHIELD, &callback1, ST_ALLOFF_WINDSHIELD },
    { ST_WINDSHIELDWIPER_ON_WINDSHIELD, EV_CMD_WI1_WINDSHIELD, &callback2, ST_WINDSHIELDWIPER_ON_WINDSHIELD },
    { ST_WINDSHIELDWIPER_ON_WINDSHIELD, EV_CMD_WA1_WINDSHIELD, &callback3, ST_WIPER_AND_WASHER_ON_WINDSHIELD },
    { ST_WIPER_AND_WASHER_ON_WINDSHIELD, EV_CMD_WA1_WINDSHIELD, &callback3, ST_WIPER_AND_WASHER_ON_WINDSHIELD },
    { ST_WIPER_AND_WASHER_ON_WINDSHIELD, EV_CMD_WA0_WINDSHIELD, &callback4, ST_TIMERWIPER_AND_WASHEROFF_WINDSHIELD },
    { ST_TIMERWIPER_AND_WASHEROFF_WINDSHIELD, EV_CMD_WA1_WINDSHIELD, &callback3, ST_WIPER_AND_WASHER_ON_WINDSHIELD },
    { ST_TIMERWIPER_AND_WASHEROFF_WINDSHIELD, EV_TIME_OVER_2_WINDSHIELD, &callback1, ST_ALLOFF_WINDSHIELD },
    { ST_TIMERWIPER_AND_WASHEROFF_WINDSHIELD, EV_TIME_UNDER_2_WINDSHIELD, &callback4, ST_TIMERWIPER_AND_WASHEROFF_WINDSHIELD },
    { ST_ANY_WINDSHIELD, EV_ERR_WINDSHIELD, &FsmError, ST_TERM_WINDSHIELD }
};

#define TRANS_COUNT_WINDSHIELD (sizeof(trans_windshield)/sizeof(*trans_windshield))

windshield_event_t get_next_event_windshield(windshield_state_t current_state, long unsigned currentTimeSeconds) {
    windshield_event_t event = EV_NONE_WINDSHIELD;
    cmd_t cmdWasher = get_cmdWindShieldWasher();
    cmd_t cmdWiper = get_cmdWindShieldWiper();
    time_t timer = time(NULL);
    unsigned long timerSeconds = difftime(timer, 0);

    if (cmdWasher == 0 && cmdWiper == 0) {
        event = EV_NONE_WINDSHIELD;
    } else if (cmdWasher == 1) {
        event = EV_CMD_WA1_WINDSHIELD;
    } else if (current_state == ST_TIMERWIPER_AND_WASHEROFF_WINDSHIELD) {
        if (timerSeconds - currentTimeSeconds > 2) {
            event = EV_TIME_OVER_2_WINDSHIELD;
        } else {
            event = EV_TIME_UNDER_2_WINDSHIELD;
        }
    } else if (current_state == ST_ALLOFF_WINDSHIELD) {
        if (cmdWiper == 1) {
            event = EV_CMD_WI1_WINDSHIELD;
        } else {
            event = EV_CMD_WI0_WINDSHIELD;
        }
    } else if (current_state == ST_WIPER_AND_WASHER_ON_WINDSHIELD && cmdWasher == 0) {
        event = EV_CMD_WA0_WINDSHIELD;
    } else {
        event = EV_ERR_WINDSHIELD;
    }
    return event;
}

windshield_state_t main_fsm_windshield(windshield_state_t currentState) {
    windshield_state_t state = currentState;
    windshield_event_t event = EV_NONE_WINDSHIELD;
    time_t currentTime;
    unsigned long currentTimeSeconds;

    if (state == ST_INIT_WINDSHIELD) {
        return ST_ALLOFF_WINDSHIELD;
    }

    currentTime = time(NULL);
    currentTimeSeconds = difftime(currentTime, 0);
    event = get_next_event_windshield(state, currentTimeSeconds);

    for (long int unsigned i = 0; i < TRANS_COUNT_WINDSHIELD; i++) {
        if ((state == trans_windshield[i].state) || (ST_ANY_WINDSHIELD == trans_windshield[i].state)) {
            if ((event == trans_windshield[i].event) || (EV_ANY_WINDSHIELD == trans_windshield[i].event)) {
                state = trans_windshield[i].next_state;
                if (trans_windshield[i].callback != NULL) {
                    trans_windshield[i].callback();
                }
                break;
            }
        }
    }
    return state;
}
