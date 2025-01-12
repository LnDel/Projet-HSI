/**
 * \file        fsm_warning.h
 * \brief       Header file for the Finite State Machine managing the warning signal.
 * \details     Defines states, events, transition structures, and function prototypes for the FSM.
 * \author      Warren Anderson, Samy Belbouab
 */

#ifndef FSM_WARNING_H
#define FSM_WARNING_H

#include <stdint.h>
#include <time.h>

/**
 * \enum warning_state_t
 * \brief Enumeration of the warning FSM states.
 */
typedef enum {
    ST_ANY_WARNING = -1,             /**< Any state */
    ST_INIT_WARNING = 0,             /**< Initial state */
    ST_OFF_WARNING = 1,              /**< Warning signal off */
    ST_ACTIVATED_AND_ON_WARNING = 2, /**< Warning signal activated and on */
    ST_ACQUITTED_WARNING = 3,        /**< Warning signal acquitted */
    ST_ACTIVATED_AND_OFF_WARNING = 4,/**< Warning signal activated and off */
    ST_ERROR_WARNING = 5,            /**< Error state */
    ST_ACQUITTED_WARNING_OFF = 6,    /**< Warning signal acquitted and off */
    ST_TERM_WARNING = 255            /**< Terminal state */
} warning_state_t;

/**
 * \enum warning_event_t
 * \brief Enumeration of the warning FSM events.
 */
typedef enum {
    EV_ANY_WARNING = -1,             /**< Any event */
    EV_NONE_WARNING = 0,             /**< No event */
    EV_CMD0_WARNING = 1,             /**< Command 0 received */
    EV_CMD1_WARNING = 2,             /**< Command 1 received */
    EV_ACQ_RECEIVED_WARNING = 3,     /**< Acknowledgment received */
    EV_ACQ_TIMEOUT_WARNING = 4,      /**< Acknowledgment timeout */
    EV_TIME1_WARNING = 5,            /**< Timer event */
    EV_ERR_WARNING = 255             /**< Error event */
} warning_event_t;

/**
 * \struct warning_transition_t
 * \brief Structure defining a state transition.
 * \details Contains the current state, the event triggering the transition,
 *          the callback function to execute, and the next state.
 */
typedef struct {
    warning_state_t state;            /**< Current state */
    warning_event_t event;            /**< Event triggering the transition */
    int (*callback)(void);            /**< Callback function to execute */
    int next_state;                   /**< Next state */
} warning_transition_t;

/**
 * \brief Determines the next event based on the current state and time.
 * \details Evaluates the current FSM state, the command, and the acknowledgment indicator to determine the next event.
 * \param currentState The current state of the FSM.
 * \param currentTimeSeconds The current time in seconds.
 * \return warning_event_t The next event for the FSM.
 */
warning_event_t get_next_event_warning(warning_state_t currentState, unsigned long currentTimeSeconds);

/**
 * \brief Main function for the warning FSM.
 * \details Processes transitions based on the current state and events, executing callback functions as needed.
 * \param currentState The current state of the FSM.
 * \return warning_state_t The new state of the FSM after processing.
 */
warning_state_t main_fsm_warning(warning_state_t currentState);

#endif /* FSM_WARNING_H */
