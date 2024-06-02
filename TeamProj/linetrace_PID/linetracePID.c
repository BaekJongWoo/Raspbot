#include <stdio.h>
#include <stdlib.h>
#include <wiringPi.h>
#include <wiringPiI2C.h>
#include <unistd.h>

#define PIN_L1 2 
#define PIN_L2 3
#define PIN_R1 0
#define PIN_R2 7

#define DEVICE_ADDR 0x16

int fd;
int turn_direction = 0; // 0 for left, 1 for right

const int BASE_SPEED = 70;
const int MAX_SPEED = 100;

// PID constants
const float Kp = 1.0;
const float Ki = 0;
const float Kd = 0;

int error_sum = 0;
int last_error = 0;

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

int make_turn(int direction) {
    if (direction == 0) {
        car_run(-50, 50); // Turn left
    } else {
        car_run(50, -50); // Turn right
    }
    delay(100); // Adjust this delay for a 90-degree turn
    car_stop();
    return 0;
}

int tracking_function() {
    int L1 = digitalRead(PIN_L1);
    int L2 = digitalRead(PIN_L2);
    int R1 = digitalRead(PIN_R1);
    int R2 = digitalRead(PIN_R2);

    if ((L1 == LOW || L2 == LOW) && R2 == LOW) {
        car_run(70, -30);
        delay(200);
    }
    else if (L1 == LOW && (R1 == LOW || R2 == LOW)) {
        car_run(-30, 70);
        delay(200);
    }
    else if (L1 == LOW) {
        car_run(-70, 70);
        delay(50);
    }
    else if (R2 == LOW) {
        car_run(70, -70);
        delay(50);
    }
    else if (L2 == LOW && R1 == HIGH) {
        car_run(-60, 60);
        delay(20);
    }
    else if (L2 == HIGH && R1 == LOW) {
        car_run(60, -60);
        delay(20);          
    }
    else if (L2 == LOW && R1 == LOW) {
        int error = 0;

        // Calculate error based on sensor states
        if (L1 == LOW) error += 3;
        if (L2 == LOW) error += 1;
        if (R1 == LOW) error -= 1;
        if (R2 == LOW) error -= 3;

        // Calculate PID terms
        error_sum += error;
        int d_error = error - last_error;

        int pid_output = Kp * error + Ki * error_sum + Kd * d_error;

        // Calculate motor speeds
        int left_speed = BASE_SPEED + pid_output;
        int right_speed = BASE_SPEED - pid_output;

        // Constrain speeds to maximum limits
        if (left_speed > MAX_SPEED) left_speed = MAX_SPEED;
        if (left_speed < -MAX_SPEED) left_speed = -MAX_SPEED;
        if (right_speed > MAX_SPEED) right_speed = MAX_SPEED;
        if (right_speed < -MAX_SPEED) right_speed = -MAX_SPEED;

        car_run(left_speed, right_speed);

        last_error = error;
    }

    if (L1 == LOW && L2 == LOW && R1 == LOW && R2 == LOW) {
        car_stop();
        delay(50);
        make_turn(turn_direction);
        turn_direction = !turn_direction; // Alternate turn direction
        return 0;
    }
    return 0;
}

int main() {
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
    
    int ret = 0;
    while (ret == 0) {
        ret = tracking_function();
        delay(50);
    }

    car_stop();
    printf("Ending\n");

    return 0;
}