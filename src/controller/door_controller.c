// Will probably work fine as is, but I think there is and edge case
// where deadlock breaks the controller. Future improvment maybe :)

#include "door_controller.h"
#include "hal/door_lock.h"

#include <stdbool.h>
#include <pthread.h>
#include <unistd.h>

// Multiple events can be triggered at the same time, ex a valid access 
// event from the HTTP server and a timeout event from the timeout thread, 
// which can cause race conditions. Mutex to protect access to the timeout 
// thread and HAL functions, since they are not assumed to be thread-safe. 
static pthread_mutex_t  event_mutex = PTHREAD_MUTEX_INITIALIZER;
static State            state;
static bool             timeout_thread_active = false;
static pthread_t        timeout_thread;

void door_controller_init() 
{
    door_lock_lock();
    state = LOCKED;
    timeout_thread_active = false;
}

void door_controller_stop() 
{
    if (timeout_thread_active) {
        pthread_cancel(timeout_thread);
        pthread_join(timeout_thread, NULL); // wait for the thread to finish
        timeout_thread_active = false;
    }
}

// If the thread recives a cancellation request, it will be cancelled at the 
// next cancellation point. usleep is a cancellation point. In the rare case 
// that the thread just began execution of door_controller_event(TIMEOUT) when 
// receiving the cancellation request, the door might be locked again immediately 
// after being unlocked. But this is a very rare case and not a big issue IMO.
void *timeout_handler(void *arg) 
{
    (void)arg; // Unused parameter

    usleep(DOOR_OPEN_TIMEOUT_MS * 1000); // Convert milliseconds to microseconds
    door_controller_event(TIMEOUT);
    return NULL;
}

void door_controller_event(Event event) 
{
    // create a cancelation point for the timeout thread
    if (event == TIMEOUT) {
        pthread_testcancel();
    }

    pthread_mutex_lock(&event_mutex);
    switch (event)
    {
    case VALID_ACCESS:
        // Start timeout thread to automatically lock the door after a certain time if left open
        // TODO: LOG
        
        if (timeout_thread_active) {
            pthread_cancel(timeout_thread); // sends a cancellation request. Returns an error code if threadID is invalid
            pthread_join(timeout_thread, NULL); // wait for the thread to finish. Returns an error code if threadID is invalid
        }
        
        door_lock_unlock();
        pthread_create(&timeout_thread, NULL, timeout_handler, NULL);
        timeout_thread_active = true;
        break;
    case INVALID_ACCESS:
        // TODO: LOG
        break;
    case LOCK_REQUEST:
        // Cancel any pending timeout thread since the door is now locked
        // TODO: LOG
        
        if (timeout_thread_active) {
            pthread_cancel(timeout_thread);
            pthread_join(timeout_thread, NULL); // wait for the thread to finish
            timeout_thread_active = false;
        }

        door_lock_lock();
        
        break;
    case TIMEOUT:
        // no need to set timeout_thread_active to false here since the thread will 
        // be joined when a new VALID_ACCESS or LOCK_REQUEST event is triggered
        // TODO: LOG
        door_lock_lock();
        break;
    default:
        break;
    }
    pthread_mutex_unlock(&event_mutex);
}

State door_controller_state()
{
    return state;
}