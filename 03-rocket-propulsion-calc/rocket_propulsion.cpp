#include <iostream>
#include <cmath>
#include <iomanip>

const double PI = 3.141592653589793;

struct RocketSpec {
    double dryMassKg;
    double propellantMassKg;
    double exhaustVelocityMs;
    double thrustN;
    double burnTimeS;
    double launchAngleDeg;
};

struct FlightMetrics {
    double wetMassKg;
    double deltaVMps;
    double massFlowKgPs;
    double initialAccelMps2;
    double thrustHorizontalN;
    double thrustVerticalN;
    double accelHorizontalMps2;
    double accelVerticalMps2;
    bool valid;
};

bool validateRocketSpec(const RocketSpec& spec) {
    if (spec.dryMassKg <= 0) return false;
    if (spec.propellantMassKg <= 0) return false;
    if (spec.exhaustVelocityMs <= 0 && spec.thrustN <= 0) return false;
    if (spec.burnTimeS <= 0) return false;
    if (spec.launchAngleDeg < 0 || spec.launchAngleDeg > 90) return false;
    return true;
}

FlightMetrics computeFlightMetrics(const RocketSpec& spec) {
    FlightMetrics metrics;
    metrics.wetMassKg = spec.dryMassKg + spec.propellantMassKg;
    metrics.massFlowKgPs = spec.propellantMassKg / spec.burnTimeS;
    metrics.deltaVMps = spec.exhaustVelocityMs * std::log(metrics.wetMassKg / spec.dryMassKg);
    metrics.initialAccelMps2 = spec.thrustN / metrics.wetMassKg;
    metrics.thrustHorizontalN = spec.thrustN * std::cos(spec.launchAngleDeg * PI / 180.0);
    metrics.thrustVerticalN = spec.thrustN * std::sin(spec.launchAngleDeg * PI / 180.0);
    metrics.accelHorizontalMps2 = metrics.thrustHorizontalN / metrics.wetMassKg;
    metrics.accelVerticalMps2 = metrics.thrustVerticalN / metrics.wetMassKg;
    metrics.valid = true;
    return metrics;
}

void printFlightMetrics(const FlightMetrics& metrics) {
    if (!metrics.valid) {
        std::cout << "Invalid rocket configuration.\n";
        return;
    }
    std::cout << std::fixed << std::setprecision(2);
    std::cout << "Wet Mass (kg): " << metrics.wetMassKg << "\n";
    std::cout << "Delta V (m/s): " << metrics.deltaVMps << "\n";
    std::cout << "Mass Flow Rate (kg/s): " << metrics.massFlowKgPs << "\n";
    std::cout << "Initial Acceleration (m/s^2): " << metrics.initialAccelMps2 << "\n";
    std::cout << "Thrust Horizontal (N): " << metrics.thrustHorizontalN << "\n";
    std::cout << "Thrust Vertical (N): " << metrics.thrustVerticalN << "\n";
    std::cout << "Horizontal Acceleration (m/s^2): " << metrics.accelHorizontalMps2 << "\n";
    std::cout << "Vertical Acceleration (m/s^2): " << metrics.accelVerticalMps2 << "\n";
}

int main() {
    RocketSpec rockets[] = {
        {1000, 500, 3000, 100000, 10, 0}, // Vertical launch
        {1000, 500, 3000, 100000, 10, 45}, // Angled launch
        {1000, 500, 0, 0, 10, 0}, // Invalid case: no thrust
        {1000, 1000, 3000, 100000, 10, 0} // Invalid case: propellant mass >= wet mass
    };

    for (const auto& rocket : rockets) {
        std::cout << "Rocket Configuration:\n";
        std::cout << "Dry Mass (kg): " << rocket.dryMassKg << "\n";
        std::cout << "Propellant Mass (kg): " << rocket.propellantMassKg << "\n";
        std::cout << "Exhaust Velocity (m/s): " << rocket.exhaustVelocityMs << "\n";
        std::cout << "Thrust (N): " << rocket.thrustN << "\n";
        std::cout << "Burn Time (s): " << rocket.burnTimeS << "\n";
        std::cout << "Launch Angle (deg): " << rocket.launchAngleDeg << "\n";

        if (validateRocketSpec(rocket)) {
            FlightMetrics metrics = computeFlightMetrics(rocket);
            printFlightMetrics(metrics);
        } else {
            std::cout << "Invalid rocket configuration.\n";
        }
        std::cout << "--------------------------\n";
    }

    return 0;
}
