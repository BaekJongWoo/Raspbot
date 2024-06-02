#include <stdio.h>
#include <stdlib.h>
#include <wiringPi.h>
#include <wiringPiI2C.h>
#include <unistd.h>

#define PIN_L2 2
#define PIN_L1 3
#define PIN_R1 0
#define PIN_R2 7

#define DEVICE_ADDR 0x16

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
    int speed = 75, delta = 25;

    unsigned int start_time = millis();
    while (millis() - start_time < msec){
        int L2 = digitalRead(PIN_L2);
        int L1 = digitalRead(PIN_L1);
        int R1 = digitalRead(PIN_R1);
        int R2 = digitalRead(PIN_R2);

        if (L1 == LOW || L2 == LOW){ // 선이 왼쪽에 걸침
            // 아주 약간 좌회전
            printf("F\n");
            car_run(speed - delta, speed + delta);
        }
        else if (R1 == LOW || R2 == LOW){ // 선이 오른쪽에 걸침
            // 아주 약간 우회전
            printf("R\n");
            car_run(speed + delta, speed - delta);
        }
        else{ // 그 외의 경우
            //직진
            printf("L\n");
        }
        delay(100);
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

        if (L1 == LOW && R1 == LOW && (L2 == LOW && R2 == LOW)){ // 교차로 인식
            car_stop();
            printf("S\n");
            break;
        }

        else if (L1 == LOW || L2 == LOW){ // 선이 왼쪽에 걸침
            // 아주 약간 좌회전
            printf("L\n");
            car_run(speed - delta, speed + delta);
        }
        else if (R1 == LOW || R2 == LOW){ // 선이 오른쪽에 걸침
            // 아주 약간 우회전
            printf("R\n");
            car_run(speed + delta, speed - delta);
        }
        else{ // 그 외의 경우
            //직진
            printf("F\n");
            car_run(speed, speed);
        }
        delay(100);
    }    
}

void turn_right(){
    
    car_stop();
}

void turn_left(){
    car_run(25, 75);
    delay(500);
    car_stop();
}

int main() {
    setup();

    //run_straight(3750);
    slow_detect();
    turn_left();

    car_stop();
    return 0;
}