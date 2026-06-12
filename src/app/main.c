#include <stdio.h>
#include "common/hello.h"
#include "hal/door_lock.h"

int main(void)
{
    printf("%s\n", hello_message());
    
    door_lock_lock();
    door_lock_unlock();

    return 0;
}
