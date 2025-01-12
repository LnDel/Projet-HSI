/**
 * \file fsm_left_turn_signal.h
 * \brief Finite State Machine for the left turn signal.
 * \details Contains state and event definitions, along with function prototypes for managing the FSM of the left turn signal.
 * \author Warren Anderson
 */

#ifndef FSM_LEFT_TURN_SIGNAL_H
#define FSM_LEFT_TURN_SIGNAL_H

#include <stdint.h>
#include <time.h>

/**
 * \brief States for the left turn signal FSM.
 */
typedef enum {
    ST_ANY_LEFT_TURNSIGNAL = -1,            /**< Any state */
    ST_INIT_LEFT_TURNSIGNAL = 0,            /**< Initial state */
    ST_OFF_LEFT_TURNSIGNAL = 1,             /**< Turn signal off */
    ST_ACTIVATED_AND_ON_LEFT_TURNSIGNAL = 2,/**< Activated and on */
    ST_ACQUITTED_LEFT_TURNSIGNAL = 3,       /**< Acquitted */
    ST_ACTIVATED_AND_OFF_LEFT_TURNSIGNAL = 4,/**< Activated and off */
    ST_ACQUITTED_LEFT_TURNSIGNAL_OFF = 5,   /**< Acquitted and off */
    ST_ERROR_LEFT_TURNSIGNAL = 6,           /**< Error state */
    ST_TERM_LEFT_TURNSIGNAL = 255           /**< Terminal state */
} left_turn_signal_state_t;

/**
 * \brief Events for the left turn signal FSM.
 */
typedef enum {
    EV_ANY_LEFT_TURNSIGNAL = -1,            /**< Any event */
    EV_NONE_LEFT_TURNSIGNAL = 0,            /**< No event */
    EV_CMD0_LEFT_TURNSIGNAL = 1,            /**< Command 0 */
    EV_CMD1_LEFT_TURNSIGNAL = 2,            /**< Command 1 */
    EV_ACQ_RECEIVED_LEFT_TURNSIGNAL = 3,    /**< Acquittement received */
    EV_ACQ_TIMEOUT_LEFT_TURNSIGNAL = 4,     /**< Acquittement timeout */
    EV_TIME1_LEFT_TURNSIGNAL = 5,           /**< Timer event */
    EV_ERR_LEFT_TURNSIGNAL = 255            /**< Error event */
} left_turn_signal_event_t;

/**
 * \brief Transition structure for the left turn signal FSM.
 */
typedef struct {
    left_turn_signal_state_t state;          /**< Current state */
    left_turn_signal_event_t event;          /**< Triggering event */
    int (*callback)(void);                   /**< Callback function */
    int next_state;                          /**< Next state */
} left_turnsignal_transition_t;

/**
 * \brief Determine the next event for the left turn signal FSM.
 * \details Evaluates the current state and time to decide the next event.
 * \param currentState The current state of the FSM.
 * \return The next event for the FSM.
 */
left_turn_signal_event_t get_next_event_left_turnsignal(left_turn_signal_state_t currentState);

/**
 * \brief Execute the main FSM logic for the left turn signal.
 * \details Processes transitions based on the current state and event.
 * \param currentState The current state of the FSM.
 * \return The next state of the FSM.
 */
left_turn_signal_state_t main_fsm_left_turnsignal(left_turn_signal_state_t currentState);

#endif /* FSM_LEFT_TURN_SIGNAL_H */