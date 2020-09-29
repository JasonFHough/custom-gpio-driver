#include <bcm2835.h>
#include <stdio.h>
#include <string.h>

// Requires the BCM2835 library

// Compile:
// gcc -o ir ir.c -l bcm2835

// Run:
// sudo ./ir read
// sudo ./ir pulse

// Combine Read Usage:
// gcc -o ir ir.c -l bcm2835 && sudo ./ir read

// Combined Pulse Usage:
// gcc -o ir ir.c -l bcm2835 && sudo ./ir pulse

// GPIO/BCM 22, Physical pin 15
#define PIN_IN RPI_GPIO_P1_15

// GPIO/BCM 18, Physical pin 12
#define PIN_OUT RPI_GPIO_P1_12

// The amount of milliseconds to delay between reads/writes (frequency)
#define FREQUENCY_DELAY 37.9

// The amount of seconds to allow for reading of IR
#define READ_SECONDS 5

// The file name to read/write to
#define READ_FILE_NAME "reads.txt"

void readIR() {
  // Set RPI PIN_IN P1-15 to be an input
  bcm2835_gpio_fsel(PIN_IN, BCM2835_GPIO_FSEL_INPT);
  // with a pulldown
  bcm2835_gpio_set_pud(PIN_IN, BCM2835_GPIO_PUD_DOWN);

  FILE *fp = fopen(READ_FILE_NAME, "w");
  if(fp == NULL) {
    printf("Couldn't open file.");
    return;
  }

  printf("%s\n", "Reading...");

  // Read for READ_SECONDS
  int timeElapsed = 0;
  int seconds = READ_SECONDS * 1000;
  while (timeElapsed < seconds)
  {
    int value = bcm2835_gpio_lev(PIN_IN);
    fputc(value + 48, fp);  // Offset value by 48 to write ASCII version of int to file
    printf("read from PIN_IN 15: %d\n", value);

    // Wait (in milliseconds)
    delay(FREQUENCY_DELAY);

    // Increase timeElapsed
    if(FREQUENCY_DELAY <= 0) {
      timeElapsed += 1;
    } else {
      timeElapsed += FREQUENCY_DELAY;
    }
  }

  printf("%s\n", "Done!");

  fclose(fp);
}

void pulseIR() {
  FILE *fp = fopen(READ_FILE_NAME, "r");
  if(fp == NULL) {
    printf("Couldn't open file.");
    return;
  }

  // Set PIN_OUT to be an output GPIO, and start out with a LOW value
  bcm2835_gpio_fsel(PIN_OUT, BCM2835_GPIO_FSEL_OUTP);
  bcm2835_gpio_write(PIN_OUT, LOW);

  printf("%s\n", "Pulsing...");

  // Read file char by char until EOF, -1, or until specified seconds length is exceeded
  int timeElapsed = 0;
  int seconds = READ_SECONDS * 1000;
  int rawChar;
  while((rawChar = fgetc(fp)) != EOF && rawChar != -1 && timeElapsed < seconds) {
    char c = (char)rawChar;
    // printf("%c\n", c);
    // printf("%d\n", rawChar);

    // IR sensor's reads are inverted: 0 = on | 1 = off
    if(c == '1') {
      bcm2835_gpio_write(PIN_OUT, LOW);
    } else if (c == '0') {
      bcm2835_gpio_write(PIN_OUT, HIGH);
    }

    // Wait (in milliseconds)
    delay(FREQUENCY_DELAY);

    // Increase timeElapsed
    if(FREQUENCY_DELAY <= 0) {
      timeElapsed += 1;
    } else {
      timeElapsed += FREQUENCY_DELAY;
    }
  }

  // End with PIN_OUT being set to LOW
  bcm2835_gpio_write(PIN_OUT, LOW);

  printf("%s\n", "Done!");

  fclose(fp);
}

int main(int argc, char **argv) {
  // bcm2835_set_debug(1);

  if (!bcm2835_init()) {
    return 1;
  }

  if(argc == 2) {
    char * argument = argv[1];
    char * read = "read";
    char * pulse = "pulse";
    if(strcmp(argument, read) == 0) {
      readIR();
    } else if(strcmp(argument, pulse) == 0) {
      pulseIR();
    } else {
      printf("%s\n", "Not a valid argument. Use either 'read' or 'pulse'");
    }
  } else if(argc > 2) {
    printf("%s\n", "Use only 1 argument: either 'read' or 'pulse'");
  } else {
    printf("%s\n", "Define what to do. Use a single argument 'read' or 'pulse'");
  }

  bcm2835_close();
  return 0;
}
