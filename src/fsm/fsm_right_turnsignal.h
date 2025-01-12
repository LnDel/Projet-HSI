#ifndef FSM_RIGHT_TURN_SIGNAL_H
#define FSM_RIGHT_TURN_SIGNAL_H

#include <stdint.h>
#include <time.h>

/* States */
typedef enum {
    ST_ANY_RIGHT_TURNSIGNAL = -1,
    ST_INIT_RIGHT_TURNSIGNAL = 0,
    ST_OFF_RIGHT_TURNSIGNAL = 1,
    ST_ACTIVATED_AND_ON_RIGHT_TURNSIGNAL = 2,
    ST_ACQUITTED_RIGHT_TURNSIGNAL = 3,
    ST_ACTIVATED_AND_OFF_RIGHT_TURNSIGNAL = 4,
    ST_ACQUITTED_RIGHT_TURNSIGNAL_OFF = 5,
    ST_ERROR_RIGHT_TURNSIGNAL = 6,
    ST_TERM_RIGHT_TURNSIGNAL = 255
} right_turn_signal_state_t;

/* Events */
typedef enum {
    EV_ANY_RIGHT_TURNSIGNAL = -1,
    EV_NONE_RIGHT_TURNSIGNAL = 0,
    EV_CMD0_RIGHT_TURNSIGNAL = 1,
    EV_CMD1_RIGHT_TURNSIGNAL = 2,
    EV_ACQ_RECEIVED_RIGHT_TURNSIGNAL = 3,
    EV_ACQ_TIMEOUT_RIGHT_TURNSIGNAL = 4,
    EV_TIME1_RIGHT_TURNSIGNAL = 5,
    EV_ERR_RIGHT_TURNSIGNAL = 255
} right_turn_signal_event_t;

/* Transition structure */
typedef struct {
    right_turn_signal_state_t state;
    right_turn_signal_event_t event;
    int (*callback)(void);
    int next_state;
} right_turnsignal_transition_t;

/* Function prototypes */
right_turn_signal_event_t get_next_event_right_turnsignal(right_turn_signal_state_t currentState, unsigned long currentTimeSeconds);
right_turn_signal_state_t main_fsm_right_turnsignal(right_turn_signal_state_t currentState);

#endif /* FSM_RIGHT_TURN_SIGNAL_H */
