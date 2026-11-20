#include <iostream>
#include <cmath>
#include <iomanip>

constexpr double LUNAR_GRAVITY = 1.625;
constexpr double PI = 3.14159265358979323846;

struct LanderConfig {
    double dryMassKg;
    double initialFuelMassKg;
    double initialAltitudeM;
    double initialVerticalVelMps;
    double initialHorizontalVelMps;
    double thrustN;
    double exhaustVelocityMs;
    double fuelBurnRateKgPs;
    double dtSec;
    int maxSteps;
};

struct StepState {
    double timeS;
    double altitudeM;
    double verticalVelMps;
    double horizontalVelMps;
    double fuelMassKg;
    double accelVerticalMps2;
};

struct LandingSummary {
    bool valid;
    bool safeLanding;
    bool outOfFuelBeforeTouchdown;
    double finalAltitudeM;
    double finalVerticalVelMps;
    double finalHorizontalVelMps;
    double remainingFuelKg;
    double fuelUsedKg;
    int stepsSimulated;
};

bool validateConfig(const LanderConfig& config) {
    if (config.dryMassKg <= 0 || config.initialFuelMassKg < 0 || config.initialAltitudeM <= 0 ||
        config.thrustN <= 0 || config.exhaustVelocityMs <= 0 || config.fuelBurnRateKgPs <= 0 ||
        config.dtSec <= 0 || config.maxSteps <= 0) {
        return false;
    }
    return true;
}

LandingSummary simulateLunarDescent(const LanderConfig& config) {
    LandingSummary summary{};
    if (!validateConfig(config)) {
        summary.valid = false;
        return summary;
    }

    summary.valid = true;
    double fuelUsedKg = 0.0;
    double massAtStep = config.dryMassKg + config.initialFuelMassKg;
    double altitudeM = config.initialAltitudeM;
    double verticalVelMps = config.initialVerticalVelMps;
    double horizontalVelMps = config.initialHorizontalVelMps;
    double fuelMassKg = config.initialFuelMassKg;

    for (int step = 0; step < config.maxSteps; ++step) {
        double thrustAccel = (massAtStep > 0) ? (config.thrustN / massAtStep) : 0.0;
        double netAccelMps2 = thrustAccel - LUNAR_GRAVITY;
        verticalVelMps += netAccelMps2 * config.dtSec;
        altitudeM += verticalVelMps * config.dtSec;

        if (fuelMassKg > 0) {
            double fuelBurned = config.fuelBurnRateKgPs * config.dtSec;
            fuelBurned = std::min(fuelBurned, fuelMassKg);
            fuelMassKg -= fuelBurned;
            fuelUsedKg += fuelBurned;
            massAtStep -= fuelBurned;
        } else {
            fuelUsedKg = config.initialFuelMassKg;
        }

        if (altitudeM <= 0) {
            summary.stepsSimulated = step + 1;
            summary.finalAltitudeM = 0;
            summary.finalVerticalVelMps = verticalVelMps;
            summary.finalHorizontalVelMps = horizontalVelMps;
            summary.remainingFuelKg = fuelMassKg;
            summary.fuelUsedKg = fuelUsedKg;
            summary.safeLanding = (std::abs(verticalVelMps) < 2.0 && std::abs(horizontalVelMps) < 2.0);
            summary.outOfFuelBeforeTouchdown = (fuelMassKg == 0);
            return summary;
        }
    }

    summary.stepsSimulated = config.maxSteps;
    summary.finalAltitudeM = altitudeM;
    summary.finalVerticalVelMps = verticalVelMps;
    summary.finalHorizontalVelMps = horizontalVelMps;
    summary.remainingFuelKg = fuelMassKg;
    summary.fuelUsedKg = fuelUsedKg;
    summary.safeLanding = false;
    summary.outOfFuelBeforeTouchdown = (fuelMassKg == 0);
    return summary;
}

void printLandingSummary(const LandingSummary& summary) {
    if (!summary.valid) {
        std::cout << "Invalid configuration for landing simulation." << std::endl;
        return;
    }

    std::cout << "Landing Simulation Summary:" << std::endl;
    std::cout << "Safe Landing: " << (summary.safeLanding ? "Yes" : "No") << std::endl;
    std::cout << "Out of Fuel Before Touchdown: " << (summary.outOfFuelBeforeTouchdown ? "Yes" : "No") << std::endl;
    std::cout << "Final Altitude: " << std::fixed << std::setprecision(2) << summary.finalAltitudeM << "m" << std::endl;
    std::cout << "Final Vertical Velocity: " << std::fixed << std::setprecision(2) << summary.finalVerticalVelMps << "m/s" << std::endl;
    std::cout << "Final Horizontal Velocity: " << std::fixed << std::setprecision(2) << summary.finalHorizontalVelMps << "m/s" << std::endl;
    std::cout << "Remaining Fuel: " << std::fixed << std::setprecision(2) << summary.remainingFuelKg << "kg" << std::endl;
    std::cout << "Fuel Used: " << std::fixed << std::setprecision(2) << summary.fuelUsedKg << "kg" << std::endl;
    std::cout << "Steps Simulated: " << summary.stepsSimulated << std::endl;
}

int main() {
    LanderConfig config1 = {
        1500.0, 1500.0, 10000.0, -10.0, 0.0, 2000.0, 250.0, 50.0, 0.1, 1000
    };

    LanderConfig config2 = {
        1000.0, 1000.0, 5000.0, -5.0, 0.0, 1000.0, 250.0, 30.0, 0.05, 2000
    };

    LanderConfig config3 = {
        500.0, 0.0, 10000.0, -1.0, 0.0, 0.0, 250.0, 0.0, 0.1, 1000
    };

    std::cout << "Simulation 1:" << std::endl;
    LandingSummary summary1 = simulateLunarDescent(config1);
    printLandingSummary(summary1);

    std::cout << "\nSimulation 2:" << std::endl;
    LandingSummary summary2 = simulateLunarDescent(config2);
    printLandingSummary(summary2);

    std::cout << "\nSimulation 3 (Invalid):" << std::endl;
    LandingSummary summary3 = simulateLunarDescent(config3);
    printLandingSummary(summary3);

    return 0;
}
