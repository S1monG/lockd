// Hardware Abstraction Layer to separate the hardware-specific driver from the application.
// This layer exposes door-lock operations without requiring callers to know how its performed.
// My "door-lock" is simulated by changing LEDs, but this can easily be changed if the right 
// drivers are present.

#ifndef LOCKD_DOOR_LOCK_H
#define LOCKD_DOOR_LOCK_H

void door_lock_lock();
void door_lock_unlock();

#endif // LOCKD_DOOR_LOCK_H