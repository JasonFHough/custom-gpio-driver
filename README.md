# Custom GPIO Driver

A custom driver to utilize the Raspberry Pi's GPIO.

> _**NOTE:**_ This project is currently a work in progress

The driver is tested and developed on a Raspberry Pi Zero W.

## Usage

To compile and run a file that includes the `driver.h` file, use `gcc`

```bash
gcc the_file.c driver.c -o main
./main
```

### Example Usage

To compile the `main.c` file included in this repo, use the following

```bash
make
./main
```

or

```bash
gcc main.c driver.c -o main
./main
```
