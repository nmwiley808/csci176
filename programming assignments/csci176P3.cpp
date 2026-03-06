/*
 * CSCI 176 - Parallel Processing
 * Program 3: Parallel Prefix Sum using Pthreads
 * Language: C++ 20
 * Environment: CLion (macOS)
 *
 * Author: Noah Wiley
 *
 * Description:
 *  This program computes the prefix sum of a large 1-D array
 *  (Size = 500,000,017) using shared-memory parallelism with
 *  POSIX threads (Pthreads)
 *
 *  The array is initialized such that:
 *      arr[i] = i + 1
 *
 *   The program implements the cost-optimal parallel prefix computation model
 *   using three stages:
 *
 *      Stage 1:
 *          Each thread computes prefix sums within its assigned
 *          subarray and updates the array stages locally.
 *
 *      Step 2:
 *          Threads compute prefix sums of the highest index
 *          values of each subarray using semaphore-based
 *          synchronization.
 *
 *      Step 3:
 *          Each thread (expect thread 0) adds the prefix sum of
 *          the previous thread's subarray to every element in
 *          its assigned range.
 *
 *   The program prints:
 *      - Start and end indexes assigned to each thread
 *      - Selected prefix sum values
 *      - Total execution time
 *
 * Compilation:
 *      g++ -std=c++20 -pthread -Wno-deprecated-declarations csci176P3.cpp -o csci176P3
 *
 *   -Wno-deprecated-declarations
 *        Suppresses warnings related to deprecated functions. On macOS,
 *        the semaphore function sem_init() is marked as deprecated even
 *        though it still works correctly. This flag prevents the compiler
 *        from displaying those warnings.
 *
 * Execution:
 *      ./csci176P3 <number_of_threads>
 *
 * Example:
 *      ./csci176P3 4
 */

#include <iostream>
#include<pthread.h>
#include<semaphore.h>
#include<sys/time.h>

using namespace std;

constexpr long N = 500000017;

double *arr;
double *block_prefix;

int thread_count;

sem_t *sem_stage2;

struct ThreadData {
    int id;
    long start;
    long end;
};

void* prefix_worker(void* arg) {
    const auto data = (ThreadData*)arg;

    const int id = data->id;
    const long start = data->start;
    const long end = data->end;

    cout << "Thread_" << id << ": start_index=" << start << ", end_index=" << end << endl;

    // Stage 1: Local Prefix Sum
    for (long i = start + 1; i <= end; i++)
        arr[i] += arr[i-1];

    // Stage 2: Prefix Of Block Sums
    if (id == 0) {
        block_prefix[0] = arr[end];
        if (thread_count > 1)
            sem_post(&sem_stage2[1]);
    }
    else {
        sem_wait(&sem_stage2[id]);

        block_prefix[id] =
            block_prefix[id-1] + arr[end];
        if (id < thread_count-1)
            sem_post(&sem_stage2[id+1]);
    }

    // Stage 3: Adjust elements
    if (id > 0) {
        const double add_value = block_prefix[id-1];
        for (long i = start; i <= end; i++)
            arr[i] += add_value;
    }
    pthread_exit(NULL);
}

// Main
int main(const int argc, char* argv[]) {
    if (argc != 2) {
        cout << "Usage: ./csci176P3 <threads>" << endl;
        return 1;
    }

    thread_count = stoi(argv[1]);

    arr = new double[N];
    block_prefix = new double[thread_count];

    sem_stage2 = new sem_t[thread_count];

    for (int i = 0; i < thread_count; i++)
        sem_init(&sem_stage2[i],0,0);

    // Initialize Array
    for (long i = 0; i < N; i++)
        arr[i] = i + 1;

    pthread_t* threads = new pthread_t[thread_count];
    ThreadData* thread_data = new ThreadData[thread_count];

    const long local_n = N / thread_count;

    timeval start_time, end_time;

    gettimeofday(&start_time, NULL);

    // Create Threads
    for (int i = 0; i < thread_count; i++) {
        long start = i * local_n;
        long end;

        if (i == thread_count - 1)
            end = N - 1;
        else
            end = start + local_n -1;

        thread_data[i].id = i;
        thread_data[i].start = start;
        thread_data[i].end = end;

        cout << "thread_" << i << " is created" << endl;

        pthread_create(&threads[i], NULL, prefix_worker, &thread_data[i]);
    }

    // Join Threads
    for (int i = 0; i < thread_count; i++)
        pthread_join(threads[i], NULL);

    gettimeofday(&end_time, NULL);

    const double runtime = (end_time.tv_sec - start_time.tv_sec) + (end_time.tv_usec - start_time.tv_usec) / 1000000.0;

    // Print selected prefix values
    cout << "prefix_sum[0] = " << arr[0] << endl;
    cout << "prefix_sum[1,000,000] = " << arr[1000000] << endl;
    cout << "prefix_sum[10,000,000] = " << arr[10000000] << endl;
    cout << "prefix_sum[100,000,000] = " << arr[100000000] << endl;
    cout << "prefix_sum[200,000,000] = " << arr[200000000] << endl;
    cout << "prefix_sum[300,000,000] = " << arr[300000000] << endl;
    cout << "prefix_sum[400,000,000] = " << arr[400000000] << endl;
    cout << "prefix_sum[500,000,016] = " << arr[500000016] << endl;

    cout << "++++ time taken (sec) = " << runtime << endl;

    delete[] arr;
    delete[] block_prefix;
    delete[] threads;
    delete[] thread_data;
    delete[] sem_stage2;

    return 0;
}