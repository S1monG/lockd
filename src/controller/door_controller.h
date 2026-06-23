#ifndef LOCKD_DOOR_CONTROLLER_H
#define LOCKD_DOOR_CONTROLLER_H

// TODO: configure
#define DOOR_OPEN_TIMEOUT_MS 10000 // Time in milliseconds after which the door should automatically lock again if left open

typedef enum {
    LOCKED,
    UNLOCKED
} State;

typedef enum {
    VALID_ACCESS,
    INVALID_ACCESS,
    LOCK_REQUEST,
    TIMEOUT // Used for timeout events when the door has been opened for X seconds. Should only ever be triggered by the timeout thread
} Event;

void door_controller_init(void);
void door_controller_stop(void);
void door_controller_event(Event);
State door_controller_state(void);

#endif // LOCKD_DOOR_CONTROLLER_H