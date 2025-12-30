# MPU9250_Driver_MoonLab
This is a register-level MPU9250 clone driver available in the MOON LAB. IT processes the raw data, and does not apply any kind of noise filtering algorithms.

# MPU9250 Clone Register Read Driver

## Overview

This repository contains a **minimal register-level driver** for reading data from a **clone MPU9250 IMU** using Arduino and MATLAB.

⚠️ **Important:**  
Although the chip is labeled *MPU9250*, the onboard **magnetometer (AK8963) is missing or non-functional**.  
This behavior is common in low-cost MPU9250 clones available in the market.

As a result:
- The sensor behaves effectively as a **6-DoF IMU** (Accelerometer + Gyroscope)
- **No magnetometer data is available**
- **Yaw (heading) is unobservable in absolute terms** without external aiding (e.g., GPS, vision, or magnetometer)

---

## Hardware Details

- **Sensor**: MPU9250 (Clone)
- **Functional Sensors**:
  - 3-axis Accelerometer
  - 3-axis Gyroscope
- **Missing / Non-functional**:
  - Magnetometer (AK8963 not detected / not present)

### Observed Clone Behavior
- WHO_AM_I register responds correctly
- Accel & gyro registers output valid data
- I2C bypass mode does **not** expose AK8963
- Magnetometer I2C address does not ACK
- Sensor should be treated as **MPU6500-class** hardware

---

## Coordinate Frame

- Raw sensor frame as provided by the chip
- **No NED/ENU alignment is applied in firmware**
- Frame alignment must be handled in MATLAB or downstream estimation code

---

## Arduino: `ReadRegister.ino`

### Purpose
- Initializes the MPU9250 over I2C
- Reads raw registers directly
- Streams raw accelerometer and gyroscope data over Serial

### Key Characteristics
- Register-level access (no high-level libraries)
- Intended for **debugging, validation, and research**
- Suitable for EKF / INS preprocessing in MATLAB

### Typical Registers Read
- Accelerometer:
  - `ACCEL_XOUT_H/L`
  - `ACCEL_YOUT_H/L`
  - `ACCEL_ZOUT_H/L`
- Gyroscope:
  - `GYRO_XOUT_H/L`
  - `GYRO_YOUT_H/L`
  - `GYRO_ZOUT_H/L`

### Serial Output
- Fixed-format numeric stream
- Designed to be parsed by MATLAB in real time
- No packet framing or checksum (by design, minimal driver)

---

## MATLAB Script

### Purpose
- Reads serial data from Arduino
- Parses raw accelerometer and gyroscope values
- Prepares data for:
  - Filtering
  - Calibration
  - EKF / INS fusion
  - Offline analysis

### Typical Workflow
1. Open serial port
2. Read incoming data line-by-line
3. Convert raw counts to physical units
4. Feed into:
   - Custom EKF
   - `insfilterIMU`
   - `insfilterErrorState`
   - Custom navigation stack

---











