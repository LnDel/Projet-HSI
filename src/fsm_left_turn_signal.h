#ifndef FSM_LEFT_TURN_SIGNAL_H
#define FSM_LEFT_TURN_SIGNAL_H

#include <stdint.h>
#include <time.h>

/* States */
typedef enum {
    ST_ANY_LEFT_TURNSIGNAL = -1,
    ST_INIT_LEFT_TURNSIGNAL = 0,
    ST_OFF_LEFT_TURNSIGNAL = 1,
    ST_ACTIVATED_AND_ON_LEFT_TURNSIGNAL = 2,
    ST_ACQUITTED_LEFT_TURNSIGNAL = 3,
    ST_ACTIVATED_AND_OFF_LEFT_TURNSIGNAL = 4,
    ST_ERROR_LEFT_TURNSIGNAL = 5,
    ST_TERM_LEFT_TURNSIGNAL = 255
} left_turn_signal_state_t;

/* Events */
typedef enum {
    EV_ANY_LEFT_TURNSIGNAL = -1,
    EV_NONE_LEFT_TURNSIGNAL = 0,
    EV_CMD0_LEFT_TURNSIGNAL = 1,
    EV_CMD1_LEFT_TURNSIGNAL = 2,
    EV_ACQ_RECEIVED_LEFT_TURNSIGNAL = 3,
    EV_ACQ_TIMEOUT_LEFT_TURNSIGNAL = 4,
    EV_TIME1_LEFT_TURNSIGNAL = 5,
    EV_ERR_LEFT_TURNSIGNAL = 255
} left_turn_signal_event_t;

/* Transition structure */
typedef struct {
    left_turn_signal_state_t state;
    left_turn_signal_event_t event;
    int (*callback)(void);
    int next_state;
} left_turnsignal_transition;

/* Function prototypes */
left_turn_signal_event_t get_next_event_leftturnsignal(left_turn_signal_state_t currentState, unsigned long currentTimeSeconds);
left_turn_signal_state_t main_fsm_left_turnsignal(left_turn_signal_state_t currentState);

#endif /* FSM_LEFT_TURN_SIGNAL_H */
