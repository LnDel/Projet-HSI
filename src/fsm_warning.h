#ifndef FSM_WARNING_H
#define FSM_WARNING_H

#include <stdint.h>
#include <time.h>

/* States */
typedef enum {
    ST_ANY_WARNING = -1,
    ST_INIT_WARNING = 0,
    ST_OFF_WARNING = 1,
    ST_ACTIVATED_AND_ON_WARNING = 2,
    ST_ACQUITTED_WARNING = 3,
    ST_ACTIVATED_AND_OFF_WARNING = 4,
    ST_ERROR_WARNING = 5,
    ST_TERM_WARNING = 255
} warning_state_t;

/* Events */
typedef enum {
    EV_ANY_WARNING = -1,
    EV_NONE_WARNING = 0,
    EV_CMD0_WARNING = 1,
    EV_CMD1_WARNING = 2,
    EV_ACQ_RECEIVED_WARNING = 3,
    EV_ACQ_TIMEOUT_WARNING = 4,
    EV_TIME1_WARNING = 5,
    EV_ERR_WARNING = 255
} warning_event_t;

/* Transition structure */
typedef struct {
    warning_state_t state;
    warning_event_t event;
    int (*callback)(void);
    int next_state;
} warning_transition;

/* Function prototypes */
warning_event_t get_next_event_warningsignal(warning_state_t currentState, unsigned long currentTimeSeconds);
warning_state_t main_fsm_warningsignal(warning_state_t currentState);

#endif /* FSM_WARNING_H */
