#include <stdio.h>
#include "common/hello.h"
#include "controller/door_controller.h"

int main(void)
{
    printf("%s\n", hello_message());
    
    door_controller_event(VALID_ACCESS);
    door_controller_event(TIMEOUT);

    return 0;
}
