/**
 * \file fsm_high_beams.h
 * \brief Header file for the Finite State Machine controlling the high beams.
 * \details Defines states, events, and function prototypes for the high beams FSM.
 * \author Samy Belbouab
 */

#ifndef FSM_HIGH_BEAMS_H
#define FSM_HIGH_BEAMS_H

#include <stdint.h>
#include <time.h>

/* States */
/**
 * \brief Enumeration of FSM states for the high beams.
 */
typedef enum {
    ST_ANY_high_beams = -1, /**< Any state. */
    ST_INIT_high_beams = 0, /**< Initialization state. */
    ST_OFF_high_beams = 1, /**< High beams off. */
    ST_ON_high_beams = 2, /**< High beams on. */
    ST_ERROR_high_beams = 3, /**< Error state. */
    ST_ACQUITTED_high_beams = 4, /**< Acquitted state. */
    ST_TERM_high_beams = 255 /**< Final state. */
} high_beams_state_t;

/* Events */
/**
 * \brief Enumeration of FSM events for the high beams.
 */
typedef enum {
    EV_ANY_high_beams = -1, /**< Any event. */
    EV_NONE_high_beams = 0, /**< No event. */
    EV_CMD0_high_beams = 1, /**< Command = 0. */
    EV_CMD1_high_beams = 2, /**< Command = 1. */
    EV_ACQ_RECEIVED_high_beams = 3, /**< Acquittement received. */
    EV_ACQ_TIMEOUT_high_beams = 4, /**< Acquittement timeout. */
    EV_ERR_high_beams = 255 /**< Error event. */
} high_beams_event_t;

/**
 * \brief Get the next event for the FSM.
 * \details Evaluates the current state and time to determine the next event.
 * \param currentState : The current state of the FSM.
 * \param currentTime : The current time.
 * \return high_beams_event_t : The next event for the FSM.
 */
high_beams_event_t get_next_event_high_beam(high_beams_state_t currentState, time_t currentTime);

/**
 * \brief Execute the main FSM logic and determine the next state.
 * \details Processes state transitions based on the current state and events.
 * \param currentState : The current state of the FSM.
 * \return high_beams_state_t : The next state of the FSM.
 */
high_beams_state_t main_fsm_high_beams(high_beams_state_t currentState);

#endif /* FSM_HIGH_BEAMS_H */