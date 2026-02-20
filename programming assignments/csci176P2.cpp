/*
 * CSCI 176 - Parallel Processing
 * Program 2: Parallel Global Sum using Pthreads
 * Language: C++ 20
 * Environment: CLion (macOS)
 *
 * Author: Noah Wiley
 *
 * Description:
 *  This program computes the global sum of a large 1-D array
 *  (size of 500,000,000) using shared-memory parallelism with
 *  POSIX threads (Pthreads)
 *
 *  The array is initialized such that:
 *      arr[i] = i + 1
 *
 *  The total work is evenly divided among Pthreads.
 *  Each thread:
 *      - Computes a partial sum
 *      - Prints its assigned range and partial sum
 *      - Updates the global sum using mutex protection
 *
 *  Two mutexes are used:
 *      mutex1 -> protects global_sum updates
 *      mutex2 -> protects cout output
 *
 * Compilation:
 *      g++ -std=c++20 -pthread csci176P2.cpp -o csci176P2
 *
 * Execution:
 *      ./csci176P2 <number_of_threads>
 *
 * Example:
 *      ./csci176P2 8
 */

#include <iostream>
#include <pthread.h>
#include <sys/time.h>
#include <vector>

using namespace std;

// Globals (All accessible to all threads

int thread_count;
pthread_mutex_t mutex1;
pthread_mutex_t mutex2;
int arr[500000000];
double global_sum;
void *Slave(void* rank);

// Macro (in-line expansion) for GET_TIME(double)
#define GET_TIME(now)\
    { struct timeval t; gettimeofday(&t, NULL);\
      now = t.tv_sec + t.tv_usec/1000000.0; }

// Main
int main(int argc, char* argv[]) {
    double start1, stop1;
    double time1;
    long thread_id;
    thread_count = atoi(argv[1]);

    vector<pthread_t> myThreads(thread_count);
    pthread_mutex_init(&mutex1, NULL);
    pthread_mutex_init(&mutex2, NULL);
    for (int i=0; i<500000000; i++) {
        arr[i] = i;
    }

    GET_TIME(start1);
    for (thread_id = 0; thread_id < thread_count; thread_id++)
        pthread_create(&myThreads[thread_id], NULL, Slave, (void*)thread_id);

        for (thread_id = 0; thread_id < thread_count; thread_id++)
            pthread_join(myThreads[thread_id], NULL);

        pthread_mutex_lock(&mutex1);
        GET_TIME(stop1);

    cout << "The sum is " << global_sum << endl;
    time1 = (stop1 - start1);

    cout << "Total Execution Time" << time1 << endl;
    pthread_mutex_unlock(&mutex1);
    return 0;
}

// Slave Function
void *Slave(void* rank) {
    double partial_sum = 0;

    int my_rank = (long)rank;
    int quotient = 500000000/thread_count;
    int remainder = 500000000%thread_count;
    int count, my_first_i, my_last_i;

    if (my_rank < remainder) {
        count = quotient + 1;
        my_first_i = my_rank * count;
    } else {
        count = quotient;
        my_first_i = my_rank * count - 1;
    }
    my_last_i = my_first_i + count;

    for (int i = my_first_i; i < my_last_i; i++) {
        partial_sum += arr[i];
    }

    pthread_mutex_lock(&mutex2);
    cout << "Thread_id" << my_rank << "\n start index " << my_first_i << "\n end index " << my_last_i << "\nPartial sum " << partial_sum << "\n\n-------------"<<endl;
    pthread_mutex_unlock(&mutex2);

    pthread_mutex_lock(&mutex1);
    global_sum += partial_sum;
    pthread_mutex_unlock(&mutex1);

    return NULL;
}