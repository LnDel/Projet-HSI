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

/* States */
typedef enum {
    ST_ANY = -1,                            /* Any state */
    ST_INIT = 0,                            /* Init state */
    ST_ALLOFF= 1,
    ST_WINDSHIELDWIPER_ON= 2,
    ST_WIPER_AND_WASHER_ON= 3,
    ST_TIMERWIPER_AND_WASHEROFF= 4,
    ST_TERM = 255,                 /* Final state */
} fsm_state_t;

/* Events */
typedef enum {
    EV_ANY = -1,                            /* Any event */
    EV_NONE = 0,                            /* No event */
    EV_CMD_EG0 = 1,
    EV_CMD_LG0 = 2,
    EV_CMD_EG1= 3,
    EV_CMD_LG1= 4,
    EV_TIME_UNDER_2=5,
    EV_TIME_OVER_2=6,
    EV_ERR = 255                            /* Error event */
} fsm_event_t;

/* Callback functions called on transitions */
static int callback1 (void) { printf("callback1"); };
static int callback2 (void) { printf("callback2");};
static int callback3 (void) { printf("callback3"); };
static int FsmError(void) {   printf("ERREUR");};

/* Transition structure */
typedef struct {
    fsm_state_t state;
    fsm_event_t event;
    int (*callback)(void);
    int next_state;
} tTransition;

/* Transition table */
tTransition trans[] = {
    /* These are examples */
    { ST_INIT, EV_ANY, &callback1, ST_ALLOFF},
    { ST_ALLOFF, EV_CMD_EG1, &callback2, ST_WINDSHIELDWIPER_ON},
    { ST_ALLOFF, EV_CMD_LG1, &callback2, ST_WIPER_AND_WASHER_ON},
    { ST_TIMERWIPER_AND_WASHEROFF, EV_CMD_LG1, &callback2, ST_WIPER_AND_WASHER_ON},
    { ST_TIMERWIPER_AND_WASHEROFF, EV_TIME_OVER_2, &callback3, ST_ALLOFF},
    { ST_TIMERWIPER_AND_WASHEROFF, EV_TIME_UNDER_2, &callback2, ST_TIMERWIPER_AND_WASHEROFF},
    { ST_WIPER_AND_WASHER_ON, EV_CMD_LG1, &callback1, ST_WIPER_AND_WASHER_ON},
    { ST_WIPER_AND_WASHER_ON, EV_CMD_LG0, &callback1, ST_TIMERWIPER_AND_WASHEROFF},
    { ST_WINDSHIELDWIPER_ON, EV_CMD_LG1, &callback3, ST_WIPER_AND_WASHER_ON},
    { ST_WINDSHIELDWIPER_ON, EV_CMD_EG0, &callback3, ST_ALLOFF},
    { ST_ANY, EV_ERR, &FsmError, ST_TERM}
};

#define TRANS_COUNT (sizeof(trans)/sizeof(*trans))

int get_next_event(int current_state)
{
  int event = EV_NONE;
  cmd_t cmd = getCmdPositionLight(); // get the cmd parameter
  activation_t acq = getActivationPositionLight(); // get the acq parameter

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
        event = get_next_event(state);
        
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
