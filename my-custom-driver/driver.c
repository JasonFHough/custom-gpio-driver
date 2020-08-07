/*
 NOTE: Lots of implementation influence taken from the BCM2835 library

 See https://www.iot-programmer.com/index.php/books/22-raspberry-pi-and-the-iot-in-c/chapters-raspberry-pi-and-the-iot-in-c/59-raspberry-pi-and-the-iot-in-c-memory-mapped-gpio?showall=1
 for some more information regarding RPi GPIO theory
 */

#include "driver.h"

volatile uint32_t* bcm2835_gpio = (uint32_t *)MAP_FAILED;

/* Map 'size' bytes starting at 'off' in file 'fd' to memory.
// Return mapped address on success, MAP_FAILED otherwise.
// On error print message.
*/
static void *mapmem(const char *msg, size_t size, int fd, off_t off) {
    void *map = mmap(NULL, size, (PROT_READ | PROT_WRITE), MAP_SHARED, fd, off);
    if (map == MAP_FAILED)
    fprintf(stderr, "bcm2835_init: %s mmap failed: %s\n", msg, strerror(errno));
    return map;
}

static void unmapmem(void **pmem, size_t size) {
    if (*pmem == MAP_FAILED) return;
    munmap(*pmem, size);
    *pmem = MAP_FAILED;
}

void gpioSetup() {
    uint32_t* bcm2835_peripherals = (uint32_t*)BCM2835_PERIPHERALS_BASE;
    off_t bcm2835_peripherals_base = 0;
    size_t bcm2835_peripherals_size = BCM2835_PERIPHERALS_SIZE;
    bcm2835_gpio = bcm2835_peripherals + BCM2835_GPIO_BASE/4;

    int memfd;
    FILE *fp;

    // Map the peripherals block
    memfd = -1;
    if ((memfd = open("/dev/gpiomem", O_RDWR | O_SYNC) ) < 0)
    {
        fprintf(stderr, "bcm2835_init: Unable to open /dev/gpiomem: %s\n",
        strerror(errno));
    }

    // Base of the peripherals block is mapped to VM
    bcm2835_peripherals = mapmem("gpio", bcm2835_peripherals_size, memfd, bcm2835_peripherals_base);

    bcm2835_gpio = bcm2835_peripherals;
}

void setGPIOPin(uint8_t pinNum) {
    // Set pin ON
    volatile uint32_t* paddr = bcm2835_gpio + BCM2835_GPSET0/4 + pinNum/32;
    uint8_t shift = pinNum % 32;

    __sync_synchronize();
    *paddr = 1 << shift;
    __sync_synchronize();
}

void clrGPIOPin(uint8_t pinNum) {
    // Set pin OFF
    volatile uint32_t* paddr = bcm2835_gpio + BCM2835_GPCLR0/4 + pinNum/32;
    uint8_t shift = pinNum % 32;

    __sync_synchronize();
    *paddr = 1 << shift;
    __sync_synchronize();
}
