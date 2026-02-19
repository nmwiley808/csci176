# CSCI 176 – Parallel Processing  
**California State University, Fresno**  
**Student: Noah Wiley**

---

## Repository Overview

This repository contains my coursework, programming assignments, in-class activities, and projects for **CSCI 176 – Parallel Processing** at Fresno State.

The purpose of this repository is to document my progress, experiments, and implementations throughout the semester, focusing on shared-memory and distributed-memory parallel computing.

---

## Course Description

CSCI 176 introduces the fundamental principles of parallel computing, including:

- Parallel architectures (SIMD, MIMD, multicore systems)  
- Shared-memory programming  
- Distributed-memory programming  
- Thread synchronization and concurrency  
- Performance metrics (speedup, efficiency, scalability)  
- Amdahl’s Law and scalability analysis  
- Load balancing techniques  
- Parallel algorithm design  

The course emphasizes both theoretical understanding and hands-on implementation using modern parallel programming APIs.

---

## Repository Structure

```
CSCI176/
│
├── homework_assignments/              # Homework Assignments
├── programming_assignments/           # Programming Assignments
├── project/                           # Term Project
├── CSCI176_Textbook.pdf               # Course Textbook
└── README.md                          # Repository Documentation
```

---

## Technologies & Tools

This repository may include:

- C / C++
- MPI (Message Passing Interface)
- POSIX Threads (Pthreads)
- OpenMP
- Linux-based development tools (gcc, g++, mpicc)
- CLion IDE

---

## Skills Developed

Throughout this course, I am developing skills in:

- Designing parallel algorithms  
- Implementing shared-memory programs  
- Implementing distributed-memory programs  
- Managing thread synchronization and preventing race conditions  
- Benchmarking execution time and performance  
- Calculating speedup and efficiency  
- Evaluating scalability of parallel systems  

---

## How to Run Programs

Most assignments are compiled and executed in a Linux-based environment.

To compile C++ programs:

```
g++ -std=c++20 program.cpp -o program
./program
```

For MPI programs:

```
mpic++ program.cpp -o program
mpirun -np 4 ./program
```

---

## Notes

- This repository is for academic coursework.
- Code is written for learning and experimentation.
- Some large output files or logs may not be included due to size limitations.

---

## Academic Integrity

All work in this repository is my own unless otherwise stated.  
This repository is for portfolio and academic documentation purposes only.

