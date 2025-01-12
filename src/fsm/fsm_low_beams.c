/**
 * \file fsm_low_beams.c
 * \brief Finite state machine management for low beams.
 * \details This file implements the logic and transitions of the finite state machine for the low beams.
 * \author Samy Belbouab
 */

#include <stdlib.h>
#include <stdio.h>

#include "../bcgv_lib.h"
#include "fsm_low_beams.h"

#define TRANS_COUNT (sizeof(trans)/sizeof(*trans))


/**
 * \brief Callback to deactivate the low beams.
 * \details This function is called during transitions to a state where the low beams should be turned off.
 * \return int: Always returns 0.
 */
static int callback1(void) {
  set_activationLowBeams(0);
  set_indicatorLowBeams(0);
  return 0;
}

/**
 * \brief Callback to activate the low beams.
 * \details This function is called during transitions to a state where the low beams should be turned on.
 * \return int: Always returns 0.
 */
static int callback2(void) {
  set_activationLowBeams(1);
  set_indicatorLowBeams(1);
  return 0;
}

/**
 * \brief Callback to acknowledge the low beams state.
 * \details This function is called during transitions to an acknowledged state.
 * \return int: Always returns 0.
 */
static int callback3(void) {
  return 0;
}

/**
 * \brief Callback to handle low beams errors.
 * \details This function is called when a transition leads to an error state.
 * \return int: Always returns -1.
 */
static int FsmError(void) {
  return -1;
}

/* Transition structure */
typedef struct {
  low_beams_state_t state;
  low_beams_event_t event;
  int (*callback)(void);
  int next_state;
} low_beams_transition_t;

/* Transition table */
static low_beams_transition_t trans[] = {
  { ST_INIT_low_beams, EV_NONE_low_beams, &callback1, ST_OFF_low_beams},
  { ST_OFF_low_beams, EV_CMD0_low_beams, &callback1, ST_OFF_low_beams},
  { ST_OFF_low_beams, EV_CMD1_low_beams, &callback2, ST_ON_low_beams},
  { ST_ON_low_beams, EV_CMD1_low_beams, &callback2, ST_ON_low_beams},
  { ST_ON_low_beams, EV_CMD0_low_beams, &callback1, ST_OFF_low_beams},
  { ST_ON_low_beams, EV_ACQ_RECEIVED_low_beams, &callback3, ST_ACQUITTED_low_beams},
  { ST_ON_low_beams, EV_ACQ_TIMEOUT_low_beams, &FsmError, ST_ERROR_low_beams},
  { ST_ACQUITTED_low_beams, EV_CMD1_low_beams, &callback3, ST_ACQUITTED_low_beams},
  { ST_ACQUITTED_low_beams, EV_CMD0_low_beams, &callback1, ST_OFF_low_beams},

  { ST_ANY_low_beams, EV_ERR_low_beams, &FsmError, ST_TERM_low_beams}
};

low_beams_event_t get_next_event_low_beams(low_beams_state_t currentState, time_t currentTime)
{
  low_beams_event_t event = EV_NONE_low_beams;
  
  cmd_t cmd = get_cmdLowBeams(); // Gets the current command
  indicator_t acq = get_indicatorLowBeams(); // Gets the current indicator

  if(cmd == 0) {
    event = EV_CMD0_low_beams;
  } else if (currentState == ST_ON_low_beams) {
    unsigned long currentTimeSeconds = difftime(currentTime, 0);
    time_t timer = time(NULL);
    unsigned long timerSeconds = difftime(timer, 0);
    if (acq == 1) {
      event = EV_ACQ_RECEIVED_low_beams;
    }
    else if (timerSeconds - currentTimeSeconds > 1) { // Timeout after 1 second without response
      event = EV_ACQ_TIMEOUT_low_beams;
    }
    else event = get_next_event_low_beams(currentState, currentTime);
  } else if (cmd == 1) {
    event = EV_CMD1_low_beams;
  } else {
    event = EV_ERR_low_beams; 
  }
  return event;
}

low_beams_state_t main_fsm_low_beams(low_beams_state_t currentState)
{
  low_beams_state_t state = currentState;
  low_beams_event_t event = get_next_event_low_beams(state, time(NULL));

  // Initialization
  if (state == ST_INIT_low_beams){
    return ST_OFF_low_beams;
  }

  for (long unsigned int i = 0; i < TRANS_COUNT; i++) {
  /* If the current state matches the transition or if the transition applies to any state */
    if ((state == trans[i].state) || (ST_ANY_low_beams == trans[i].state)) {
      /* If the event matches or if the transition applies to any event */
      if ((event == trans[i].event) || (EV_ANY_low_beams == trans[i].event)) {
        /* Apply the next state */
        state = trans[i].next_state;
        if (trans[i].callback != NULL) {
          /* Call the transition function */
          trans[i].callback();
        }
        break;
      }
    }
  }
  return state; // Return the new state
}