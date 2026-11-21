#include <iostream>
#include <iomanip>
#include <array>
#include <vector>
#include <algorithm>
#include <numeric>
#include <cmath>
#include <stdexcept>

template <typename T, std::size_t WindowSize>
class SlidingWindowStats {
    static_assert(WindowSize > 0, "WindowSize must be greater than 0");

private:
    std::array<T, WindowSize> window;
    std::size_t head = 0;
    std::size_t count = 0;

public:
    void push(const T& value) {
        std::size_t writeIndex = (head + count) % WindowSize;
        window[writeIndex] = value;
        if (count == WindowSize) {
            head = (head + 1) % WindowSize; // Advance oldest pointer
        } else {
            ++count;
        }
    }

    std::size_t size() const {
        return count;
    }

    std::size_t windowSize() const {
        return WindowSize;
    }

    std::vector<T> windowValues() const {
        std::vector<T> result;
        result.reserve(count);
        for (std::size_t i = 0; i < count; ++i) {
            result.push_back(window[(head + i) % WindowSize]);
        }
        return result;
    }

    T min() const {
        if (count == 0) {
            throw std::runtime_error("Window is empty");
        }
        T mn = window[head];
        for (std::size_t i = 1; i < count; ++i) {
            if (window[(head + i) % WindowSize] < mn) {
                mn = window[(head + i) % WindowSize];
            }
        }
        return mn;
    }

    T max() const {
        if (count == 0) {
            throw std::runtime_error("Window is empty");
        }
        T mx = window[head];
        for (std::size_t i = 1; i < count; ++i) {
            if (window[(head + i) % WindowSize] > mx) {
                mx = window[(head + i) % WindowSize];
            }
        }
        return mx;
    }

    T mean() const {
        if (count == 0) {
            throw std::runtime_error("Window is empty");
        }
        T sum = std::accumulate(window.begin(), window.begin() + count, T{});
        return sum / count;
    }

    T variance() const {
        if (count == 0) {
            throw std::runtime_error("Window is empty");
        }
        T meanValue = mean();
        T sumOfSquaredDifferences = 0;
        for (std::size_t i = 0; i < count; ++i) {
            T diff = window[(head + i) % WindowSize] - meanValue;
            sumOfSquaredDifferences += diff * diff;
        }
        return sumOfSquaredDifferences / count; // Population variance
    }
};

int main() {
    SlidingWindowStats<double, 5> stats;
    std::vector<double> samples = {1.2, 3.4, 5.6, 7.8, 9.0, 2.3, 4.5, 6.7, 8.9, 1.1, 3.3, 5.5};

    std::cout << std::fixed << std::setprecision(2);
    std::cout << "OakSeed Sliding Window Stats\n";
    std::cout << "---------------------------\n";

    for (const auto& sample : samples) {
        stats.push(sample);
        std::cout << "Sample: " << sample << "\n";
        std::cout << "Window Values: ";
        for (const auto& value : stats.windowValues()) {
            std::cout << value << " ";
        }
        std::cout << "\n";
        std::cout << "Size: " << stats.size() << ", Window Size: " << stats.windowSize() << "\n";
        std::cout << "Min: " << stats.min() << ", Max: " << stats.max() << ", Mean: " << stats.mean() << ", Variance: " << stats.variance() << "\n";
        std::cout << "---------------------------\n";
    }

    std::cout << "\nFinal Summary Statistics:\n";
    std::cout << "Size: " << stats.size() << ", Window Size: " << stats.windowSize() << "\n";
    std::cout << "Min: " << stats.min() << ", Max: " << stats.max() << ", Mean: " << stats.mean() << ", Variance: " << stats.variance() << "\n";

    return 0;
}
