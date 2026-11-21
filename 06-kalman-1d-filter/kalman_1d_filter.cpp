#include <iostream>
#include <iomanip>
#include <cmath>

struct KalmanConfig {
    double processNoise;
    double measurementNoise;
    double initialEstimateError;
};

class KalmanFilter1D {
public:
    KalmanFilter1D(const KalmanConfig& config, double initialEstimate)
        : cfg_(config), x_(initialEstimate), P_(config.initialEstimateError) {}

    void predict(double dt, double controlInput = 0.0) {
        x_ += controlInput * dt;
        P_ += cfg_.processNoise * dt;
    }

    double update(double measurement) {
        double K = P_ / (P_ + cfg_.measurementNoise);
        x_ += K * (measurement - x_);
        P_ *= (1.0 - K);
        return x_;
    }

    double estimate() const { return x_; }
    double errorCovariance() const { return P_; }

private:
    KalmanConfig cfg_;
    double x_;
    double P_;
};

int main() {
    const int steps = 30;
    const double dt = 0.1;
    const double trueValue = 10.0;
    const double processNoise = 0.05;
    const double measurementNoise = 1.0;
    const double initialEstimate = 0.0;
    const double initialEstimateError = 10.0;

    KalmanConfig config = {processNoise, measurementNoise, initialEstimateError};
    KalmanFilter1D filter(config, initialEstimate);

    std::cout << std::fixed << std::setprecision(2);
    std::cout << "Step\tMeasurement\tEstimate\tErrorCovariance\tEstimationError\n";

    for (int step = 0; step < steps; ++step) {
        double measurement = trueValue + std::sqrt(measurementNoise) * std::sin(step * 0.5);
        filter.predict(dt);
        double estimate = filter.update(measurement);
        double errorCovariance = filter.errorCovariance();
        double estimationError = trueValue - estimate;

        std::cout << step << "\t" << measurement << "\t\t" << estimate << "\t\t" << errorCovariance << "\t\t" << estimationError << "\n";
    }

    double finalEstimate = filter.estimate();
    double finalErrorCovariance = filter.errorCovariance();
    double finalAbsoluteError = std::abs(trueValue - finalEstimate);

    std::cout << "Final Estimate: " << finalEstimate << "\n";
    std::cout << "Final Error Covariance: " << finalErrorCovariance << "\n";
    std::cout << "Final Absolute Error: " << finalAbsoluteError << "\n";

    return 0;
}
