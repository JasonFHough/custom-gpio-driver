//
//  main.c
//  my-custom-driver
//
//  Created by Jason Hough on 7/13/20.
//  Copyright © 2020 Jason Hough. All rights reserved.
//

#include "driver.h"
#include <unistd.h>

int main(int argc, const char * argv[]) {
    printf("Starting...\n");
    gpioSetup();
    int pin = 17;   // BCM Pin Number
    
    while (1) {
        setGPIOPin(17);
        
        sleep(.5);
        
        clrGPIOPin(17);
        
        sleep(.5);
    }
    
    return 0;
}
