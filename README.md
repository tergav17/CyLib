# CyLib
This repository includes a number of libraries which allow for enhanced functionality for the ISU CyBot platform. These libraries involve movement, sensing, and communication tasks which extend the normal topics taught in the CPRE 288 course. Some of these libraries may be dependent on other libraries to work, or may be able to act independently. The libraries included are:

- uart.h: Includes basic UART functionality, not much different that the ISU version uses. Needs no dependencies
- cyrial.h: Allows for terminal / GUI interfacing through the Cyrial Commander. Dependent on uart.h
- imu.h: Provides I2C functionality, IMU interfacing, and gyro data access. Dependent on Timer.h
- cymove.h: Provides advanced movement functions, including absolute turning and bumper driving. Dependent on imu.h and open_interface.h
