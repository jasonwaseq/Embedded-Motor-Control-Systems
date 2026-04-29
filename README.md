# ECE118 Lab 3 - Embedded Motor Control Systems

In this project, I developed a comprehensive embedded motor control system using the Uno32 microcontroller to interface with multiple types of actuators, including DC motors, RC servos, and stepper motors. The system incorporated software-generated PWM signals to regulate motor speed and position, analog sensor inputs for real-time control, and hardware driver modules such as the DS3658 and L298 H-bridge for power handling and bidirectional control. I conducted in-depth analysis of motor behavior, including inductive kickback effects and waveform characteristics, using oscilloscopes to optimize circuit performance and reliability. Additionally, I implemented state-machine-driven stepper motor control and compared direct coil driving with dedicated driver ICs to evaluate trade-offs in efficiency, precision, and system complexity.   

## Repository Layout

- `src/` - shared source files
- `include/` - shared headers
- `Lab3_Part*.X/` - MPLAB X project folders for each lab part
- `ECE118_Lab3.X/` - additional MPLAB X project configuration
- `scripts/` - helper scripts used during development

## Notes

- The project is intended for MPLAB X / PIC microcontroller workflows.
- Build artifacts and IDE-local files are ignored through the repo `.gitignore`.