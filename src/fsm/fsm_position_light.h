/**
 * \file        fsm_position_light.h
 * \author      Samy Belbouab
 * \brief       Header file for the Finite State Machine controlling the position light.
 * \details
 */

#ifndef FSM_POSITION_LIGHT_H
#define FSM_POSITION_LIGHT_H

#include <stdint.h>
#include <time.h>

/* States */
typedef enum {
    ST_ANY_position_light = -1,            /* Any state */
    ST_INIT_position_light = 0,           /* Init state */
    ST_OFF_position_light = 1,            /* Off state */
    ST_ON_position_light = 2,             /* On state */
    ST_ERROR_position_light = 3,          /* Error state */
    ST_ACQUITTED_position_light = 4,      /* Acquitted state */
    ST_TERM_position_light = 255          /* Final state */
} position_light_state_t;

/* Events */
typedef enum {
    EV_ANY_position_light = -1,           /* Any event */
    EV_NONE_position_light = 0,           /* No event */
    EV_CMD0_position_light = 1,           /* Cmd = 0 */
    EV_CMD1_position_light = 2,           /* Cmd = 1 */
    EV_ACQ_RECEIVED_position_light = 3,   /* Acquittement received */
    EV_ACQ_TIMEOUT_position_light = 4,    /* Acquittement timeout */
    EV_ERR_position_light = 255           /* Error event */
} position_light_event_t;

/**
 * \brief Get the next event for the FSM.
 * 
 * \param currentState The current state of the FSM.
 * \param currentTime The current time.
 * \return The next event for the FSM.
 */
position_light_event_t get_next_event_position_light(position_light_state_t currentState, time_t currentTime);

/**
 * \brief Execute the main FSM logic and determine the next state.
 * 
 * \param currentState The current state of the FSM.
 * \return The next state of the FSM.
 */
position_light_state_t main_fsm_position_light(position_light_state_t currentState);

#endif /* FSM_POSITION_LIGHT_H */
