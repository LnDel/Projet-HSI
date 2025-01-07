/**
 * \file        fsm.c
 * \author      Alexis Daley
 * \version     0.4
 * \date        08 otober 2023
 * \brief       This is a template file to create a Finite State Machine.
 * \details
 */
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <time.h>
#include "bcgv_lib.h"
#include "fsm_windshield.h"
/* Callback functions called on transitions */
static int callback1 (void) { //ST_ALLOFF
  printf("Windshield wiper and washer turned off\n");
  set_activationShieldWasher(0);
  set_activationShieldWiper(0);
};
static int callback2 (void) { //ST_WINDSHIELDWIPER_ON
printf("Windshield wiper turned on\n");
  set_activationShieldWiper(1); 
};
static int callback3 (void) { //ST_WIPER_AND_WASHER_ON
  printf("Windshield wiper and washer turned on\n");
  set_activationShieldWasher(1);
  set_activationShieldWasher(1);
};
static int callback4 (void) { //ST_TIMERWIPER_AND_WASHEROFF
  printf("Windshield wiper timer and washer turned off\n");
  set_activationShieldWasher(1);
  set_activationShieldWasher(0);
};
static int FsmError(void) {   
  printf("ERREUR");
};
/* Transition structure */
typedef struct {
    windshield_state_t state;
    windshield_event_t event;
    int (*callback)(void);
    int next_state;
} windshield_transition;
/* Transition table */
static windshield_transition trans[] = {
    { ST_INIT_WINDSHIELD, EV_ANY_WINDSHIELD, &callback1, ST_ALLOFF_WINDSHIELD },
    { ST_ALLOFF_WINDSHIELD, EV_CMD_WI1_WINDSHIELD, &callback2, ST_WINDSHIELDWIPER_ON_WINDSHIELD },
    { ST_ALLOFF_WINDSHIELD, EV_CMD_WA1_WINDSHIELD, &callback3, ST_WIPER_AND_WASHER_ON_WINDSHIELD },
    { ST_TIMERWIPER_AND_WASHEROFF_WINDSHIELD, EV_CMD_WA1_WINDSHIELD, &callback3, ST_WIPER_AND_WASHER_ON_WINDSHIELD },
    { ST_TIMERWIPER_AND_WASHEROFF_WINDSHIELD, EV_TIME_OVER_2_WINDSHIELD, &callback1, ST_ALLOFF_WINDSHIELD },
    { ST_TIMERWIPER_AND_WASHEROFF_WINDSHIELD, EV_TIME_UNDER_2_WINDSHIELD, &callback4, ST_TIMERWIPER_AND_WASHEROFF_WINDSHIELD },
    { ST_WIPER_AND_WASHER_ON_WINDSHIELD, EV_CMD_WA1_WINDSHIELD, &callback3, ST_WIPER_AND_WASHER_ON_WINDSHIELD },
    { ST_WIPER_AND_WASHER_ON_WINDSHIELD, EV_CMD_WA0_WINDSHIELD, &callback4, ST_TIMERWIPER_AND_WASHEROFF_WINDSHIELD },
    { ST_WINDSHIELDWIPER_ON_WINDSHIELD, EV_CMD_WA1_WINDSHIELD, &callback3, ST_WIPER_AND_WASHER_ON_WINDSHIELD },
    { ST_WINDSHIELDWIPER_ON_WINDSHIELD, EV_CMD_WI0_WINDSHIELD, &callback1, ST_ALLOFF_WINDSHIELD },
    { ST_ANY_WINDSHIELD, EV_ERR_WINDSHIELD, &FsmError, ST_TERM_WINDSHIELD }
};
#define TRANS_COUNT (sizeof(trans)/sizeof(*trans))
windshield_event_t get_next_event_windshield(windshield_state_t current_state, long unsigned currentTimeSeconds) {
    windshield_event_t event = EV_NONE_WINDSHIELD;
    cmd_t cmdWasher = get_cmdWindShieldWasher(); // Get the command for the windshield washer
    cmd_t cmdWiper = get_cmdWindShieldWiper();   // Get the command for the windshield wiper
    time_t timer = time(NULL);
    unsigned long timerSeconds = difftime(timer, 0);

    if (cmdWasher == 0 && cmdWiper == 0) {
        event = get_next_event(current_state, currentTimeSeconds);
    } else if (current_state == ST_ALLOFF_WINDSHIELD) {
        if (cmdWiper == 1) {
            event = EV_CMD_WI1_WINDSHIELD;
        } else { // cmdWasher == 1
            event = EV_CMD_WA1_WINDSHIELD;
        }
    } else if (current_state == ST_WINDSHIELDWIPER_ON_WINDSHIELD) {
        if (cmdWasher == 1) {
            event = EV_CMD_WA1_WINDSHIELD;
        } else if (cmdWiper == 0) {
            event = EV_CMD_WI0_WINDSHIELD;
        } else { // cmdWiper == 1
            event = get_next_event(current_state, currentTimeSeconds);
        }
    } else if (current_state == ST_WIPER_AND_WASHER_ON_WINDSHIELD) {
        if (cmdWasher == 0) {
            event = EV_CMD_WA0_WINDSHIELD;
        } else { // cmdWasher == 1
            event = get_next_event(current_state, currentTimeSeconds);
        }
    } else if (timerSeconds - currentTimeSeconds > 2) {
        event = EV_TIME_OVER_2_WINDSHIELD;
    } else { // timerSeconds - currentTimeSeconds < 2
        event = get_next_event(current_state, currentTimeSeconds);
    }

    return event;
}

windshield_state_t main_fsm_windshield(windshield_state_t currentState) {
    int ret = 0;
    windshield_state_t state = currentState;
    windshield_event_t event = get_next_event(state, time(NULL));
    time_t currentTime;
    unsigned long currentTimeSeconds;

    currentTime = time(NULL);
    currentTimeSeconds = difftime(currentTime, 0);
    event = get_next_event(state, currentTimeSeconds);

    /* Process transitions */
    for (int i = 0; i < TRANS_COUNT; i++) {
        /* Check if the transition applies */
        if ((state == trans[i].state) || (ST_ANY_WINDSHIELD == trans[i].state)) {
            if ((event == trans[i].event) || (EV_ANY_WINDSHIELD == trans[i].event)) {
                /* Update the state and call the callback */
                state = trans[i].next_state;
                if (trans[i].callback != NULL) {
                    ret = (trans[i].callback)();
                }
                break;
            }
        }
    }

    return ret;
}