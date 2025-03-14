# Bank Loan Rate Optimization System

## Project Overview

This diploma project implements a simulation system to optimize bank loan rates by analyzing multiple enterprises' payment behaviors. The system uses statistical modeling and parallel processing to determine optimal loan rates that maximize bank profits while considering enterprise payment capabilities.

## Project Structure

```
Diploma/
├── 1loanrate.cpp        # Main simulation with single iteration
├── 2loanrate.cpp        # Enhanced simulation with n=10 epochs
├── prog_24.cpp          # Standalone rate optimization program
├── distr.py            # Distribution analysis script
├── f1.txt             # Sample input configuration
├── f2.txt             # Sample input configuration
├── f3.txt             # Sample input configuration
└── f11.txt            # Sample input configuration
```

## Features

- Multi-threaded simulation using pthreads
- Statistical analysis of enterprise payment behavior
- Random number generation with normal distribution
- Configurable parameters:
  - Number of enterprises
  - Loan period (months)
  - Total loan amount
  - Expected payment rate (a)
  - Payment variance (sigma)
  - Number of processing threads

## Technical Details

### Input Parameters

- `num_pr`: Number of enterprises
- `month`: Loan period in months
- `bank`: Total loan amount
- `a`: Expected payment rate (e.g., 1.3)
- `sigma`: Payment variance (e.g., 0.1)
- `pt_count`: Number of parallel threads

### Simulation Models

1. **Single Iteration (1loanrate.cpp)**

   - Basic simulation model
   - Direct result output

2. **Multi-Epoch (2loanrate.cpp)**

   - n=10 epochs for each configuration
   - Averaged results for better stability

3. **Rate Optimization (prog_24.cpp)**
   - Standalone program for rate optimization
   - Iterative rate testing

### Sample Configuration (f1.txt)

```
10 10 1000    # enterprises, months, total amount
1.3 0.1       # payment rate, variance
2             # thread count
```

## Setup and Compilation

```bash
# Compile programs
g++ -o 1loanrate 1loanrate.cpp -pthread
g++ -o 2loanrate 2loanrate.cpp -pthread
g++ -o prog_24 prog_24.cpp
```

## Usage

1. **Direct Input Mode**

```bash
./1loanrate
# Follow prompts for parameters
```

2. **File Input Mode**

```bash
./1loanrate f1.txt
./2loanrate f2.txt
```

3. **Rate Optimization**

```bash
./prog_24 f1.txt
```

## Example Results

```
Input parameters:
- Enterprises: 10
- Months: 10
- Total amount: 1000
- Payment rate: 1.3
- Variance: 0.1

Output:
Maximum rate = 1.23
Maximum profit = 157.8
Average lifetime = 83.5
```

## Implementation Details

### Random Number Generation

```cpp
double gen_rand(double a, double sigma) {
    random_device rd;
    mt19937 gen(rd());
    normal_distribution<> d(a, sigma);
    return d(gen);
}
```

### Parallel Processing

- Uses POSIX threads (pthread)
- Thread-safe random number generation
- Shared memory for result collection

## Future Improvements

- GUI interface for parameter input
- Real-time visualization of results
- Extended statistical analysis
- Database integration for result storage
- Machine learning integration for parameter optimization

## License

MIT License

## Author

MSU Diploma Project, 2021

## Acknowledgments

- Moscow State University
- Department of Computational Mathematics# Diploma
  2021 MSU diploma

Includes 1loanrate.cpp, 2loanrate.cpp programms that performs a simulation to find the combination of parameters (my_rate, a, and sigma) that results in the maximum average earnings for a given number of enterprises and months. The results are optionally written to output files (ex: f1.txt, f2.txt, ...).

Difference between two programs in parameter n=10 which means as epoch. Helps to find mean value for the same outputs.
