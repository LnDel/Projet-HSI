/**
 * \file        fsm_high_beams.c
 * \author      Samy Belbouab
 * \version     0.1
 * \date        06 December 2024
 * \brief       Implementation file for the Finite State Machine controlling the high beams.
 */

#include <stdlib.h>
#include <stdio.h>

#include "bcgv_lib.h"
#include "fsm_high_beams.h"

/* Callback functions called on transitions */

static int callback1_high_beams(void) {
  printf("High beams turned off\n");
  set_activationHighBeams(0);
  set_indicatorHighBeams(0);
  return 0;
}

static int callback2_high_beams(void) {
  printf("High beams turned on\n");
  set_activationHighBeams(1);
  set_indicatorHighBeams(1);
  return 0;
}

static int callback3_high_beams(void) {
  printf("High beams acquitted\n");
  return 0;
}

static int FsmError_high_beams(void) {
  printf("High beams Error occurred\n");
  return -1;
}

/* Transition structure */
typedef struct {
  high_beams_state_t state;
  high_beams_event_t event;
  int (*callback)(void);
  high_beams_state_t next_state;
} high_beams_transition_t;

/* Transition table */
static high_beams_transition_t trans_high_beams[] = {
  { ST_INIT_high_beams, EV_NONE_high_beams, &callback1_high_beams, ST_OFF_high_beams },
  { ST_OFF_high_beams, EV_CMD0_high_beams, &callback1_high_beams, ST_OFF_high_beams },
  { ST_OFF_high_beams, EV_CMD1_high_beams, &callback2_high_beams, ST_ON_high_beams },
  { ST_ON_high_beams, EV_CMD1_high_beams, &callback2_high_beams, ST_ON_high_beams },
  { ST_ON_high_beams, EV_CMD0_high_beams, &callback1_high_beams, ST_OFF_high_beams },
  { ST_ON_high_beams, EV_ACQ_RECEIVED_high_beams, &callback3_high_beams, ST_ACQUITTED_high_beams },
  { ST_ON_high_beams, EV_ACQ_TIMEOUT_high_beams, &FsmError_high_beams, ST_ERROR_high_beams },
  { ST_ACQUITTED_high_beams, EV_CMD1_high_beams, &callback3_high_beams, ST_ACQUITTED_high_beams },
  { ST_ACQUITTED_high_beams, EV_CMD0_high_beams, &callback1_high_beams, ST_OFF_high_beams },
  { ST_ANY_high_beams, EV_ERR_high_beams, &FsmError_high_beams, ST_TERM_high_beams }
};

#define TRANS_COUNT_HIGH_BEAMS (sizeof(trans_high_beams)/sizeof(*trans_high_beams))

high_beams_event_t get_next_event_high_beams(high_beams_state_t currentState, time_t currentTime) {
  high_beams_event_t event = EV_NONE_high_beams;

  cmd_t cmd = get_cmdHighBeams();
  indicator_t acq = get_indicatorHighBeams();

  if (cmd == 0) {
    event = EV_CMD0_high_beams;
  } else if (currentState == ST_ON_high_beams) {
    unsigned long currentTimeSeconds = difftime(currentTime, 0);
    time_t timer = time(NULL);
    unsigned long timerSeconds = difftime(timer, 0);
    if (acq == 1) {
      event = EV_ACQ_RECEIVED_high_beams;
    } else if (timerSeconds - currentTimeSeconds > 1) {
      event = EV_ACQ_TIMEOUT_high_beams;
    } else {
      event = EV_NONE_high_beams;
    }
  } else if (cmd == 1) {
    event = EV_CMD1_high_beams;
  } else {
    event = EV_ERR_high_beams;
  }
  return event;
}

high_beams_state_t main_fsm_high_beams(high_beams_state_t currentState) {
  high_beams_state_t state = currentState;
  high_beams_event_t event = get_next_event_high_beams(state, time(NULL));

  for (int i = 0; i < TRANS_COUNT_HIGH_BEAMS; i++) {
    if ((state == trans_high_beams[i].state) || (ST_ANY_high_beams == trans_high_beams[i].state)) {
      if ((event == trans_high_beams[i].event) || (EV_ANY_high_beams == trans_high_beams[i].event)) {
        state = trans_high_beams[i].next_state;
        if (trans_high_beams[i].callback != NULL) {
          trans_high_beams[i].callback();
        }
        break;
      }
    }
  }
  return state;
}