/**
 * \file        fsm_high_beams.h
 * \author      Samy Belbouab
 * \version     0.1
 * \date        06 December 2024
 * \brief       Header file for the Finite State Machine controlling the high beams.
 * \details
 */

#ifndef FSM_HIGH_BEAMS_H
#define FSM_HIGH_BEAMS_H

#include <stdint.h>
#include <time.h>

/* States */
typedef enum {
    ST_ANY_high_beams = -1,                            /* Any state */
    ST_INIT_high_beams = 0,                            /* Init state */
    ST_OFF_high_beams = 1,                             /* Off state */
    ST_ON_high_beams = 2,                              /* On state */
    ST_ERROR_high_beams = 3,                           /* Error state */
    ST_ACQUITTED_high_beams = 4,                       /* Acquitted state */
    ST_TERM_high_beams = 255                           /* Final state */
} high_beams_state_t;

/* Events */
typedef enum {
    EV_ANY_high_beams = -1,                            /* Any event */
    EV_NONE_high_beams = 0,                            /* No event */
    EV_CMD0_high_beams = 1,                            /* Cmd = 0 */
    EV_CMD1_high_beams = 2,                            /* Cmd = 1 */
    EV_ACQ_RECEIVED_high_beams = 3,                    /* Acquittement received */
    EV_ACQ_TIMEOUT_high_beams = 4,                     /* Acquittement timeout */
    EV_ERR_high_beams = 255                            /* Error event */
} high_beams_event_t;

/**
 * \brief Get the next event for the FSM.
 * 
 * \param currentState The current state of the FSM.
 * \param currentTime The current time.
 * \return The next event for the FSM.
 */
high_beams_event_t get_next_event_high_beam(high_beams_state_t currentState, time_t currentTime);

/**
 * \brief Execute the main FSM logic and determine the next state.
 * 
 * \param currentState The current state of the FSM.
 * \return The next state of the FSM.
 */
high_beams_state_t main_fsm_high_beams(high_beams_state_t currentState);

#endif /* FSM_HIGH_BEAMS_H */