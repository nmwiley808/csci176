/*
 * CSCI 176 - Parallel Processing
 * Program 1: Task-Parallel Fibonacci Comparison using Pipes
 * Language: C++20
 * Environment: CLion
 *
 * Author: Noah Wiley
 * Description:
 *      This program compares recursive and iterative Fibonacci
 *      implementations using task-parallel processing with
 *      Unix pipes and fork().
 *
 *  Processes:
 *      Parent: creates pipes and forks children
 *      Child 1: controller
 *      Child 2: recursive Fibonacci
 *      Child 3: iterative Fibonacci
 *
 * Compilation:
 *      g++ -std=c++20 csci176P1.cpp -o csci176P1
 *
 * Execution:
 *      ./csci176P1 <N>
 */

#include <unistd.h>     // fork, pipe, read, write
#include <sys/wait.h>   // wait
#include <iostream>
#include <chrono>
#include <cstdlib>

using namespace std;

// Fibonacci Implementations

long long fib_recursive(const int n) {
    if (n <= 2) return 1;
    return fib_recursive(n-1) + fib_recursive(n-2);
}

long long fib_iterative(const int n) {
    if (n <= 2) return 1;
    long long a = 1, b = 1;
    for (int i = 3; i <= n; ++i) {
        const long long temp = a + b;
        a = b;
        b = temp;
    }
    return b;
}

// Main

int main(const int argc, char* argv[]) {
    if (argc != 2) {
        cerr << "Usage: ./csci176P1 <N>\n";
        return EXIT_FAILURE;
    }

    const int N = atoi(argv[1]);

    // Pipes
    int ctrl_to_rec[2];
    int ctrl_to_itr[2];
    int rec_to_ctrl[2];
    int itr_to_ctrl[2];

    pipe(ctrl_to_rec);
    pipe(ctrl_to_itr);
    pipe(rec_to_ctrl);
    pipe(itr_to_ctrl);

    // Child 1: Controller

    if (fork() == 0) {
        write(ctrl_to_rec[1], &N, sizeof(int));
        write(ctrl_to_itr[1], &N, sizeof(int));

        double rec_time{}, itr_time{};
        read(rec_to_ctrl[0], &rec_time, sizeof(double));
        read(itr_to_ctrl[0], &itr_time, sizeof(double));

        cout << "\n=== Fibonacci Timing Results ===\n";
        cout << "Recursive time : " << rec_time << " seconds\n";
        cout << "Iterative time : " << itr_time << " seconds\n";

        _exit(0);
    }

    // Child 2: Recursive

    if (fork() == 0) {
        int n{};
        read(ctrl_to_rec[0], &n, sizeof(int));

        const auto start = chrono::steady_clock::now();
        const long long result = fib_recursive(n);
        const auto end = chrono::steady_clock::now();

        const double elapsed = chrono::duration<double>(end - start).count();

        cout << "Recursive Fibonacci("<< n << ") = " << result << 'n';

        write(rec_to_ctrl[1], &elapsed, sizeof(double));
        _exit(0);
    }

    // Child 3: Iterative

    if (fork() == 0) {
        int n{};
        read(ctrl_to_itr[0], &n, sizeof(int));

        const auto start = chrono::steady_clock::now();
        const long long result = fib_iterative(n);
        const auto end = chrono::steady_clock::now();

        const double elapsed = chrono::duration<double>(end - start).count();

        cout << "Iterative Fibonacci(" << n << ") = " << result << '\n';

        write(itr_to_ctrl[1], &elapsed, sizeof(double));
        _exit(0);
    }

    // Parent Waits
    for (int i = 0; i < 3; ++i) {
        wait(nullptr);
    }

    return 0;
}
