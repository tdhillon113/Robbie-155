// sensor.c
#include <bcm2835.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>

#define ECHO RPI_V2_GPIO_P1_18
#define TRIG RPI_V2_GPIO_P1_16

double get_time() {
    struct timeval t;
    gettimeofday(&t, NULL);
    return t.tv_usec + t.tv_sec * 1000000.0;  // Convert to microseconds
}

void gpio_reset() {
    bcm2835_gpio_set_pud(ECHO, BCM2835_GPIO_PUD_OFF);
    bcm2835_gpio_set_pud(TRIG, BCM2835_GPIO_PUD_OFF);
    bcm2835_gpio_fsel(ECHO, BCM2835_GPIO_FSEL_INPT);  // Input for Echo
    bcm2835_gpio_fsel(TRIG, BCM2835_GPIO_FSEL_OUTP);  // Output for Trigger
}

void runSensor() {
    double pulse_start, pulse_end, pulse_duration;
    double distance;

    // Initialize the BCM2835 library
    if (!bcm2835_init()) {
        printf("BCM2835 initialization failed. Are you running as root?\n");
        return;
    }

    bcm2835_gpio_fsel(ECHO, BCM2835_GPIO_FSEL_INPT);  // Echo is input
    bcm2835_gpio_fsel(TRIG, BCM2835_GPIO_FSEL_OUTP);  // Trigger is output

    bcm2835_gpio_write(TRIG, LOW);
    printf("Waiting for sensor to settle\n");
    bcm2835_delay(2000);  // Wait for sensor to settle

    bcm2835_gpio_write(TRIG, HIGH);  // Send a pulse to TRIG
    bcm2835_delayMicroseconds(10);   // Keep HIGH for 10ms
    bcm2835_gpio_write(TRIG, LOW);   // Set TRIG back to LOW

    // Wait for Echo to go HIGH
    while (bcm2835_gpio_lev(ECHO) == 0);
    pulse_start = get_time();  // Record time when Echo goes HIGH

    // Wait for Echo to go LOW
    while (bcm2835_gpio_lev(ECHO) == 1);
    pulse_end = get_time();  // Record time when Echo goes LOW

    // Calculate the pulse duration
pulse_duration = pulse_end - pulse_start;

    // Calculate distance (speed of sound is 34300 cm/s)
    distance = (pulse_duration * 34300) / 2000000.0;  // Dividing by 2 for round-trip

    printf("Distance: %.2f cm\n", distance);

    gpio_reset();  // Reset GPIO pins
    bcm2835_close();  // Cleanup BCM2835
}
