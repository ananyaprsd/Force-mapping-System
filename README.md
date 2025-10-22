# Force-Mapping-System

## Overview
This project implements a real-time force mapping system using 6 FSR-402 sensors connected to an ESP32. The system predicts full foot pressure distribution and streams the force data via serial. This repository contains the software part of the project along with documentation, sample datasets, and hardware images.

## Folder Structure
- **src/**: Arduino/ESP32 code for reading sensors, calibration, and Kalman filtering.  
- **Docs/**: Project documentation in PDF format.  
- **Dataset/**: Example sensor data from the force mapping system.  
- **Hardware_Image/**: Images of the hardware setup.  
- **platformio.ini**: PlatformIO environment configuration for ESP32.

## Features
- Reads 6 FSR-402 sensors in real-time.  
- Converts raw ADC values to forces in Newtons.  
- Smooths measurements using a Kalman filter.  
- Streams force data over serial in the format: `f1,f2,f3,f4,f5,f6`.  
- Designed for integration with robotic applications.  

## How to Use
1. Install [PlatformIO](https://platformio.org/) in VSCode.  
2. Connect your ESP32 to your computer.  
3. Open the project folder in VSCode → PlatformIO.  
4. Upload the code to ESP32.  
5. Open the serial monitor at 115200 baud to see real-time force outputs.  

### Optional: Testing Without Hardware
- Use the dataset in `Dataset/` to simulate sensor readings.  
- You can write a Python script to visualize pressure maps from the sample data.

## Notes
- This repository contains only the software portion; hardware setup is required to run on actual sensors.  
- Documentation in `Docs/` provides detailed project methodology, calibration, and analysis.  

## Author
Ananya Prasad M
