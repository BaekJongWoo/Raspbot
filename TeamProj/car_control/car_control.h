#ifndef CAR_CONTROL_H
#define CAR_CONTROL_H

#include <wiringPi.h>
#include <wiringPiI2C.h>

#define PIN_L2 2
#define PIN_L1 3
#define PIN_R1 0
#define PIN_R2 7

#define DEVICE_ADDR 0x16

typedef enum{
    turn_left,
    turn_right,
    turn_back,
    go_straight,
} Turn;

typedef enum{
    up,
    right,
    down,
    left,
    
    unknown
} Direction;

extern int fd;
extern int turn_direction;

int setup();
int write_i2c_block_data(int reg, unsigned char* data, int length);
int ctrl_car(int l_dir, int l_speed, int r_dir, int r_speed);
int control_servo(int id, int angle);
int car_run(int speed1, int speed2);
int car_stop();
void run_straight(int msec);
int slow_detect();
void car_turn_right();
void car_turn_left();
void car_turn_back();
void car_move_little();

#endif
