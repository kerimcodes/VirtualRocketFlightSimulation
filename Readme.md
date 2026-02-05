Virtual Rocket Flight Computer & GNC Simulation

This project simulates a rocket flight computer, sensor dynamics, and GNC (Guidance, Navigation, and Control) algorithms from scratch using C for the physics engine and Python for data analysis.

Key Features:

Physics Engine: Custom 1-DOF rocket physics simulation (Thrust, Gravity, Mass variation).

Sensor Modeling: Realistic IMU sensor simulation with Gaussian noise and bias.

Signal Processing: Real-time noise filtering using Moving Average Filter.

State Estimation: Finite State Machine (FSM) implementation to detect flight phases (IDLE, ASCENT, DESCENT, LANDED).

Data Visualization: Post-flight analysis using Python (Pandas/Matplotlib)