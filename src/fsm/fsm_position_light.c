/**
 * \file        fsm_position_light.c
 * \author      Samy Belbouab
 * \version     0.1
 * \date        06 december 2024
 * \brief       This is a template file to create a Finite State Machine.
 * \details
*/

#include <stdlib.h>
#include <stdio.h>

#include "../bcgv_lib.h"

#include "fsm_position_light.h"

/* Callback functions called on transitions */

// ============ Need to modify it ============
static int callback1(void) {
  set_activationPositionLights(0);
  set_indicatorPositionLights(0);
  return 0;
}

static int callback2(void) {
  set_activationPositionLights(1);
  set_indicatorPositionLights(1);
  return 0;
}

static int callback3(void) {
  return 0;
}

static int FsmError(void) {
  printf("Position light Error occurred\n");
  return -1;
}

/* Transition structure */
typedef struct {
  position_light_state_t state;
  position_light_event_t event;
  int (*callback)(void);
  int next_state;
} position_light_transition_t;

/* Transition table */
static position_light_transition_t trans[] = {
  { ST_INIT_position_light, EV_NONE_position_light, &callback1, ST_OFF_position_light},
  { ST_OFF_position_light, EV_CMD0_position_light, &callback1, ST_OFF_position_light},
  { ST_OFF_position_light, EV_CMD1_position_light, &callback2, ST_ON_position_light},
  { ST_ON_position_light, EV_CMD1_position_light, &callback2, ST_ON_position_light},
  { ST_ON_position_light, EV_CMD0_position_light, &callback1, ST_OFF_position_light},
  { ST_ON_position_light, EV_ACQ_RECEIVED_position_light, &callback3, ST_ACQUITTED_position_light},
  { ST_ON_position_light, EV_ACQ_TIMEOUT_position_light, &FsmError, ST_ERROR_position_light},
  { ST_ACQUITTED_position_light, EV_CMD1_position_light, &callback3, ST_ACQUITTED_position_light},
  { ST_ACQUITTED_position_light, EV_CMD0_position_light, &callback1, ST_OFF_position_light},

  { ST_ANY_position_light, EV_ERR_position_light, &FsmError, ST_TERM_position_light}
};

#define TRANS_COUNT (sizeof(trans)/sizeof(*trans))

position_light_event_t get_next_event_position_light(position_light_state_t currentState, time_t currentTime)
{
  position_light_event_t event = EV_NONE_position_light;
  
  cmd_t cmd = get_cmdPositionLights(); // get the cmd parameter
  indicator_t acq = get_indicatorPositionLights(); // get the acq parameter

  if(cmd == 0) {
    event = EV_CMD0_position_light;
  }else if (currentState == ST_ON_position_light) {
    unsigned long currentTimeSeconds = difftime(currentTime, 0);
    time_t timer = time(NULL);
    unsigned long timerSeconds = difftime(timer, 0);
    if (acq == 1) {
      event = EV_ACQ_RECEIVED_position_light;
    }
    else if (timerSeconds - currentTimeSeconds > 1) { // get the timeout event if there is no response since 1 second
      event = EV_ACQ_TIMEOUT_position_light;
    }
    else event = get_next_event_position_light(currentState, currentTime);
  }else if (cmd == 1) {
    event = EV_CMD1_position_light;
  }else event = EV_ERR_position_light; 
  return event;
}

position_light_state_t main_fsm_position_light(position_light_state_t currentState)
{
  position_light_state_t state = currentState;
  position_light_event_t event = get_next_event_position_light(state, time(NULL));

  // for the initialisation
  if (state == ST_INIT_position_light){
    return ST_OFF_position_light;
  }

  for (int i = 0; i < TRANS_COUNT; i++) {
  /* If State is current state OR The transition applies to all states ...*/
    if ((state == trans[i].state) || (ST_ANY_position_light == trans[i].state)) {
      /* If event is the transition event OR the event applies to all */
      if ((event == trans[i].event) || (EV_ANY_position_light == trans[i].event)) {
        /* Apply the new state */
        state = trans[i].next_state;
        if (trans[i].callback != NULL) {
          /* Call the state function */
          trans[i].callback();
        }
        break;
      }
    }
  }
  return state; // return the new state
}