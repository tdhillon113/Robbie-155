#include "MotorHat.h"
#include <stdlib.h>
#include <bcm2835.h>
#include "PWM.h"
// #include "MotorHat.h"

// Initialize all motors with their pin configurations
void initMotors(void) {
    // Motor 0
    motors[0].pwm = 8;
    motors[0].in2 = 9;
    motors[0].in1 = 10;

    // Motor 1
    motors[1].pwm = 13;
    motors[1].in2 = 12;
    motors[1].in1 = 11;

    // Motor 2
    motors[2].pwm = 2;
    motors[2].in2 = 3;
    motors[2].in1 = 4;

    // Motor 3
    motors[3].pwm = 7;
    motors[3].in2 = 6;
    motors[3].in1 = 5;
}

// Initialize the motor hat
void initHat(void) {
    // Initialize PWM first, as specified
    PWMInit(0x60);  // Standard address for motor HAT

    // Initialize motor pin configurations
    initMotors();

    // Set all motors to stopped state initially
    for (int i = 0; i < 4; i++) {
        run(RELEASE, i);
    }
}

// Set a pin to a specific value
void setPin(unsigned char pin, unsigned char value) {
    if (value == 1) {
        setPWM(pin, 4096, 0);    // Full on
    } else {
        setPWM(pin, 0, 0);       // Off
    }
}

// Run a motor in a specific direction
void run(unsigned char command, unsigned char motorID) {
    if (motorID > 3) {
        printf("Motor must be between 0 and 3 inclusive\n");
        return;
    }

    switch (command) {
        case FORWARD:
            setPin(motors[motorID].in2, 0);
            setPin(motors[motorID].in1, 1);
            break;

        case BACKWARD:
            setPin(motors[motorID].in1, 0);
            setPin(motors[motorID].in2, 1);
            break;

        case RELEASE:
            setPin(motors[motorID].in1, 0);
            setPin(motors[motorID].in2, 0);
            break;

        case BRAKE:
            setPin(motors[motorID].in1, 1);
            setPin(motors[motorID].in2, 1);
            break;

        default:
            printf("Invalid command\n");
            break;
    }
}