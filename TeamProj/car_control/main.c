#include "car_control.h"
#include <stdio.h>

int main() {
    setup();

    //run_straight(3750);
    slow_detect();
    printf("turn left");
    car_turn_back();

    car_stop();
    return 0;
}
