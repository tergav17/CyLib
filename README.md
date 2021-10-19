# CyLib
This repository includes a number of libraries which allow for enhanced functionality for the ISU CyBot platform. These libraries involve movement, sensing, and communication tasks which extend the normal topics taught in the CPRE 288 course. Some of these libraries may be dependent on other libraries to work, or may be able to act independently. The libraries included are:

- uart.h: Includes basic UART functionality, not much different that the ISU version uses. Needs no dependencies
- cyrial.h: Allows for terminal / GUI interfacing through the Cyrial Commander. Dependent on uart.h
- imu.h: Provides I2C functionality, IMU interfacing, and gyro data access. Dependent on Timer.h
- cymove.h: Provides advanced movement functions, including absolute turning and bumper driving. Dependent on imu.h and open_interface.h
# uart.h
This library includes very basic CyBot UART functionality. It is configured at 155200 baud, in order to use the WiFi module. It allows for the sending of individual characters or strings, and the recieving of characters. It also allows for interrupts, but the handler has no useful code at the moment. This UART library is minimual, as it is only really meant to provide commincation to the Cyrial Commander, thought it technically can be used with PuTTY too. In order to use the library, uart_init() needs to be called before any commands are used. If interrupts are to be used, uart_interrupt_init() must be called. The function descriptions are as follows:
```c
void uart_init(void);

/*
Initializes the UART by configuring the pins, UART settings, and enabling the UART. Needs to be called at startup for communication to work.
*/

void uart_sendChar(char data);

/*
Sends a character "data" to the UART, no error is returned.
*/

char uart_receive(void);

/*
Blocks till a character is recived by the UART, and returns it.
*/

void uart_sendStr(const char *data);

/*
Sends a null-terminated string "*data" to the UART, blocks till entire string has been sent.
*/

void uart_interrupt_init();

/*
Initialize UART interrupt functionality, when a character is recived, uart_interrupt_handler() is called.
*/

void uart_interrupt_handler();

/*
Is called when a recieve interrupt is caught from the UART.
*/
```
