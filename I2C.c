#include<stdio.h>
#include<regex.h>
#include<fcntl.h>
#include<unistd.h>
#include<bcm2835.h>
#include"I2C.h"

#define BUFFERSIZE 4096

extern int i2cInit(int address) {
    // Initialize the GPIO headers
    if (!bcm2835_init()) {
        printf("bcm2835_init failed. Are you running as root??\n");
        return 1;
    }

    // Prepare the hat for i2c communication
    if (!bcm2835_i2c_begin()) {
        printf("bcm2835_i2c_begin failed. Are you running as root??\n");
        return 1;
    }

    // Set the address of the hat you are communicating with
    bcm2835_i2c_setSlaveAddress(address);
    return 0;
}

extern void write8(unsigned char reg, unsigned char value) {
    // Create a char buffer instead of unsigned char
    char buff[2];
    buff[0] = (char)reg;
    buff[1] = (char)value;

    // Write to the I2C bus and check the return value
    uint8_t result = bcm2835_i2c_write(buff, 2);

    // Error handling based on BCM2835 reason codes
    switch(result) {
        case BCM2835_I2C_REASON_OK:
            // Write successful, no action needed
            break;
        case BCM2835_I2C_REASON_ERROR_NACK:
            printf("Write Error: No acknowledgment received (NACK)\n");
            break;
        case BCM2835_I2C_REASON_ERROR_CLKT:
            printf("Write Error: Clock stretch timeout\n");
            break;
        case BCM2835_I2C_REASON_ERROR_DATA:
            printf("Write Error: Data transfer error\n");
            break;
        default:
            printf("Write Error: Unknown error code: %d\n", result);
            break;
}
}

unsigned int readU8(unsigned char reg) {
    // Create a char buffer instead of unsigned char
    char buff[2];
    buff[0] = (char)reg;

    // First write the register we want to read from
    uint8_t write_result = bcm2835_i2c_write(buff, 1);
    if (write_result != BCM2835_I2C_REASON_OK) {
        printf("Error writing register address for read operation\n");
        return 0;
    }

    // Then read from the register
    uint8_t read_result = bcm2835_i2c_read(buff, 1);

    // Error handling for read operation
    switch(read_result) {
        case BCM2835_I2C_REASON_OK:
            return (unsigned int)(unsigned char)buff[0];  // Proper casting of read value

        case BCM2835_I2C_REASON_ERROR_NACK:
            printf("Read Error: No acknowledgment received (NACK)\n");
            break;

        case BCM2835_I2C_REASON_ERROR_CLKT:
            printf("Read Error: Clock stretch timeout\n");
            break;

        case BCM2835_I2C_REASON_ERROR_DATA:
            printf("Read Error: Data transfer error\n");
            break;

        default:
            printf("Read Error: Unknown error code: %d\n", read_result);
            break;
    }

    return 0;  // Return 0 on any error
}