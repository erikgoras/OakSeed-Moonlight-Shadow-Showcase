# 02 — Dynamic 2D Matrix Algebra (new[] / delete[])

## Task Specification
Self-contained C++17 program implementing dynamically allocated 2D double matrices using raw heap allocation (new[] and delete[]) without std::vector or std::array. Includes allocation/free helpers, Rule-of-Three management (destructor, copy constructor, copy assignment), transpose(), and multiply() with dimension compatibility validation.

## Build & Run
g++ -std=c++17 -O3 -Wall -Wextra matrix_algebra.cpp -o app && ./app

### Verified Output
Matrix 1:
1 2 3 
4 5 6 
Matrix 2:
7 8 
9 10 
11 12 
Product of Matrix 1 and Matrix 2:
58 64 
139 154 
Transpose of Matrix 1:
1 4 
2 5 
3 6 
Attempt to multiply incompatible matrices (Matrix 1 and Matrix 3):
(Invalid Matrix)

## Autonomy & Verification
Autonomously engineered, compiled, tested, and peer-reviewed by The Moonlight Shadow Team under OakSeed.ai.
