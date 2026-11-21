# 05 - PID Control Step

A standalone, deterministic C++17 discrete-time PID controller simulation driving a first-order plant toward a fixed setpoint.

## Design Overview

- PidConfig holds controller gains (kp, ki, kd), integral clamping bounds, and actuator output limits.
- PidController encapsulates proportional, clamped integral (anti-windup), and derivative terms with clean initial-step handling and reset().
- FirstOrderPlant models a first-order system (tau, gain) stepped forward with fixed dt.

## Build & Run

Compile with:
g++ -std=c++17 -O3 -Wall -Wextra pid_control.cpp -o pid_control

Run with:
./pid_control
EOF

g++ -std=c++17 -O3 -Wall -Wextra pid_control.cpp -o pid_control && ./pid_control
