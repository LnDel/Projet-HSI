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
/* States */
typedef enum {
  ST_ANY = -1,                            /* Any state */
  ST_INIT = 0,                            /* Init state */
  ST_ALLOFF= 1,
  ST_WINDSHIELDWIPER_ON= 2,
  ST_WIPER_AND_WASHER_ON= 3,
  ST_TIMERWIPER_AND_WASHEROFF= 4,
  ST_TERM = 255,                 /* Final state */
} windshield_state_t;
/* Events */
typedef enum {
  EV_ANY = -1,                            /* Any event */
  EV_NONE = 0,                            /* No event */
  EV_CMD_WI0 = 1,                         // WI == Wiper
  EV_CMD_WA0 = 2,                         // WA == Washer
  EV_CMD_WI1= 3,
  EV_CMD_WA1= 4,
  EV_TIME_UNDER_2=5,
  EV_TIME_OVER_2=6,
  EV_ERR = 255                            /* Error event */
} windshield_event_t;
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
} tTransition;
/* Transition table */
tTransition trans[] = {
  /* These are examples */
  { ST_INIT, EV_ANY, &callback1, ST_ALLOFF},
  { ST_ALLOFF, EV_CMD_WI1, &callback2, ST_WINDSHIELDWIPER_ON},
  { ST_ALLOFF, EV_CMD_WA1, &callback3, ST_WIPER_AND_WASHER_ON},
  { ST_TIMERWIPER_AND_WASHEROFF, EV_CMD_WA1, &callback3, ST_WIPER_AND_WASHER_ON},
  { ST_TIMERWIPER_AND_WASHEROFF, EV_TIME_OVER_2, &callback1, ST_ALLOFF},
  { ST_TIMERWIPER_AND_WASHEROFF, EV_TIME_UNDER_2, &callback4, ST_TIMERWIPER_AND_WASHEROFF},
  { ST_WIPER_AND_WASHER_ON, EV_CMD_WA1, &callback3, ST_WIPER_AND_WASHER_ON},
  { ST_WIPER_AND_WASHER_ON, EV_CMD_WA0, &callback4, ST_TIMERWIPER_AND_WASHEROFF},
  { ST_WINDSHIELDWIPER_ON, EV_CMD_WA1, &callback3, ST_WIPER_AND_WASHER_ON},
  { ST_WINDSHIELDWIPER_ON, EV_CMD_WI0, &callback1, ST_ALLOFF},
  { ST_ANY, EV_ERR, &FsmError, ST_TERM}
};
#define TRANS_COUNT (sizeof(trans)/sizeof(*trans))
windshield_event_t get_next_event(windshield_state_t current_state, long unsigned currentTimeSeconds)
{
  windshield_event_t event = EV_NONE;
  cmd_t cmdWasher = get_cmdWindShieldWasher(); // get the cmd parameter for the windshield washer
  cmd_t cmdWiper = get_cmdWindShieldWiper(); // get the cmd parameter for the windshield wiper
  indicator_t acq = getIndicatorPositionLight(); // get the acq parameter
  time_t timer = time(NULL);
  unsigned long timerSeconds = difftime(timer, 0);
  if(cmdWasher == 0 && cmdWiper == 0){
    event = get_next_event(current_state, currentTimeSeconds);
  }else if (current_state == ST_ALLOFF) {
    if (cmdWiper == 1) {
      event = EV_CMD_WI1;
    }else { // (cmdWasher == 1) 
      event = EV_CMD_WA1;
    }
  }else if (current_state == ST_WINDSHIELDWIPER_ON) {
    if (cmdWasher == 1) {
      event = EV_CMD_WA1;
    }else if (cmdWiper == 0) {
      event = EV_CMD_WI0;
    }else { // (cmdWiper == 1)
      event = get_next_event(current_state, currentTimeSeconds);
    }
  }else if (current_state == ST_WIPER_AND_WASHER_ON) {
    if (cmdWasher == 0) {
      event = EV_CMD_WA0;
    }else { // (cmdWasher == 1)
      event = get_next_event(current_state, currentTimeSeconds);
    }
  }else if (timerSeconds - currentTimeSeconds > 2) {
    event = EV_TIME_OVER_2;
  }else { // (timerSeconds - currentTimeSeconds < 2)
    event = get_next_event(current_state, currentTimeSeconds);
  }
  return event;
}
windshield_state_t main_fsm_windshield(currentState)
{
  int ret = 0; 
  windshield_state_t state = currentState;
  windshield_event_t event = get_next_event(state, time(NULL));
  time_t currentTime;
  unsigned long currentTimeSeconds;

    currentTime = time(NULL);
    currentTimeSeconds = difftime(currentTime, 0);
    event = get_next_event(state, currentTimeSeconds);
    
    /* For each transitions */
    for (int i = 0; i < TRANS_COUNT; i++) {
      /* If State is current state OR The transition applies to all states ...*/
      if ((state == trans[i].state) || (ST_ANY == trans[i].state)) {
        /* If event is the transition event OR the event applies to all */
        if ((event == trans[i].event) || (EV_ANY == trans[i].event)) {
          /* Apply the new state */
          state = trans[i].next_state;
          if (trans[i].callback != NULL) {
              /* Call the state function */
              ret = (trans[i].callback)();
          }
          break;
        }
    }
  }
  return ret;
}