#include <stdio.h>
#include <bcm2835.h>
#include <math.h>
#include <unistd.h>
#include "I2C.h"
#include "PWM.h"

void PWMInit(unsigned int address) {
    printf("Initializing PWM...\n");

    // Must initialize I2C first to avoid core dump
    if (i2cInit(address) != 0) {
        printf("Failed to initialize I2C\n");
        return;
    }
    printf("I2C initialized successfully\n");

    // Set all PWM outputs to 0
    setAllPWM(0, 0);

    // Configure mode registers as per Python implementation
    write8(__MODE2, __OUTDRV);
    write8(__MODE1, __ALLCALL);

    // Wait for oscillator
    usleep(5000);

    // Wake up from sleep
    unsigned char mode1 = readU8(__MODE1);
    mode1 = mode1 & ~__SLEEP;    // Clear sleep bit
    write8(__MODE1, mode1);

    // Wait for oscillator again
    usleep(5000);

    printf("PWM initialized successfully\n");
}

void setPWMFreq(int freq) {
    // Constrain frequency
    if (freq < 1) {
        printf("Frequency out of range (min 1Hz), setting to 1Hz\n");
        freq = 1;
    }
    if (freq > 3500) {
        printf("Frequency out of range (max 3500Hz), setting to 3500Hz\n");
        freq = 3500;
    }

    // Following Python implementation exactly
    float prescaleval = 25000000.0;    // 25MHz
    prescaleval /= 4096.0;             // 12-bit
    prescaleval /= (float)freq;
 printf("Setting PWM frequency to %d Hz\n", freq);
    printf("Estimated pre-scale: %f\n", prescaleval);

    int prescale = floor(prescaleval + 0.5);
    printf("Final pre-scale: %d\n", prescale);

    // Read old mode
    unsigned char oldmode = readU8(__MODE1);

    // Sleep mode to set prescale
    unsigned char newmode = (oldmode & 0x7F) | __SLEEP;
    write8(__MODE1, newmode);        // Go to sleep
    write8(__PRESCALE, prescale);    // Set prescale
    write8(__MODE1, oldmode);        // Restore old mode

    // Wait for oscillator
    usleep(5000);

    // Restart with restart bit set
    write8(__MODE1, oldmode | __RESTART);
}

void setAllPWM(int on, int off) {
    // Ensure values are in 12-bit range
    on &= 0xFFF;
    off &= 0xFFF;

    write8(__ALL_LED_ON_L, on & 0xFF);
    write8(__ALL_LED_ON_H, on >> 8);
    write8(__ALL_LED_OFF_L, off & 0xFF);
    write8(__ALL_LED_OFF_H, off >> 8);
}

void setPWM(int channel, int on, int off) {
    if (channel < 0 || channel > 15) {
        printf("PWM channel must be between 0 and 15 inclusive\n");
        return;
    }

    // Ensure values are in 12-bit range
    on &= 0xFFF;
    off &= 0xFFF;

    // Calculate register addresses for this channel
    int chan_offset = channel * 4;
    write8(__LED0_ON_L + chan_offset, on & 0xFF);
    write8(__LED0_ON_H + chan_offset, on >> 8);
    write8(__LED0_OFF_L + chan_offset, off & 0xFF);
    write8(__LED0_OFF_H + chan_offset, off >> 8);
}