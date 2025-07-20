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

The electronics consist of two interconnected custom-designed Printed Circuit Boards (PCBs), designed using **KiCad**. Both boards are **4-layer designs** to facilitate efficient signal routing and power integrity. A **Molex PCB stack connector** is used to establish a robust electrical connection between the MCU and Power PCBs.

### MCU PCB (Top Board)

The top PCB serves as the brain of the controller, housing the main microcontroller and various sensors and interfaces for user interaction and precise feedback.

| Top Layer | Layer 1 | Layer 2 | Bottom Layer | 3D Top View | 3D Bottom View |
| :------------------: | :---------------: | :---------------: | :---------------------: | :---------: | :------------: |
| <img height="130" alt="Layer 1 (Top Copper)" src="https://github.com/user-attachments/assets/a0bdffdf-dbb2-43c1-b529-12d50b2d14fd" /> | <img height="130" alt="Layer 2 (Inner 1)" src="https://github.com/user-attachments/assets/55852d03-11d7-4d15-98dc-e1f933781a28" /> | <img height="130" alt="Layer 3 (Inner 2)" src="https://github.com/user-attachments/assets/21bd59ee-b05b-4289-b452-49b394edb754" /> | <img height="130" alt="Layer 4 (Bottom Copper)" src="https://github.com/user-attachments/assets/6d4225da-9f7a-4c76-8256-08973aba0530" /> | <img height="130" alt="3D Top View" src="https://github.com/user-attachments/assets/0e6ac78a-482c-4bc0-8312-50df366507aa" /> | <img height="130" alt="3D Bottom View" src="https://github.com/user-attachments/assets/6db4fe6c-68bf-409a-b58a-c0ea64c7a62d" /> |

**MCU PCB Key Components:**
- **STM32F4 Microcontroller:** The central processing unit for control algorithms, sensor data acquisition, and communication.
- **AS5048A Angle Position Encoder:** Provides 14-bit absolute rotor position feedback for precise commutation and control.
- **SK6805 LED Ring:** A customizable visual indicator for status and debugging.
- **Reset Button:** For convenient system resets.
- **ISM330DHCX IMU (Inertial Measurement Unit):** Provides acceleration and angular velocity data, useful for advanced motion control or system stabilization.

**MCU PCB v1.0 Release Package:**
 - [Schematic](electronics/BLDCM-MCU/BLDCM-MCU.pdf)
 - [Interactive BOM](https://brenocq.s3.us-east-1.amazonaws.com/BLDCM/BLDCM-MCU-BOM-v1.0.html)
 - Gerbers

### Power PCB (Bottom Board)

The bottom PCB handles the high-current motor driving functions and essential power monitoring.

| Top Layer | Layer 1 | Layer 2 | Bottom Layer | 3D Top View | 3D Bottom View |
| :------------------: | :---------------: | :---------------: | :---------------------: | :---------: | :------------: |
| <img height="130" alt="Power PCB Layer 1 (Top Copper)" src="https://github.com/user-attachments/assets/faa62cd5-9e9b-43b6-9e27-ffdd01d51ee1" /> | <img height="130" alt="Power PCB Layer 2 (Inner 1)" src="https://github.com/user-attachments/assets/73f51755-ee40-4ae1-b4b3-89a09125597c" /> | <img height="130" alt="Power PCB Layer 3 (Inner 2)" src="https://github.com/user-attachments/assets/59dc8296-1d9e-4efc-b00d-c2b493198382" /> | <img height="130" alt="Power PCB Layer 4 (Bottom Copper)" src="https://github.com/user-attachments/assets/ed9737b9-2924-47f6-ae12-5b252e8fc6ef" /> | <img height="130" alt="Power PCB 3D Top View" src="https://github.com/user-attachments/assets/97b7900b-0eeb-4e15-8f6d-265f3d73ea2c" /> | <img height="130" alt="Power PCB 3D Bottom View" src="https://github.com/user-attachments/assets/57febf18-13cf-4071-8960-21bef18e5e0f" /> |

**Power PCB Key Components:**
- **TMC6300 Motor Driver:** A 3-phase motor driver that translates the MCU's PWM and control signals into high-current outputs for the motor phases.
- **TCAN1462 CAN Transceiver:** Enables robust CAN FD communication for daisy-chaining motors in series.
- **J-Link Connector:** Provides easy SWD debugging and programming for the STM32 microcontroller.
- **USB Connector:** Facilitates direct USB Full-Speed communication for data logging, control, and power input.
- **Micro-Lock Plus Connectors:** Robust 4-pin Molex connectors that provide a vibration-resistant link for power delivery and CAN bus communications.
- **Voltage Sensors (x4):** One monitors the DC bus voltage, while three dedicated sensors measure filtered phase voltages for safe operation and FOC.
- **INA240 Current Sensors (x3):** Three precision analog-output current sensors enable high-speed, accurate phase current measurement for FOC.

**Power PCB v1.0 Release Package:**
 - [Schematic](electronics/BLDCM-Power/BLDCM-Power.pdf)
 - [Interactive BOM](https://brenocq.s3.us-east-1.amazonaws.com/BLDCM/BLDCM-Power-BOM-v1.0.html)
 - Gerbers

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
