#ifndef LOCKD_DOOR_CONTROLLER_H
#define LOCKD_DOOR_CONTROLLER_H

// TODO: keep?
// A state is needed if I want to have access to the status of the door
// or if I want to avoid locking/unlocking repeatedly when multiple events happen
/* typedef enum {
    LOCKED,
    UNLOCKED
} State;
*/

typedef enum {
    VALID_ACCESS,
    INVALID_ACCESS,
    LOCK_REQUEST // Used for both timeout events when the door has been opened for X seconds or as a "lock" event from the POST /lock endpoint
} Event;

void door_controller_event(Event);

#endif // LOCKD_DOOR_CONTROLLER_H