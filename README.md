<div align="center">
  <img src="https://github.com/user-attachments/assets/3808f84a-4d3c-40c2-98d6-4721f3d3a842"/>
</div>

# BLDC Motor Controller

> [!NOTE]  
> This project is currently in active development. Features and documentation are subject to change.

## Table of Contents
- [Project Overview](#project-overview)
- [Folder Structure](#folder-structure)
- [Mechanical Design](#mechanical-design)
- [Electronics Design](#electronics-design)
  - [MCU PCB (Top Board)](#mcu-pcb-top-board)
  - [Power PCB (Bottom Board)](#power-pcb-bottom-board)
- [Schematics](#schematics)
  - [MCU PCB Schematic](#mcu-pcb-schematic)
  - [Power PCB Schematic](#power-pcb-schematic)
- [Firmware](#firmware)
- [Simulation](#simulation)
- [Results](#results)
- [License](#license)

---

## Project Overview

This project aims to develop a **high-precision Brushless DC (BLDC) motor controller** from scratch, covering both hardware and firmware aspects. The controller is designed for advanced motor control applications, leveraging a powerful STM32 microcontroller, a dedicated motor driver IC, and comprehensive sensing capabilities for precise operation and rotor position estimation.

The core of the system is a custom two-board stack: an MCU board housing the main control logic and a power board managing motor drive and sensing.

## Folder Structure

The project is organized into logical directories to enhance maintainability and readability:

* `simulation/`: Contains code and files related to motor control simulation.
* `mechanical/`: Houses all mechanical design files (CAD models, part designs).
* `electronics/`: Stores KiCad project files for both the MCU and Power PCBs, including schematics and PCB layouts.
* `firmware/`: Contains all the embedded software for the STM32 microcontroller.
* `controller/`: Specific implementations of motor control algorithms (trapezoidal, FOC).
* `common/`: Shared utilities and communication definitions across different parts of the project.

---

## Mechanical Design

The mechanical aspects of this project are centered around integrating the custom electronics with an off-the-shelf **EM3215 BLDC motor**. It's important to note that, for this specific project, the **motor's original windings are being utilized**, and there are no plans to re-wrap the wire around the coils. The `mechanical` folder contains various CAD files (FreeCAD) for components like the rotor adapter, chassis, and PCB cutouts, ensuring a perfect fit and robust assembly.

<div align="center">
  <img src="https://github.com/user-attachments/assets/35e5e8cf-3523-439b-9cf8-6a88cd3317ed" height="300"/>
</div>

---

## Electronics Design

The electronics consist of two interconnected custom-designed Printed Circuit Boards (PCBs), designed using **KiCad**. Both boards are **4-layer designs** to facilitate efficient signal routing and power integrity. A **Molex PCB stack connector** is used to establish the robust electrical connection between the MCU and Power PCBs.

### MCU PCB (Top Board)

The top PCB serves as the brain of the controller, housing the main microcontroller and various sensors and interfaces for user interaction and precise feedback.

| Layer 1 (Top Copper) | Layer 2 (Inner 1) | Layer 3 (Inner 2) | Layer 4 (Bottom Copper) | 3D Top View | 3D Bottom View |
| :------------------: | :---------------: | :---------------: | :---------------------: | :---------: | :------------: |
| ![image](https://github.com/user-attachments/assets/e3ad61e8-1968-4f51-974c-bd88d7b194d1) | ![image](https://github.com/user-attachments/assets/eda2a795-9acf-4874-b4b9-3f5ed1b4a181) |![image](https://github.com/user-attachments/assets/e96e2e1d-825a-4590-90dd-7f235e1fa9ca)|![image](https://github.com/user-attachments/assets/3b5efa68-7e7b-4f3e-bcba-d8abd25edf7d) | ![image](https://github.com/user-attachments/assets/b6fc8bb2-36ec-4e66-9482-b3439ea36996) | ![image](https://github.com/user-attachments/assets/9a11b0be-57d6-442e-8f05-b2bf757be1ca) |

**Key Components on the MCU PCB:**

* **STM32F4 Microcontroller:** The central processing unit for control algorithms, sensor data acquisition, and communication.
* **MT6701 Angle Position Encoder:** Provides high-resolution absolute rotor position feedback for precise commutation and control.
* **SK6805 LED Ring:** A customizable visual indicator for status and debugging.
* **Reset Button:** For convenient system resets.
* **ISM330DHCX IMU (Inertial Measurement Unit):** Provides acceleration and angular velocity data, useful for advanced motion control or system stabilization.

### Power PCB (Bottom Board)

The bottom PCB handles the high-current motor driving functions and essential power monitoring.

| Layer 1 (Top Copper) | Layer 2 (Inner 1) | Layer 3 (Inner 2) | Layer 4 (Bottom Copper) | 3D Top View | 3D Bottom View |
| :------------------: | :---------------: | :---------------: | :---------------------: | :---------: | :------------: |
| ![image](https://github.com/user-attachments/assets/d9922683-cbf5-4868-8b58-452e7566250c) | ![image](https://github.com/user-attachments/assets/d3ce2ac5-880d-4d81-8f6c-d508b4a6b694) | ![image](https://github.com/user-attachments/assets/56faf783-717c-470a-8ce2-7477bf948e30) | ![image](https://github.com/user-attachments/assets/879c9ce4-2996-44c6-8344-d4e239203147)| ![image](https://github.com/user-attachments/assets/ff8c0972-ba50-4a18-bf02-6bc89084233d) | ![image](https://github.com/user-attachments/assets/dca16bc5-c2b6-410b-afd7-4b836abd5348) |

**Key Components on the Power PCB:**

* **TMC6300 Motor Driver:** A powerful and integrated 3-phase motor driver, responsible for generating the PWM signals to control the motor phases.
* **CAN (Controller Area Network) Transceiver:** Enables robust communication with other electronic systems. It will be used later to connect motors in series.
* **J-Link Connector:** Provides easy debugging and programming interface for the STM32 microcontroller.
* **USB Connector:** Allows for direct communication with a host PC for data logging, control, and visualizations.
* **Voltage Sensor:** For monitoring the DC bus voltage, essential for safe operation and accurate power calculations.
* **INA239-Q1 Current Sensors (x3):** One dedicated current sensor for each motor phase, enabling precise current measurement for advanced control algorithms like Field-Oriented Control (FOC).

---

## Schematics

Here you will find the schematics for both the MCU and Power PCBs. These diagrams detail the interconnections of all components and are crucial for understanding the electrical design. You can find a more up to date version of them by opening the KiCad project file.

### MCU PCB Schematic

![image](https://github.com/user-attachments/assets/f03aca32-99ec-416c-8252-476495d778d1)

### Power PCB Schematic

![image](https://github.com/user-attachments/assets/b6926eec-cd2d-4039-8d5a-8df13450388b)

---

## Firmware

The firmware, developed for the **STM32F4** microcontroller, is the intelligence behind the motor control. It includes:

* **Low-level Peripheral Drivers:** Custom drivers for ADC, timers, GPIO, SPI, I2C, UART, USB, and DMA to efficiently manage hardware resources.
* **Motor Control Algorithms:** Implementation of both **trapezoidal** and **Field-Oriented Control (FOC)** algorithms, located in the `controller` directory, to provide flexible and high-performance motor driving capabilities.
* **Sensor Integration:** Code to interface with the MT6701 encoder, INA239-Q1 current sensors, voltage sensor, and ISM330DHCX IMU for comprehensive feedback.
* **System Utilities:** Logging, error handling, and platform-specific configurations for the STM32 environment.

_The firmware architecture and design decisions will be documented in the future._

---

## Simulation

The `simulation` directory contains resources for simulating the BLDC motor system and visualizing the sensor data in real time. This allows for testing and validating control algorithms and system behavior in a virtual environment before deployment on the hardware. The `bldc.atta` file can be opened with [Atta](https://github.com/brenocq/atta), and it implements a basic motor model simulation and plotting of sensor data.

_Instructions on how to run the BLDCM simulation with [Atta](https://github.com/brenocq/atta) will be added in the future_

---

## Results

Below are some pictures/videos taken during the development. The controller design and calibration procedures will be explained in depth in the future :)

https://github.com/user-attachments/assets/e86b0316-7ecf-4e15-9d3b-c0315d690438

<div align="center">
  <img src="https://github.com/user-attachments/assets/54bf5e98-5cf2-4c85-9d33-0520dd883df0" height="300"/>
  <img src="https://github.com/user-attachments/assets/4c7803e4-1f16-4ebd-a230-3d951dd21e42" height="300"/>
  <img src="https://github.com/user-attachments/assets/f1e9a11f-33d6-4eaf-ae9b-b5f8f190689e" height="300"/>
</div>

## License
This project is licensed under the MIT License - check [LICENSE](LICENSE) for details.
