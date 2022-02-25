# RasPi-UARTinC
This repo demonstates how to handle UART communication using C programming language with the pigpio library on Raspberry Pi.

## Introduction
There are lots of tutorials on how to handle UART communication of Raspberry Pi using python, but very few use C programming language. However, if performance is the priority, C may be the first choice.

## Enable UART
To enable the UART communication, run

`sudo raspi-config`

and go to Interfacing Options → Serial and select "No" when asked "Would you like a login shell to be accessible over
serial?" and "Yes" when asked "Would you like the serial port hardware to be enabled?" Choose "Yes" when leaving raspi-config and reboot your Raspberry Pi to enable the serial port.

The default UART will appear as */dev/serial0*. The related hadrware pins are:

| Pin# | Description |
| ----------- | ----------- |
| 14 | GND |
| 10 | UART_RX0 |
| 8 | UART_TX0 |

## The pigpio library
The [pigpio library](https://abyz.me.uk/rpi/pigpio/index.html) provides high performance C interface for manipulating Raspberry Pi GPIOs, and utilities for threading. To install the library, run:
```
wget https://github.com/joan2937/pigpio/archive/master.zip
unzip master.zip
cd pigpio-master
make
sudo make install
```

## Getting started
The demo application uses two threads to read and write the UART at specified interval. If you use a wire to bridge Pin 10 and 8 together, it works as a UART loop test. However, due to that the two threads are not synchronized, the reading thread may return partial data.

To compile the source code, run:

`gcc -Wall -pthread -o helloworld helloworld.c -lpigpio -lrt`

To run the application:

`sudo ./helloworld`
