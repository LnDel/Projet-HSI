/**
 * \file fsm_low_beams.h
 * \brief Header file for the Finite State Machine controlling the low beams.
 * \details Defines states, events, and functions for the low beams FSM.
 * \author Samy Belbouab
 */

#ifndef FSM_LOW_BEAMS_H
#define FSM_LOW_BEAMS_H

#include <stdint.h>
#include <time.h>

/**
 * \enum low_beams_state_t
 * \brief Enumeration of states for the low beams FSM.
 */
typedef enum {
    ST_ANY_low_beams = -1,                            /**< Any state */
    ST_INIT_low_beams = 0,                            /**< Init state */
    ST_OFF_low_beams = 1,                             /**< Off state */
    ST_ON_low_beams = 2,                              /**< On state */
    ST_ERROR_low_beams = 3,                           /**< Error state */
    ST_ACQUITTED_low_beams = 4,                       /**< Acquitted state */
    ST_TERM_low_beams = 255                           /**< Final state */
} low_beams_state_t;

/**
 * \enum low_beams_event_t
 * \brief Enumeration of events for the low beams FSM.
 */
typedef enum {
    EV_ANY_low_beams = -1,                            /**< Any event */
    EV_NONE_low_beams = 0,                            /**< No event */
    EV_CMD0_low_beams = 1,                            /**< Command: off */
    EV_CMD1_low_beams = 2,                            /**< Command: on */
    EV_ACQ_RECEIVED_low_beams = 3,                    /**< Acquittement received */
    EV_ACQ_TIMEOUT_low_beams = 4,                     /**< Acquittement timeout */
    EV_ERR_low_beams = 255                            /**< Error event */
} low_beams_event_t;

/**
 * \brief Get the next event for the FSM.
 * \details Determines the next event based on the current state and time.
 * \param currentState The current state of the FSM.
 * \param currentTime The current time.
 * \return low_beams_event_t The next event for the FSM.
 */
low_beams_event_t get_next_event_low_beams(low_beams_state_t currentState, time_t currentTime);

/**
 * \brief Execute the main FSM logic and determine the next state.
 * \details Processes transitions based on the current state and event.
 * \param currentState The current state of the FSM.
 * \return low_beams_state_t The next state of the FSM.
 */
low_beams_state_t main_fsm_low_beams(low_beams_state_t currentState);

#endif /* FSM_LOW_BEAMS_H */