# MPI Programming Examples

This repository contains various examples of parallel programming using the Message Passing Interface (MPI). The examples demonstrate fundamental concepts of MPI and parallel computing by implementing common algorithms and techniques.

## Table of Contents
  - [Parallel Addition](#parallel-addition)
  - [Parallel Bolle's Integration](#parallel-bolles-integration)
  - [Matrix-Vector Product](#matrix-vector-product)
  - [Jacobi Iterative Method](#jacobi-iterative-method)
  - [Parallel Quick Sort](#parallel-quick-sort)
  - [Matrix Multiplication](#matrix-multiplication)
  - [Parallel Gaussian Elimination](#parallel-gaussian-elimination)
  - [Cannon's Algorithm for Matrix Multiplication](#cannons-algorithm-for-matrix-multiplication)

## Examples

### Parallel Addition
This example demonstrates how to perform parallel addition of large arrays using MPI. Each process is responsible for summing a portion of the array.

### Parallel Bolle's Integration
This program calculates the integral of a function using Bolle's method in parallel, distributing the calculation across multiple processes.

### Matrix-Vector Product
The matrix-vector product example shows how to efficiently compute the product of a matrix and a vector in parallel, utilizing MPI for data distribution.

### Jacobi Iterative Method
This implementation of the Jacobi iterative method solves a system of linear equations in parallel, distributing the computational workload among multiple processes.

### Parallel Quick Sort
The parallel quick sort example showcases a parallel sorting algorithm, using MPI to distribute the sorting workload and combine results.

### Matrix Multiplication
This example performs matrix multiplication in parallel using MPI, demonstrating how to efficiently distribute and compute the product of two matrices.

### Parallel Gaussian Elimination
The parallel Gaussian elimination example illustrates how to solve linear equations by distributing the work among multiple processes.

### Cannon's Algorithm for Matrix Multiplication
This implementation of Cannon's algorithm shows an efficient way to multiply matrices in parallel, utilizing a grid-based approach for data distribution.

### Prerequisites

- C Compiler (GCC recommended)
- MPI Library (MPICH or OpenMPI)
- Make (optional)

## Usage

1. Clone the repository:
   ```bash
   git clone https://github.com/chandrashekhar-patil/MPI-.git
   cd MPI
