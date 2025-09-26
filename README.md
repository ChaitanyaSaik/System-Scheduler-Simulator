# System Scheduler Simulation

## Project Overview

The **System Scheduler Simulation** is a C++ project that simulates multiple CPU scheduling algorithms used in operating systems. It includes both **preemptive and non-preemptive scheduling**, provides detailed metrics, and visualizes execution with a **text-based Gantt chart**.

### Supported Scheduling Algorithms

* **First-Come, First-Served (FCFS)**
* **Shortest Job First (SJF) - Preemptive (SRTF)**
* **Priority Scheduling - Preemptive**
* **Round Robin (RR)**

### Key Features

* Multi-threaded simulation to mimic process execution.
* Preemptive and non-preemptive scheduling support.
* Text-based Gantt chart visualization of CPU execution.
* Calculates metrics: **Waiting Time (WT), Turnaround Time (TAT), CPU Utilization**.
* Input validation and error handling for all user inputs.
* Configurable input for arrival time, burst time, priority, and time quantum for RR.

### Limitations Addressed

* CPU idle times are visualized in Gantt chart.
* Processes with shorter burst time or higher priority can preempt the current process.
* Dynamic arrival of processes is handled correctly.

### Future Enhancements (Optional)

* Multi-core CPU simulation for parallel process execution.
* Graphical GUI for Gantt chart visualization.
* Throughput calculation (processes per unit time).

## Project Structure

```
SystemSchedulerSimulator/
│
├─ src/
│   └─ SystemSchedulerSimulator.cpp  # Main C++ source code
├─ README.md                         # Project documentation
└─ screenshots/                      # Optional: Output or Gantt chart images
```

## How to Compile and Run

1. Open a terminal and navigate to the `src` directory.
2. Compile the C++ code with threading support:

```
g++ SystemSchedulerSimulator.cpp -o scheduler -std=c++11 -pthread
```

3. Run the executable:

```
./scheduler
```

## Sample Input

```
Enter number of processes: 4
Process 1: Arrival=0, Burst=8, Priority=2
Process 2: Arrival=1, Burst=4, Priority=1
Process 3: Arrival=2, Burst=2, Priority=3
Process 4: Arrival=3, Burst=1, Priority=4
Time Quantum for Round Robin: 2
```

## Sample Output (Excerpt)

```
=== FCFS Scheduling ===
Process 1 executing for 8 unit(s) starting at time 0
Process 2 executing for 4 unit(s) starting at time 8
...
Gantt Chart: P1 P1 P1 P1 P1 P1 P1 P1 P2 P2 P2 P2 P3 P3 P4
Avg WT=7, Avg TAT=10.75, CPU Utilization=100%
```

## Highlights for Resume / Interview

* Demonstrates strong understanding of **OS scheduling concepts**.
* Implements **multi-threading in C++**.
* Includes **preemptive & non-preemptive algorithms**.
* Shows ability to handle **metrics calculation and visualization**.
* Fully functional and professional code ready for **technical interviews**.

---

**Author:** Chaitanya Sai Kurapati
<br>
**Language:** C++
<br>
**Date:** Jun-2024
