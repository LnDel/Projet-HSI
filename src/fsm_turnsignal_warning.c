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
  ST_OFF= 1,
  ST_ACTIVATED_AND_ON= 2,
  ST_ACQUITTED= 3,
  ST_ACTIVATED_AND_OFF= 4,
  ST_ERROR=5,
  ST_TERM = 255,                 /* Final state */
} turn_signal_state_t;

/* Events */
typedef enum {
  EV_ANY = -1,                            /* Any event */
  EV_NONE = 0,                            /* No event */
  EV_CMD0 = 1,
  EV_CMD1 = 2,
  EV_ACQ_RECEIVED= 3,
  EV_ACQ_TIMEOUT= 4,
  EV_TIME1=5,
  EV_ERR = 255                            /* Error event */
} turn_signal_event_t;

/* Callback functions called on transitions */
static int callback1 (void) { //ST_OFF
  printf("callback1"); 
};
static int callback2 (void) { //ST_ACTIVATED_AND_ON
  printf("callback1"); 
};
static int callback3 (void) { //ST_ACQUITTED
  printf("callback1"); 
};
static int callback4 (void) { //ST_ACTIVATED_AND_OFF
  printf("callback1"); 
};

static int callback2 (void) { printf("callback2");};
static int callback3 (void) { printf("callback3"); };
static int FsmError(void) {   printf("ERREUR");};

/* Transition structure */
typedef struct {
  turn_signal_state_t state;
  turn_signal_event_t event;
  int (*callback)(void);
  int next_state;
} tTransition;

/* Transition table */
tTransition trans[] = {
  /* These are examples */
  { ST_INIT, EV_ANY, &callback1, ST_OFF},
  { ST_OFF, EV_CMD1, &callback2, ST_ACTIVATED_AND_ON},
  { ST_ACTIVATED_AND_ON, EV_ACQ_RECEIVED, &callback3, ST_ACQUITTED},
  { ST_ACTIVATED_AND_ON, EV_ACQ_TIMEOUT, &FsmError, ST_ERROR},
  { ST_ACTIVATED_AND_ON, EV_CMD1, &callback2, ST_ACTIVATED_AND_ON},
  { ST_ACTIVATED_AND_ON, EV_CMD0, &callback1, ST_OFF},
  { ST_ACQUITTED, EV_CMD0, &callback1, ST_OFF},
  { ST_ACQUITTED, EV_CMD1, &callback3, ST_ACQUITTED},
  { ST_ACQUITTED, EV_TIME1, &callback4, ST_ACTIVATED_AND_OFF},
  { ST_ACTIVATED_AND_OFF, EV_ACQ_TIMEOUT, &FsmError, ST_ERROR},
  { ST_ACTIVATED_AND_OFF, EV_ACQ_RECEIVED, &callback3, ST_ACQUITTED},
  { ST_ACTIVATED_AND_OFF, EV_CMD1, &callback4, ST_ACTIVATED_AND_OFF},
  { ST_ACTIVATED_AND_OFF, EV_CMD0, &callback1, ST_OFF},
  { ST_ACQUITTED, EV_TIME1, &callback2, ST_ACTIVATED_AND_ON},
  { ST_ACQUITTED, EV_CMD0, &callback1, ST_OFF},
  { ST_ANY, EV_ERR, &FsmError, ST_TERM}
};

#define TRANS_COUNT (sizeof(trans)/sizeof(*trans))

turn_signal_event_t get_next_event(int currentState, unsigned long currentTimeSeconds)
{
  turn_signal_event_t event = EV_NONE;
  cmd_t cmd = getCmdPositionLight(); // get the cmd parameter
  indicator_t acq = getIndicatorPositionLight(); // get the acq parameter

  time_t timer = time(NULL);
  unsigned long timerSeconds = difftime(timer, 0);

  if(cmd == 0) {
    event = EV_CMD0;
  }else if (cmd == 1) {
    event = EV_CMD1;
  }else if (acq == 1) {
    event = EV_ACQ_RECEIVED;
  }
  // ================ Need to add a condition depending on a timer (1 second) ================
  else if (acq == 0) {
    event = EV_ACQ_TIMEOUT;
  }
  
  if(cmd == 0) {
    event = EV_CMD0;
  }
  else if(currentState == ST_ACTIVATED_AND_ON){
    if(acq == 1){
      event = EV_ACQ_RECEIVED;
    }
    else if((timerSeconds - currentTimeSeconds) > 1){
      event = EV_ACQ_TIMEOUT;
    }
    else { // cmd == 1 
      event = get_next_event(currentState, currentTimeSeconds);
    }
  }
  else if(currentState == ST_ACTIVATED_AND_OFF){
    if(acq == 1){
      event = EV_ACQ_RECEIVED;
    }
    else if((timerSeconds - currentTimeSeconds) > 1){
      event = EV_ACQ_TIMEOUT;
    }
    else {
      event = get_next_event(currentState, currentTimeSeconds);
    }
  }
  else if(currentState == ST_ACQUITTED){
    if((timerSeconds - currentTimeSeconds) == 1){
      event = EV_TIME1;
    }
    else {
      event = get_next_event(currentState, currentTimeSeconds);
    }
  }

  else if(cmd == 1){
    event = EV_CMD1;
  }
  else {
    event = EV_ERR;
  }
  return event;
}


turn_signal_state_t main(turn_signal_state_t currentState)
{ 
  int ret = 0; 
  turn_signal_state_t state = currentState;
  turn_signal_event_t event = get_next_event(state, time(NULL)); 

  time_t currentTime = time(NULL);
  unsigned long currentTimeSeconds = difftime(currentTime, 0);

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
