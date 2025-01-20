#ifndef ROBOT_H
#define ROBOT_H

#include <stdint.h>  // For uint8_t, uint16_t types

// Motor IDs (left and right motors)
#define LEFT_MOTOR_ID 1
#define RIGHT_MOTOR_ID 2

// Motor directions
#define FORWARD 1
#define BACKWARD 2
//#define RELEASE 0

// Function prototypes
void setSpeed(int motor_id, int speed);
void robotInit(uint8_t left_id, uint8_t right_id, int8_t left_trim, int8_t right_trim);
void stop(void);
void forward(uint8_t speed, uint16_t seconds);
void backward(uint8_t speed, uint16_t seconds);
void left(uint8_t speed, uint16_t seconds);
void right(uint8_t speed, uint16_t seconds);

#endif // ROBOT_H

             