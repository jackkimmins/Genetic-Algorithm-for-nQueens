# n-Queens Solved with Genetic Algorithms
## What is The N-queens Problem?
The n-Queens problem is a classic problem in computer science and mathematics that involves finding a way to place `n` queens on an `n x n` chessboard such that no queen is attacking any other queen.

>  A queen can attack any piece that is on the same row, column, or diagonal as the queen.

The n-Queens problem is often used as a benchmark for testing the performance of algorithms, because it is a problem that is easy to understand but difficult to solve. The problem becomes increasingly difficult as the value of `n` increases, making it a good test of the efficiency of an algorithm.

## My Genetic Algorithm Implementation
I have implemented a Genetic Algorithm to solve the n-Queens problem. Genetic Algorithms are a type of optimisation algorithm that use principles of natural selection and genetics to find solutions to problems.

### My Approach
I used C++ for my implementation because it is a fast and efficient programming language that is well-suited for implementing the Genetic Algorithm. Some of the ideas and genetic variation operators that I used are listed below:
 - **Tournament Selection**: A method of selecting candidate solutions for breeding by randomly selecting a group of candidates and choosing the fittest one as the parent.
 - **Multi-point Crossover**: A method of creating offspring from parent solutions by combining their genetic
   information at multiple points.
 - **Adaptive Mutation**: A method of introducing small changes to candidate solutions by randomly altering
   their genetic information, with the rate of mutation being more
   likely to occur in less fit solutions.

### Example
![Example Image of Console Program](https://github.com/jackkimmins/NQueens/raw/master/Example.png)

~*n-Size of `8`, done with compiler optimisations enabled (`0.000117s` without - `0.000001s` with)*


| `n` | `clock_t` | real | user |
|--|--|--|--|
| 8 | 0.000001s | 0.001s | 0.001s |
| 16 | 0.000862s | 0.002s | 0.001s |
| 32 | 0.011414s | 0.012s | 0.0012s |
| 64 | 0.124400s | 0.126s | 0.126s |
| 128 | 1.471171s | 1.475s | 1.475s |
| 256 | 16.249298s | 16.262s | 16.26s |