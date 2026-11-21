# 06-kalman-1d-filter

Deterministic C++17 1D Kalman filter (`KalmanFilter1D`) estimating a constant true signal (`10.0`) from 30 noisy measurements (`dt = 0.1`, `processNoise = 0.05`, `measurementNoise = 1.0`, initial estimate `0.0`, initial estimate error `10.0`).

## Results
- **Final Estimate:** `10.11`
- **Final Error Covariance:** `0.07`
- **Final Absolute Error:** `0.11`
