//
//  driver.h
//  my-custom-driver
//
//  Created by Jason Hough on 7/13/20.
//  Copyright © 2020 Jason Hough. All rights reserved.
//

#ifndef driver_h
#define driver_h

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/mman.h>

// GPIO Constants
#define BCM2835_PERIPHERALS_BASE 0x20000000     // for RPi 1
#define BCM2835_PERIPHERALS_SIZE 0x01000000     // for RPi 1
#define BCM2835_GPIO_BASE 0x200000
#define BCM2835_GPSET0 0x001c
#define BCM2835_GPCLR0 0x0028

void gpioSetup();

void setGPIOPin(uint8_t pinNum);

void clrGPIOPin(uint8_t pinNum);

#endif /* driver_h */
