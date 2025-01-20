#include "Robot.h"
#include <stdio.h>
#include <unistd.h>  // For sleep function
#include "MotorHat.h"  // Assuming this has motor control functions

// Robot motor trim offsets
static int8_t left_trim = 0;
static int8_t right_trim = 0;

// Motor IDs (for left and right motors)
static uint8_t left_motor_id = LEFT_MOTOR_ID;
static uint8_t right_motor_id = RIGHT_MOTOR_ID;

// Function to initialize the robot
void robotInit(uint8_t left_id, uint8_t right_id, int8_t left_trim_offset, int8_t right_trim_offset) {
    // Initialize motor hat and motors
    initMotors();  // Assumes this function sets up the motors

    // Store motor IDs and trim values
    left_motor_id = left_id;
    right_motor_id = right_id;
    left_trim = left_trim_offset;
    right_trim = right_trim_offset;

    // Ensure motors are stopped initially
    stop();
    printf("Robot initialized\n");
}

// Function to set the speed of the motor
void setSpeed(int motor_id, int speed) {
    // Assuming you have a way to access your motor control pins or PWM channels
    if (motor_id == 1) {
        // Set speed for left motor
        // For example, use a PWM library or GPIO to control the motor
        // pwmWrite(left_motor_pwm_channel, speed);
    } else if (motor_id == 2) {
        // Set speed for right motor
        // pwmWrite(right_motor_pwm_channel, speed);
    }
}

// Function to stop all motors
void stop(void) {
    // Call to the motor hat library to stop motors
    run(RELEASE, left_motor_id);  // Stop left motor
    run(RELEASE, right_motor_id); // Stop right motor
    printf("Robot stopped\n");
}

// Function to move the robot forward
void forward(uint8_t speed, uint16_t seconds) {
    // Apply speed trim offsets

uint8_t left_speed = speed + left_trim;
    uint8_t right_speed = speed + right_trim;

    // Ensure speed is within valid range (0-255)
    left_speed = (left_speed > 255) ? 255 : (left_speed < 0) ? 0 : left_speed;
    right_speed = (right_speed > 255) ? 255 : (right_speed < 0) ? 0 : right_speed;

    // Set motor speed and move forward
    setSpeed(left_motor_id, left_speed);
    setSpeed(right_motor_id, right_speed);
    run(FORWARD, left_motor_id);  // Move left motor forward
    run(FORWARD, right_motor_id); // Move right motor forward

    printf("Moving forward at speed %d\n", speed);

    // If seconds is provided, stop after that duration
    if (seconds > 0) {
        sleep(seconds);  // Sleep for 'seconds' time
        stop();
    }
}

// Function to move the robot backward
void backward(uint8_t speed, uint16_t seconds) {
    // Apply speed trim offsets
    uint8_t left_speed = speed + left_trim;
    uint8_t right_speed = speed + right_trim;

    // Ensure speed is within valid range (0-255)
    left_speed = (left_speed > 255) ? 255 : (left_speed < 0) ? 0 : left_speed;
    right_speed = (right_speed > 255) ? 255 : (right_speed < 0) ? 0 : right_speed;

    // Set motor speed and move backward
    setSpeed(left_motor_id, left_speed);
    setSpeed(right_motor_id, right_speed);
    run(BACKWARD, left_motor_id);  // Move left motor backward
    run(BACKWARD, right_motor_id); // Move right motor backward

    printf("Moving backward at speed %d\n", speed);

    // If seconds is provided, stop after that duration
    if (seconds > 0) {
        sleep(seconds);  // Sleep for 'seconds' time
        stop();
    }
}

// Function to turn the robot left
void left(uint8_t speed, uint16_t seconds) {
    // Apply speed trim offsets
    uint8_t left_speed = speed + left_trim;
 uint8_t right_speed = speed + right_trim;

    // Ensure speed is within valid range (0-255)
    left_speed = (left_speed > 255) ? 255 : (left_speed < 0) ? 0 : left_speed;
    right_speed = (right_speed > 255) ? 255 : (right_speed < 0) ? 0 : right_speed;

    // Set motor speed and spin left
    setSpeed(left_motor_id, left_speed);
    setSpeed(right_motor_id, right_speed);
    run(BACKWARD, left_motor_id);  // Left motor backward
    run(FORWARD, right_motor_id);  // Right motor forward

    printf("Turning left at speed %d\n", speed);

    // If seconds is provided, stop after that duration
    if (seconds > 0) {
        sleep(seconds);  // Sleep for 'seconds' time
        stop();
    }
}

// Function to turn the robot right
void right(uint8_t speed, uint16_t seconds) {
    // Apply speed trim offsets
    uint8_t left_speed = speed + left_trim;
    uint8_t right_speed = speed + right_trim;

    // Ensure speed is within valid range (0-255)
    left_speed = (left_speed > 255) ? 255 : (left_speed < 0) ? 0 : left_speed;
    right_speed = (right_speed > 255) ? 255 : (right_speed < 0) ? 0 : right_speed;

    // Set motor speed and spin right
    setSpeed(left_motor_id, left_speed);
    setSpeed(right_motor_id, right_speed);
    run(FORWARD, left_motor_id);  // Left motor forward
    run(BACKWARD, right_motor_id); // Right motor backward

    printf("Turning right at speed %d\n", speed);

    // If seconds is provided, stop after that duration
    if (seconds > 0) {
        sleep(seconds);  // Sleep for 'seconds' time
        stop();
    }
}
