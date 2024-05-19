#include <stdio.h>
#include <stdlib.h>
#include <wiringPi.h>
#include <wiringPiI2C.h>

#define Tracking_Left1 2    // Define the GPIO pins
#define Tracking_Left2 3
#define Tracking_Right1 0
#define Tracking_Right2 7

#define DEVICE_ADDR 0x16

int fd;

int write_i2c_block_data(int reg, unsigned char* data, int length) {
    unsigned char buf[length + 1];
    buf[0] = reg; // 시작 레지스터
    for (int i = 0; i < length; i++) {
        buf[i + 1] = data[i]; // 데이터 블록
    }
    if (write(fd, buf, length + 1) != length + 1) {
        printf("Failed to write to the i2c bus.\n");
        return -1;
    }
    return 0;
}

int _ctrl_car(int l_dir, int l_speed, int r_dir, int r_speed) {
    int reg = 0x01;
    unsigned char data[4] = {l_dir, l_speed, r_dir, r_speed};
    return write_i2c_block_data(DEVICE_ADDR, reg, data, 4);
}

int control_servo(int id, int angle){
    int reg = 0x03;
    if (angle < 0) angle = 0;
    if (angle > 180) angle = 180;
    unsigned int data[2] = {id, angle};
    return write_i2c_block_data(reg, data, 2);
}

int control_car(int speed1, int speed2) {
    int dir1 = (speed1 < 0) ? 0 : 1;
    int dir2 = (speed2 < 0) ? 0 : 1;

    return _ctrl_car(car, dir1, abs(speed1), dir2, abs(speed2));
}

void car_spin_right(int speed1, int speed2){
}

void car_spin_left(int speed1, int speed2){
}

void car_run(int speed1, int speed2){
}

void car_stop(){
}

void tracking_function() {
    int Tracking_Left1Value = digitalRead(Tracking_Left1);
    int Tracking_Left2Value = digitalRead(Tracking_Left2);
    int Tracking_Right1Value = digitalRead(Tracking_Right1);
    int Tracking_Right2Value = digitalRead(Tracking_Right2);

    if ((Tracking_Left1Value == LOW || Tracking_Left2Value == LOW) && Tracking_Right2Value == LOW) {
        car_spin_right(70, 30);
        delay(200); // sleep for 0.2 seconds
    } else if (Tracking_Left1Value == LOW && (Tracking_Right1Value == LOW || Tracking_Right2Value == LOW)) {
        car_spin_left(30, 70);
        delay(200); // sleep for 0.2 seconds
    } else if (Tracking_Left1Value == LOW) {
        car_spin_left(70, 70);
        delay(50); // sleep for 0.05 seconds
    } else if (Tracking_Right2Value == LOW) {
        car_spin_right(70, 70);
        delay(50); // sleep for 0.05 seconds
    } else if (Tracking_Left2Value == LOW && Tracking_Right1Value == HIGH) {
        car_spin_left(60, 60);
        delay(20); // sleep for 0.02 seconds
    } else if (Tracking_Left2Value == HIGH && Tracking_Right1Value == LOW) {
        car_spin_right(60, 60);
        delay(20); // sleep for 0.02 seconds
    } else if (Tracking_Left2Value == LOW && Tracking_Right1Value == LOW) {
        car_run(70, 70);
    }
}

int main() {
    if (wiringPiSetup() == -1) {
        printf("WiringPi Setup Failure");
        return 1;
    }
    
    int fd = wiringPiI2CSetup(DEVICE_ADDR);
    if(fd == -1){
        printf("I2C Setup Failure")
    }

    pinMode(Tracking_Left1, INPUT);
    pinMode(Tracking_Left2, INPUT);
    pinMode(Tracking_Right1, INPUT);
    pinMode(Tracking_Right2, INPUT);

    while (1) {
        tracking_function();
        // Add a small delay to prevent excessive CPU usage
        delay(50);
    }

    car_stop();
    printf("Ending\n");

    return 0;
}

// Placeholder functions for car control
void car_spin_right(int speed1, int speed2) {
    // Implement motor control logic for spinning right
    printf("Spinning right: %d, %d\n", speed1, speed2);
}

void car_spin_left(int speed1, int speed2) {
    // Implement motor control logic for spinning left
    printf("Spinning left: %d, %d\n", speed1, speed2);
}

void car_run(int speed1, int speed2) {
    // Implement motor control logic for running forward
    printf("Running: %d, %d\n", speed1, speed2);
}

void car_stop() {
    // Implement motor control logic for stopping the car
    printf("Stopping car\n");
}

