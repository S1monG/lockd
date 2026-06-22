#include "door_controller.h"
#include "hal/door_lock.h"

// static State state = LOCKED;

void door_controller_event(Event event) 
{
    switch (event)
    {
    case VALID_ACCESS:
        // TODO: LOG
        door_lock_unlock();
        break;

    case INVALID_ACCESS:
        // TODO: LOG
        break;

    case LOCK_REQUEST:
        // TODO: LOG
        door_lock_lock();
        break;
    
    default:
        break;
    }
}