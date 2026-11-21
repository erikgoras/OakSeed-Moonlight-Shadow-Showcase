#include <algorithm>
#include <cmath>
#include <iomanip>
#include <iostream>

struct PidConfig {
    double kp = 2.0;
    double ki = 0.5;
    double kd = 0.05;
    double integralMin = -20.0;
    double integralMax = 20.0;
    double outputMin = -10.0;
    double outputMax = 10.0;
};

class PidController {
public:
    explicit PidController(const PidConfig& cfg) : cfg_(cfg) {}

    void reset() {
        integral_ = 0.0;
        prevError_ = 0.0;
        hasPrevError_ = false;
    }

    double update(double setpoint, double measurement, double dt,
                  double& pOut, double& iOut, double& dOut) {
        const double error = setpoint - measurement;
        pOut = cfg_.kp * error;

        integral_ = std::clamp(integral_ + error * dt, cfg_.integralMin, cfg_.integralMax);
        iOut = cfg_.ki * integral_;

        if (!hasPrevError_) {
            dOut = 0.0;
            hasPrevError_ = true;
        } else {
            dOut = cfg_.kd * (error - prevError_) / dt;
        }
        prevError_ = error;

        const double raw = pOut + iOut + dOut;
        return std::clamp(raw, cfg_.outputMin, cfg_.outputMax);
    }

private:
    PidConfig cfg_;
    double integral_ = 0.0;
    double prevError_ = 0.0;
    bool hasPrevError_ = false;
};

class FirstOrderPlant {
public:
    FirstOrderPlant(double initialState, double timeConstant, double gain)
        : state_(initialState), tau_(timeConstant), gain_(gain) {}

    double step(double controlInput, double dt) {
        const double dState = (-state_ + gain_ * controlInput) / tau_;
        state_ += dState * dt;
        return state_;
    }

    double state() const { return state_; }

private:
    double state_;
    double tau_;
    double gain_;
};

int main() {
    constexpr double kSetpoint = 10.0;
    constexpr double kDt = 0.1;
    constexpr int kSteps = 60;
    constexpr double kTolerance = 0.1;

    const PidConfig cfg{2.0, 0.5, 0.05, -20.0, 20.0, -10.0, 10.0};
    PidController pid(cfg);
    FirstOrderPlant plant(0.0, 1.0, 1.0);

    double maxOvershoot = 0.0;
    bool steadyStateReached = false;

    std::cout << std::fixed << std::setprecision(2);
    std::cout << "OakSeed PID Controller Step\n";
    std::cout << "Setpoint=" << kSetpoint << ", dt=" << kDt << ", steps=" << kSteps
              << ", Kp=" << cfg.kp << ", Ki=" << cfg.ki << ", Kd=" << cfg.kd << "\n";
    std::cout << "-----------------------------------------------------------------\n";
    std::cout << " Step    Error   P_Term   I_Term   D_Term  Ctrl_Out     State\n";
    std::cout << "-----------------------------------------------------------------\n";

    for (int s = 1; s <= kSteps; ++s) {
        const double current = plant.state();
        const double error = kSetpoint - current;
        double pTerm = 0.0, iTerm = 0.0, dTerm = 0.0;
        const double u = pid.update(kSetpoint, current, kDt, pTerm, iTerm, dTerm);
        const double nextState = plant.step(u, kDt);

        const double overshoot = std::max(0.0, nextState - kSetpoint);
        maxOvershoot = std::max(maxOvershoot, overshoot);
        steadyStateReached = (std::abs(kSetpoint - nextState) <= kTolerance);

        std::cout << std::setw(5) << s
                  << std::setw(9) << error
                  << std::setw(9) << pTerm
                  << std::setw(9) << iTerm
                  << std::setw(9) << dTerm
                  << std::setw(10) << u
                  << std::setw(10) << nextState << "\n";
    }

    const double finalState = plant.state();
    const double finalError = std::abs(kSetpoint - finalState);

    std::cout << "-----------------------------------------------------------------\n";
    std::cout << "Final State: " << finalState
              << ", Final Error: " << finalError
              << ", Max Overshoot: " << maxOvershoot
              << ", Steady State Reached: " << (steadyStateReached ? "Yes" : "No") << "\n";

    return 0;
}
