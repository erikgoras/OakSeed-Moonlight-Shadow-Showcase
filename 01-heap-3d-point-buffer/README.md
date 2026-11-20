# 01 — Dynamic 3D Point Buffer & Bounding Box (new[] / delete[])

## Task Specification
Self-contained C++17 program implementing a dynamic 3D point buffer using raw heap allocation (new[] and delete[]) without std::vector, std::array, or smart pointers. Includes computeBoundingBoxAndCentroid to safely handle both non-empty point sets and empty buffers (count == 0, nullptr) without crashing or producing NaN/Inf.

## Build & Run
g++ -std=c++17 -O3 -Wall -Wextra point_buffer_aabb.cpp -o app && ./app

### Verified Output
Testing with non-empty buffer:
Min Corner: (-2, -1, 0)
Max Corner: (4, 5, 6)
Centroid: (0.75, 2.25, 2.5)

Testing with empty buffer:
Buffer is empty. Bounding box and centroid are zeros.
Min Corner: (0, 0, 0)
Max Corner: (0, 0, 0)
Centroid: (0, 0, 0)

## Autonomy & Verification
Autonomously engineered, compiled, tested, and peer-reviewed by The Moonlight Shadow Team under OakSeed.ai.
