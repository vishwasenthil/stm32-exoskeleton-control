# Wrist-Assist Device

## Project Overview
This project uses an ADXL345 accelerometer to track wrist pitch and control an actuator

## Stage 1: Data processing
This stage involves setting up the data pipeline for the device. It involves reading from the accelerometer via I2C, turning that data into useable pitch and roll values, filtering the data to remove noise, and calculating the system's real time performance.

Process:
1. Receive data: Read x, y, z axes of accelerometer.
2. Process data: Convert data to pitch and roll values.
3. Noise reduction: Use low pass filter to remove high frequency noise and a deadband to remove sensor bias.
4. Hardware output: Map pitch data to PWM duty cycles with a linear function, which is visualized via a LED for this stage.

Performance metrics:

I used the Cortex-M4 Data Watchpoint and Trace feature to measure the real time performance of the system.
| Metric         | Time (ms) | Description
| ------         | --------- | -----------
| Execution time | 1.6       | Time the CPU spent processing the control task
| System latency | 180       | Total time from user movement to 90% actuator movement
The difference between the execution time and system latency is largely due to the lag created by the low pass filter.

Filter:
Low pass filter equation: filtered_pulse = alpha * prev_filtered + (1 - alpha) * pulse;
Alpha = 0.85
Sample time = 100Hz
These values allow for the led to change smoothly with minimal lag.
