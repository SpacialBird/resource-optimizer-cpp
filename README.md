# Algorithmic Optimization & Performance Tuning Suite

## System Architecture & Core Objective
This repository contains a high-performance algorithmic suite designed to solve two complex combinatorial optimization problems:
1. **Workforce/Agent Pairing Maximization:** Pairs entities based on asymmetric affinity and synergy matrices to maximize total productive output.
2. **Constrained Resource Allocation (Knapsack Variant):** Allocates a strict budget across multiple variable-cost item categories, maximizing capital deployment without exceeding constraints.

The system is structured into modular C++ engines with separated logic for the core algorithms, rigorous automated unit testing, and Python-based benchmarking scripts for empirical complexity verification.

## Algorithmic Breakdown & Data Structures

### 1. Greedy Heuristic Engine (Pairing Maximization)
- **Time Complexity:** $O(N^2 \log N)$ (Worst/Average Case)
- **Space Complexity:** $O(N^2)$
- **Implementation:** 
  - Generates a complete state space of candidate pairs and sorts them descendingly using `std::sort` with custom comparators.
  - Utilizes `std::vector<bool>` for $O(1)$ assignment validation.
  - **Optimization:** Avoids dynamic array resizing or element deletion (which would incur $O(N)$ cost per step) by utilizing a logical advancing pointer, effectively reducing candidate extraction to $O(1)$ time.

### 2. Iterative Branch & Bound Engine (Resource Allocation)
- **Time Complexity:** Bounded to $O(N \cdot M \cdot K)$ (where $N$ = categories, $M$ = budget constraint, $K$ = items per category)
- **Space Complexity:** $O(N \cdot M)$
- **Implementation:**
  - Implements a depth-first search (DFS) state-space tree exploration.
  - **Precomputation:** Calculates prefix arrays (`minimos`, `maximos`) in $O(N)$ prior to execution to allow $O(1)$ bound checking at any depth level.
  - **Memoization:** Deploys a 2D boolean matrix (`visitado[nivel][suma]`) to track explored sub-problems, pruning redundant branches instantly.

## Design Patterns & Implementation Details
- **Iterative Backtracking:** Designed as a state-machine using loop constructs rather than recursion to completely eliminate call stack overhead and prevent stack overflow exceptions on deep tree traversals.
- **Cache Locality:** Prioritizes contiguous memory allocations (`std::vector`) over pointer-based nodes to maximize L1/L2 CPU cache hit rates during sorting and matrix traversals.
- **Invariant Control & Pruning:** Enforces strict execution invariants in the Branch & Bound engine:
  - *Feasibility Pruning:* If current state cost + minimum future path exceeds the budget constraint, prune.
  - *Optimality Pruning:* If current state cost + maximum theoretical future path cannot surpass the best known global optimum, prune.

## Benchmarking & Performance
Performance was rigorously profiled against various datasets (Uniform, Adversarial, Correlated distributions).
- **Greedy Engine:** Tested up to $N=2048$ ($\approx 2 \times 10^6$ candidates), achieving execution times under 260ms. Empirical scaling matched the theoretical $O(N^2 \log N)$ curve with $R^2 = 0.9967$.
- **Branch & Bound Engine:** Demonstrated radical state-space reduction. While the unoptimized brute-force approach grew exponentially $O(K^N)$ and failed at depth 28, the optimized memoization engine resolved maximal constraints (depth 200, $K=20$) in $\approx 75$ms, conforming to a constrained quadratic growth curve $O(C^2)$.

## Build & Run Instructions
The project is built using `make` with GCC optimization flags.

```bash
# Compile all optimized binaries (-O2 -std=c++17 -Wall)
make

# Run the Greedy algorithm
make run-ar

# Run the Branch & Bound algorithm
make run-bt

# Execute unit tests
./tests_AR
./tests_BT
```
