/**
 * \file        fsm_right_turn_signal.h
 * \author      Warren Anderson
 * \brief       Definition of states, events, and structures for the right turn signal finite state machine (FSM).
 * \details     This file contains the definitions of states, events, transition structures, 
 *              as well as the main FSM function prototypes.
 */

#ifndef FSM_RIGHT_TURN_SIGNAL_H
#define FSM_RIGHT_TURN_SIGNAL_H

#include <stdint.h>
#include <time.h>

/**
 * \enum right_turn_signal_state_t
 * \brief Enumeration of states in the right turn signal FSM.
 */
typedef enum {
    ST_ANY_RIGHT_TURNSIGNAL = -1,                /**< Matches any state. */
    ST_INIT_RIGHT_TURNSIGNAL = 0,                /**< Initialization state. */
    ST_OFF_RIGHT_TURNSIGNAL = 1,                 /**< Signal is off. */
    ST_ACTIVATED_AND_ON_RIGHT_TURNSIGNAL = 2,    /**< Signal is activated and on. */
    ST_ACQUITTED_RIGHT_TURNSIGNAL = 3,           /**< Signal is acknowledged and remains on. */
    ST_ACTIVATED_AND_OFF_RIGHT_TURNSIGNAL = 4,   /**< Signal is activated but off. */
    ST_ACQUITTED_RIGHT_TURNSIGNAL_OFF = 5,       /**< Signal is acknowledged and off. */
    ST_ERROR_RIGHT_TURNSIGNAL = 6,               /**< Error state. */
    ST_TERM_RIGHT_TURNSIGNAL = 255               /**< Terminal state. */
} right_turn_signal_state_t;

/**
 * \enum right_turn_signal_event_t
 * \brief Enumeration of events that can trigger a transition in the FSM.
 */
typedef enum {
    EV_ANY_RIGHT_TURNSIGNAL = -1,        /**< Matches any event. */
    EV_NONE_RIGHT_TURNSIGNAL = 0,        /**< No event. */
    EV_CMD0_RIGHT_TURNSIGNAL = 1,        /**< Command 0 received. */
    EV_CMD1_RIGHT_TURNSIGNAL = 2,        /**< Command 1 received. */
    EV_ACQ_RECEIVED_RIGHT_TURNSIGNAL = 3,/**< Acknowledgment received. */
    EV_ACQ_TIMEOUT_RIGHT_TURNSIGNAL = 4, /**< Acknowledgment timeout. */
    EV_TIME1_RIGHT_TURNSIGNAL = 5,       /**< Timer-related event. */
    EV_ERR_RIGHT_TURNSIGNAL = 255        /**< Error event. */
} right_turn_signal_event_t;

/**
 * \struct right_turnsignal_transition_t
 * \brief Structure representing a transition in the FSM.
 */
typedef struct {
    right_turn_signal_state_t state;     /**< Current state of the FSM. */
    right_turn_signal_event_t event;     /**< Event triggering the transition. */
    int (*callback)(void);              /**< Callback function executed during the transition. */
    int next_state;                     /**< Next state after the transition. */
} right_turnsignal_transition_t;

/**
 * \brief Determines the next event for the right turn signal FSM.
 * \details Analyzes the current state and time to return the corresponding event.
 * \param currentState The current state of the FSM.
 * \param currentTimeSeconds The current time in seconds.
 * \return right_turn_signal_event_t : The next event for the FSM.
 */
right_turn_signal_event_t get_next_event_right_turnsignal(right_turn_signal_state_t currentState);

/**
 * \brief Executes the main logic of the right turn signal FSM.
 * \details Handles transitions based on the current state and events.
 * \param currentState The current state of the FSM.
 * \return right_turn_signal_state_t : The new state of the FSM.
 */
right_turn_signal_state_t main_fsm_right_turnsignal(right_turn_signal_state_t currentState);

#endif /* FSM_RIGHT_TURN_SIGNAL_H */
