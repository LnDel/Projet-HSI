#ifndef FSM_WINDSHIELD_H
#define FSM_WINDSHIELD_H

#include <stdint.h>
#include <time.h>

/* States */
typedef enum {
  ST_ANY_WINDSHIELD = -1,                            /* Any state */
  ST_INIT_WINDSHIELD = 0,                            /* Init state */
  ST_ALLOFF_WINDSHIELD = 1,
  ST_WINDSHIELDWIPER_ON_WINDSHIELD = 2,
  ST_WIPER_AND_WASHER_ON_WINDSHIELD = 3,
  ST_TIMERWIPER_AND_WASHEROFF_WINDSHIELD = 4,
  ST_TERM_WINDSHIELD = 255,                 /* Final state */
} windshield_state_t;

/* Events */
typedef enum {
  EV_ANY_WINDSHIELD = -1,                            /* Any event */
  EV_NONE_WINDSHIELD = 0,                            /* No event */
  EV_CMD_WI0_WINDSHIELD = 1,                         // WI == Wiper
  EV_CMD_WA0_WINDSHIELD = 2,                         // WA == Washer
  EV_CMD_WI1_WINDSHIELD = 3,
  EV_CMD_WA1_WINDSHIELD = 4,
  EV_TIME_UNDER_2_WINDSHIELD = 5,
  EV_TIME_OVER_2_WINDSHIELD = 6,
  EV_ERR_WINDSHIELD = 255                            /* Error event */
} windshield_event_t;

/* Transition structure */
typedef struct {
    windshield_state_t state;
    windshield_event_t event;
    int (*callback)(void);
    int next_state;
} tTransition;

/* Function prototypes */
windshield_event_t get_next_event_windshield(windshield_state_t current_state, long unsigned currentTimeSeconds);
windshield_state_t main_fsm_windshield(windshield_state_t currentState);

#endif /* FSM_WINDSHIELD_H */