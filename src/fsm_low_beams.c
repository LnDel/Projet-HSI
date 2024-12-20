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
#include <stdint.h>
#include <time.h>

#include "bcgv_lib.h"

/* States */
typedef enum {
    ST_ANY = -1,                            /* Any state */
    ST_INIT = 0,                            /* Init state */
    ST_OFF = 1,                             /* Off state */
    ST_ON = 2,                              /* On state */
    ST_ERROR = 3,                           /* Error state */
    ST_ACQUITTED = 4,                       /* Acquitted state */
    ST_TERM = 255                           /* Final state */
} fsm_state_t;

/* Events */
typedef enum {
    EV_ANY = -1,                            /* Any event */
    EV_NONE = 0,                            /* No event */
    EV_CMD0 = 1,                            /* Cmd = 0 */
    EV_CMD1 = 2,                            /* Cmd = 1 */
    EV_ACQ_RECEIVED = 3,                     /* Acquittement received */
    EV_ACQ_TIMEOUT = 4,                     /* Acquittement timeout */
    EV_ERR = 255                            /* Error event */
} fsm_event_t;

/* Callback functions called on transitions */

// ============ Need to modify it ============
static int callback1(void) {
    printf("turn off\n");
    return 0;
}

static int callback2(void) {
    printf("Callback 2 executed\n");
    return 0;
}

static int callback3(void) {
    printf("Callback 3 executed\n");
    return 0;
}

static int FsmError(void) {
    printf("FSM Error occurred\n");
    return -1;
}

/* Transition structure */
typedef struct {
    fsm_state_t state;
    fsm_event_t event;
    int (*callback)(void);
    int next_state;
} tTransition;

/* Transition table */
tTransition trans[] = {
    { ST_INIT, EV_NONE, &callback1, ST_OFF},
    { ST_OFF, EV_CMD0, &callback1, ST_OFF},
    { ST_OFF, EV_CMD1, &callback2, ST_ON},
    { ST_ON, EV_CMD1, &callback2, ST_ON},
    { ST_ON, EV_CMD0, &callback1, ST_OFF},
    { ST_ON, EV_ACQ_RECEIVED, &callback3, ST_ACQUITTED},
    { ST_ON, EV_ACQ_TIMEOUT, &FsmError, ST_ERROR},
    { ST_ACQUITTED, EV_CMD1, &callback3, ST_ACQUITTED},
    { ST_ACQUITTED, EV_CMD0, &callback1, ST_OFF},

    { ST_ANY, EV_ERR, &FsmError, ST_TERM}
};

#define TRANS_COUNT (sizeof(trans)/sizeof(*trans))

int get_next_event(int currentState, time_t currentTime)
{
  int event = EV_NONE;
  
  cmd_t cmd = get_cmdLowBeams(); // get the cmd parameter
  activation_t acq = get_activationLowBeams(); // get the acq parameter

  if(cmd == 0) {
    event = EV_CMD0;
  }else if (currentState == ST_ON) {
    unsigned long currentTimeSeconds = difftime(currentTime, 0);
    time_t timer = time(NULL);
    unsigned long timerSeconds = difftime(timer, 0);
    if (acq == 1) {
      event = EV_ACQ_RECEIVED;
    }
    else if (timerSeconds - currentTimeSeconds > 1) { // get the timeout event if there is no response since 1 second
      event = EV_ACQ_TIMEOUT;
    }
    else event = get_next_event(currentState, currentTime);
  }else if (cmd == 1) {
    event = EV_CMD1;
  }else event = EV_ERR; 
  return event;
}

int main(void)
{
  int i = 0;
  int ret = 0; 
  int event = EV_NONE;
  int state = ST_INIT;
  
  /* While FSM hasn't reach end state */
  while (state != ST_TERM) {
    /* Get event */
    time_t currentTime = time(NULL);
    event = get_next_event(state, currentTime);
    
    /* For each transitions */
    for (i = 0; i < TRANS_COUNT; i++) {
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
  }

  return ret;
}