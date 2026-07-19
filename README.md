<div align="center">

# Ultimate Servo Tester V3 🛠️🚀

[![Open Source](https://img.shields.io/badge/Open%20Source-%E2%9D%A4-brightgreen.svg)](https://opensource.org/)
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)
[![PCB Size](https://img.shields.io/badge/PCB%20Size-sub--80mm-blue.svg)]()
[![Cost](https://img.shields.io/badge/Cost-Sub--%E2%82%AC40-orange.svg)]()

</div>

![Ultimate Servo Tester V3](/md_pictures/V3/hero-servotester-v3.jpg)

The **Ultimate Servo Tester V3** is a high-performance, open-source diagnostic instrument designed for makers, engineers, and robotics competitors. Driven by an Arduino Nano, this project represents a complete hardware overhaul focused on field durability, universal power handling, zero-latency graphical visualization, and dual-stage circuit protection.

Unlike standard commercial testers that only verify position, the V3 variant introduces advanced **Velocity Profiling** and a **Mechanical Boundary Capture Routine** to safely test linkages without risking physical damage or structural binding.

---

## 📸 System Overview

| Parameter          | Specification                                      |
| :----------------- | :------------------------------------------------- |
| **Processor Core** | Arduino Nano (ATmega328P @ 16MHz)                  |
| **Display Panel**  | 3-Inch High-Contrast Graphical OLED                |
| **Bus Protocol**   | High-Speed Hardware SPI (Instant Refresh Rate)     |
| **Form Factor**    | Ultra-Compact Layout (Max dimension < 80mm)        |
| **BOM Cost**       | Under €40 (100% standard off-the-shelf components) |
| **Assembly Time**  | Under 3 Hours from bare PCB to functional device   |

---

## 🔥 Key Engineering Features

### 1. Multi-Mode Advanced Control
*   **Absolute Position Mode:** Direct tracking using a high-precision rotary encoder.
*   **Velocity Sweep Mode:** Automated, uniform cycles with adjustable speed profiling to check transitions and stall points.
*   **Mechanical Boundary Capture (Keyframing):** Manually map safe minimum and maximum travel windows. This prevents the tester from driving a servo beyond its structural limit when embedded inside a tight mechanical linkage, protecting your gears and chassis.

### 2. Dual-Stage Hardware Protection Circuit
Standard hobby testers lack adequate protection, meaning a stalled or shorted servo will back-feed current and destroy the logic microcontroller. The V3 architecture resolves this via a multi-tier split-rail electrical topology:
*   **Primary Line Protection:** A **2A Fast-Blow Glass Fuse** isolates major power faults instantly.
*   **Servo Bus Shielding:** A dedicated **PTC Resettable Thermistor (Polyfuse)** dynamically chokes down excessive current during a servo stall or short circuit, automatically resetting once the fault condition clears without losing system logic power.

```
   [Power Input] ---> (2A Glass Fuse) ---> [LDO Regulator] ---> [Arduino Logic Core]
           |
           +---------> (PTC Polyfuse)  ---> [Dual Servo Output Bus Rails]
```

### 3. Universal Multi-Chemistry Power Management
No bench supply? No problem. The onboard power distribution network accepts multiple battery types, making it ideal for immediate field use at robotics competitions:
*   **2S - 3S Lithium Polymer (LiPo)** (7.4V - 11.1V)
*   **6S - 10S Nickel-Metal Hydride (NiMH)** (7.2V - 12V)
*   **8V - 12V DC Bench Power Supply** via screw terminals

---

## 📈 The Architectural Evolution

The V3 is the culmination of three distinct design sprints addressing display bottlenecks, electrical vulnerabilities, and user experience limitations:

| Attribute            | Version 1 ($V1$)               | Version 2 ($V2$)                   | Version 3 ($V3$)                                                |
| :------------------- | :----------------------------- | :--------------------------------- | :-------------------------------------------------------------- |
| **Interface Screen** | Standard 16x2 Character LCD    | Standard 16x2 Character LCD        | **Fast 3" SPI Graphical OLED**                                  |
| **Control Formats**  | Raw Pulse Position Only        | Position, Center, Velocity         | **Position, Velocity, and Custom Linkage Boundary Capture**     |
| **Bus Protocol**     | Parallel Wire (High Pin Count) | $I^2C$ Bus (Noticeable screen lag) | **High-Speed Hardware SPI (Instant Refresh)**                   |
| **Circuit Defense**  | None (Vulnerable to shorts)    | Basic minimal line filtering       | **Dual-Stage Protection (2A Glass Fuse + Dedicated Servo PTC)** |
| **Power Inputs**     | Unstable 12V Barrel Jack only  | Low-current regulated supply       | **Universal Compatibility (LiPo, NiMH, or DC Bench)**           |

---

## 🛠️ Repository Organization

```
├── Firmware/             # Optimized Arduino C++ Source Code (.ino)
├── Hardware/             # Gerber Files, Schematic Diagrams, and PCB Layouts
├── 3D_Prints/            # STL/STEP files for the pocket-sized protective enclosure
└── Documentation/        # Complete Bill of Materials (BOM) and component datasheets
```

## 🚀 Quick Start Guide

### Hardware Requirements
1. **PCB:** Fabricate the Gerber files located in `Hardware/` using any standard quick-turn PCB house.
2. **Flash the MCU:** Open `Firmware/Ultimate_Servo_Tester_V3.ino` in the Arduino IDE. Install the required SPI display libraries, select **Arduino Nano**, and hit upload.
3. **Assemble:** Populate the custom PCB board. Through-hole component design allows for painless hand-soldering in **under 3 hours**.

### Operating the UI
*   **Rotate Encoder:** Navigate menus or adjust active positioning parameters.
*   **Short Press:** Toggle between Position, Velocity, and Boundary Setup modes.
*   **Long Press:** Lock boundary keyframe parameters inside the setup menu environment.

---

## 🤝 Open Source & Contributions
This project is **100% Open Source**. Anyone can clone this repository, download the fabrication assets, order parts, and build an industrial-grade field diagnostic module for a sub-€40 investment. 

Contributions, issue reports, and hardware optimization forks are highly encouraged. Feel free to open a Pull Request!

---

## 📄 License
This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.
