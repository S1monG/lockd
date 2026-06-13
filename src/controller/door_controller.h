#ifndef LOCKD_DOOR_CONTROLLER_H
#define LOCKD_DOOR_CONTROLLER_H

/* typedef enum { // TODO: keep?
    LOCKED,
    UNLOCKED
} State;
*/

typedef enum {
    VALID_ACCESS,
    INVALID_ACCESS,
    TIMEOUT // When the door has been opened for X seconds
} Event;

void door_controller_event(Event);

#endif // LOCKD_DOOR_CONTROLLER_H