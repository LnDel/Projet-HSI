/**
 * \file        fsm_windshield.h
 * \brief       Header file for the Finite State Machine managing the windshield wiper and washer.
 * \details     Defines states, events, transition structures, and function prototypes for the windshield FSM.
 * \author      Warren Anderson, Samy Belbouab
 * \version     1.0
 * \date        08 Jan 2025
 */

#ifndef FSM_WINDSHIELD_H
#define FSM_WINDSHIELD_H

#include <stdint.h>
#include <time.h>

/**
 * \enum windshield_state_t
 * \brief Enumeration of the windshield FSM states.
 */
typedef enum {
    ST_ANY_WINDSHIELD = -1,                  /**< Any state */
    ST_INIT_WINDSHIELD = 0,                  /**< Initial state */
    ST_ALLOFF_WINDSHIELD = 1,                /**< All windshield functions off */
    ST_WINDSHIELDWIPER_ON_WINDSHIELD = 2,    /**< Wiper activated */
    ST_WIPER_AND_WASHER_ON_WINDSHIELD = 3,   /**< Wiper and washer activated */
    ST_TIMERWIPER_AND_WASHEROFF_WINDSHIELD = 4, /**< Timer-controlled wiper and washer off */
    ST_TERM_WINDSHIELD = 255                 /**< Terminal state */
} windshield_state_t;

/**
 * \enum windshield_event_t
 * \brief Enumeration of the windshield FSM events.
 */
typedef enum {
    EV_ANY_WINDSHIELD = -1,                  /**< Any event */
    EV_NONE_WINDSHIELD = 0,                  /**< No event */
    EV_CMD_WI0_WINDSHIELD = 1,               /**< Command: Wiper off */
    EV_CMD_WA0_WINDSHIELD = 2,               /**< Command: Washer off */
    EV_CMD_WI1_WINDSHIELD = 3,               /**< Command: Wiper on */
    EV_CMD_WA1_WINDSHIELD = 4,               /**< Command: Washer on */
    EV_TIME_UNDER_2_WINDSHIELD = 5,          /**< Timer under 2 seconds */
    EV_TIME_OVER_2_WINDSHIELD = 6,           /**< Timer over 2 seconds */
    EV_ERR_WINDSHIELD = 255                  /**< Error event */
} windshield_event_t;

/**
 * \struct windshield_transition_t
 * \brief Structure defining a state transition.
 * \details Contains the current state, the event triggering the transition,
 *          the callback function to execute, and the next state.
 */
typedef struct {
    windshield_state_t state;                /**< Current state */
    windshield_event_t event;                /**< Event triggering the transition */
    int (*callback)(void);                   /**< Callback function to execute */
    int next_state;                          /**< Next state */
} windshield_transition_t;

/**
 * \brief Determines the next event based on the current state and time.
 * \details Evaluates the current FSM state, commands, and timer to determine the next event.
 * \param current_state The current state of the FSM.
 * \param currentTimeSeconds The current time in seconds.
 * \return windshield_event_t The next event for the FSM.
 */
windshield_event_t get_next_event_windshield(windshield_state_t current_state, unsigned long currentTimeSeconds);

/**
 * \brief Main function for the windshield FSM.
 * \details Processes transitions based on the current state and events, executing callback functions as needed.
 * \param currentState The current state of the FSM.
 * \return windshield_state_t The new state of the FSM after processing.
 */
windshield_state_t main_fsm_windshield(windshield_state_t currentState);

#endif /* FSM_WINDSHIELD_H */
