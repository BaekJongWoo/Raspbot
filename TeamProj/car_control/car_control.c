#include "car_control.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <wiringPi.h>
#include <wiringPiI2C.h>

int fd;
int turn_direction = 2;

int setup(){
    if (wiringPiSetup() == -1) {
        printf("WiringPi Setup Failure\n");
        return 1;
    }
    else printf("WiringPi Setup Successed\n");

    fd = wiringPiI2CSetup(DEVICE_ADDR);
    if (fd == -1) {
        printf("I2C Setup Failure\n");
        return 1;
    }
    else printf("I2C Setup Successed\n");

    pinMode(PIN_L1, INPUT);
    pinMode(PIN_L2, INPUT);
    pinMode(PIN_R1, INPUT);
    pinMode(PIN_R2, INPUT);
}

int write_i2c_block_data(int reg, unsigned char* data, int length) {
    unsigned char buf[length + 1];
    buf[0] = reg;
    for (int i = 0; i < length; i++) {
        buf[i + 1] = data[i];
    }
    if (write(fd, buf, length + 1) != length + 1) {
        printf("Failed to write to the i2c bus\n");
        return -1;
    }
    return 0;
}

int ctrl_car(int l_dir, int l_speed, int r_dir, int r_speed) {
    int reg = 0x01;
    unsigned char data[4] = {l_dir, l_speed, r_dir, r_speed};
    return write_i2c_block_data(reg, data, 4);
}

int control_servo(int id, int angle) {
    int reg = 0x03;
    if (angle < 0) angle = 0;
    if (angle > 180) angle = 180;
    unsigned char data[2] = {id, (unsigned char)angle};
    return write_i2c_block_data(reg, data, 2);
}

int car_run(int speed1, int speed2) {
    int dir1 = (speed1 < 0) ? 0 : 1;
    int dir2 = (speed2 < 0) ? 0 : 1;

    return ctrl_car(dir1, abs(speed1), dir2, abs(speed2));
}

int car_stop() {
    int reg = 0x02;
    unsigned char stop_data[1] = {0x00};
    return write_i2c_block_data(reg, stop_data, 1);
}

void run_straight(int msec){
    int speed = 50, delta = 30;

    unsigned int start_time = millis();
    while (millis() - start_time < msec){
        int L2 = digitalRead(PIN_L2);
        int L1 = digitalRead(PIN_L1);
        int R1 = digitalRead(PIN_R1);
        int R2 = digitalRead(PIN_R2);

        if (L1 == LOW || L2 == LOW){ // 선이 왼쪽에 걸침
            // 아주 약간 좌회전
            car_run(speed - delta, speed + delta);
        }
        else if (R1 == LOW || R2 == LOW){ // 선이 오른쪽에 걸침
            // 아주 약간 우회전
            car_run(speed + delta, speed - delta);
        }
        else{ // 그 외의 경우
            //직진
        }
        delay(10);
    }    
}

int slow_detect() {
    int speed = 30, delta = 10;
    int L2, L1, R1, R2;

    while (1){
        L2 = digitalRead(PIN_L2);
        L1 = digitalRead(PIN_L1);
        R1 = digitalRead(PIN_R1);
        R2 = digitalRead(PIN_R2);

        if (L1 == LOW && R1 == LOW && (L2 == LOW || R2 == LOW)){ // 교차로 인식
            //car_stop();
            printf("S\n");
            break;
        }

        else if (L1 == LOW || L2 == LOW){ // 선이 왼쪽에 걸침
            // 아주 약간 좌회전
            car_run(speed - delta, speed + delta);
        }
        else if (R1 == LOW || R2 == LOW){ // 선이 오른쪽에 걸침
            // 아주 약간 우회전
            car_run(speed + delta, speed - delta);
        }
        else{ // 그 외의 경우
            //직진
            car_run(speed, speed);
        }
        delay(10);
    }    
}

void car_turn_right(){
    car_run(30, 30);
    delay(300);
    car_run(75, 0);
    delay(1000);
    car_run(50, 0);

    while(1){
        int L1 = digitalRead(PIN_L1);
        int R1 = digitalRead(PIN_R1);
        
        if (L1 == LOW || R1 == LOW) break;
    }
}

void car_turn_left(){
    car_run(30, 30);
    delay(300);
    car_run(0, 75);
    delay(1000);
    car_run(0,50);

    while(1){
        int L1 = digitalRead(PIN_L1);
        int R1 = digitalRead(PIN_R1);
        
        if (L1 == LOW || R1 == LOW) break;
    }
}

void car_turn_back(){
    car_run(-60, 60);
    delay(1500);
    car_run(-50, 50);

    while(1){
        int L1 = digitalRead(PIN_L1);
        int R1 = digitalRead(PIN_R1);
        
        if (L1 == LOW || R1 == LOW) break;
    }
}

void car_move_little(){
    car_run(60,60);
    delay(1000);
}