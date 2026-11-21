#include <iostream>
#include <iomanip>
#include <array>
#include <vector>
#include <algorithm>
#include <numeric>

template <typename T, std::size_t Capacity>
class RingBuffer {
    static_assert(Capacity > 0, "Capacity must be greater than 0");

    std::array<T, Capacity> buffer;
    std::size_t head = 0;
    std::size_t count = 0;
    std::size_t pushedCount = 0;
    std::size_t overwrittenCount = 0;

public:
    void push(const T& value) {
        if (full()) {
            overwrittenCount++;
        } else {
            pushedCount++;
        }
        buffer[head] = value;
        head = (head + 1) % Capacity;
        if (count < Capacity) {
            count++;
        }
    }

    bool empty() const {
        return count == 0;
    }

    bool full() const {
        return count == Capacity;
    }

    std::size_t size() const {
        return count;
    }

    std::size_t capacity() const {
        return Capacity;
    }

    std::vector<T> orderedValues() const {
        std::vector<T> result;
        result.reserve(count);
        std::size_t start = (Capacity + head - count) % Capacity;
        for (std::size_t i = 0; i < count; ++i) {
            result.push_back(buffer[(start + i) % Capacity]);
        }
        return result;
    }

    std::pair<T, T> minMax() const {
        auto values = orderedValues();
        auto [minIt, maxIt] = std::minmax_element(values.begin(), values.end());
        return {*minIt, *maxIt};
    }

    double average() const {
        auto values = orderedValues();
        double sum = std::accumulate(values.begin(), values.end(), 0.0);
        return sum / values.size();
    }

    std::size_t pushed() const {
        return pushedCount;
    }

    std::size_t overwritten() const {
        return overwrittenCount;
    }
};

int main() {
    RingBuffer<double, 8> telemetry;
    const std::array<double, 15> samples = {
        10.1, 20.2, 30.3, 40.4, 50.5,
        60.6, 70.7, 80.8, 90.9, 100.1,
        110.11, 120.12, 130.13, 140.14, 150.15
    };

    std::cout << std::fixed << std::setprecision(2);

    for (std::size_t i = 0; i < samples.size(); ++i) {
        telemetry.push(samples[i]);
        std::cout << "Step " << (i + 1) << ": ";
        std::cout << "Pushed: " << telemetry.pushed() << ", ";
        std::cout << "Overwritten: " << telemetry.overwritten() << ", ";
        std::cout << "Size: " << telemetry.size() << ", ";
        std::cout << "Capacity: " << telemetry.capacity() << std::endl;

        auto [min, max] = telemetry.minMax();
        double avg = telemetry.average();
        std::cout << "Min: " << min << ", Max: " << max << ", Avg: " << avg << std::endl;
        std::cout << "Buffer: ";
        for (const auto& value : telemetry.orderedValues()) {
            std::cout << value << " ";
        }
        std::cout << std::endl << std::endl;
    }

    std::cout << "Final Summary:" << std::endl;
    std::cout << "Pushed: " << telemetry.pushed() << ", ";
    std::cout << "Overwritten: " << telemetry.overwritten() << std::endl;
    auto [min, max] = telemetry.minMax();
    double avg = telemetry.average();
    std::cout << "Min: " << min << ", Max: " << max << ", Avg: " << avg << std::endl;
    std::cout << "Buffer: ";
    for (const auto& value : telemetry.orderedValues()) {
        std::cout << value << " ";
    }
    std::cout << std::endl;

    return 0;
}
