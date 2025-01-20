#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include "MotorHat.h"
#include "PWM.h"
#include "I2C.h"
#include "Robot.h"

#define LEFT_TRIM 0
#define RIGHT_TRIM 0
#define LEFT_MOTOR 1
#define RIGHT_MOTOR 2

int main() {
    // Initialize the I2C bus for communication
    int address = 0x40; // Replace with the I2C address of your PWM driver
    if (i2cInit(address) != 0) {
        printf("Failed to initialize I2C.\n");
        return 1;
    }

    // Initialize the PWM driver
    PWMInit(address);
    setPWMFreq(50); // Set the frequency to 50Hz for servo/motor control

    // Initialize the Motor HAT
    initHat();
    initMotors();

    // Example usage: Run motor 0 forward
    printf("Running motor 0 forward...\n");
    setPin(motors[0].pwm, 255); // Set PWM to full speed
    setPin(motors[0].in1, 1);   // Set IN1 high
    setPin(motors[0].in2, 0);   // Set IN2 low
    run(FORWARD, 0);            // Command motor 0 to move forward

    // Wait for 2 seconds
    bcm2835_delay(2000);

    // Stop the motor
    printf("Stopping motor 0...\n");
    run(RELEASE, 0);

    // Example: Set a PWM value on channel 0
    printf("Setting PWM on channel 0...\n");
    setPWM(0, 0, 2048); // Halfway duty cycle for channel 0

    // Example: Set all PWM channels
    printf("Setting all PWM channels to low...\n");
    setAllPWM(0, 0);

    // Cleanup and exit
    bcm2835_close();
    printf("Program complete.\n");
    return 0;
}
