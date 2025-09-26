#include <iostream>
#include <vector>
#include <queue>
#include <thread>
#include <mutex>
#include <algorithm>
#include <chrono>
#include <iomanip>
#include <climits>

std::mutex mtx; // For synchronized console output

struct Process {
    int pid;
    int arrivalTime;
    int burstTime;
    int remainingTime;
    int priority;
    int startTime;
    int completionTime;
};

void executeProcess(Process &p, int execTime) {
    std::lock_guard<std::mutex> lock(mtx);
    std::cout << "Process " << p.pid << " executing for " << execTime
              << " unit(s) starting at time " << p.startTime << "\n";
    std::this_thread::sleep_for(std::chrono::milliseconds(execTime * 100));
    p.remainingTime -= execTime;
}

void resetProcesses(std::vector<Process> &procs) {
    for (auto &p : procs) {
        p.startTime = 0;
        p.completionTime = 0;
        p.remainingTime = p.burstTime;
    }
}

// FCFS - Non-preemptive
void FCFS(std::vector<Process> processes) {
    std::cout << "\n=== FCFS Scheduling ===\n";
    int currentTime = 0;
    std::vector<std::thread> threads;
    std::vector<int> gantt;

    for (auto &p : processes) {
        if (currentTime < p.arrivalTime)
            currentTime = p.arrivalTime;
        p.startTime = currentTime;
        threads.emplace_back(executeProcess, std::ref(p), p.burstTime);
        for (int i = 0; i < p.burstTime; i++) gantt.push_back(p.pid);
        currentTime += p.burstTime;
        p.completionTime = currentTime;
    }

    for (auto &t : threads) t.join();

    // Metrics
    double totalWT = 0, totalTAT = 0;
    int totalBurst = 0;
    for (auto &p : processes) {
        int TAT = p.completionTime - p.arrivalTime;
        int WT = TAT - p.burstTime;
        totalWT += WT; totalTAT += TAT; totalBurst += p.burstTime;
        std::cout << "Process " << p.pid << ": WT=" << WT << ", TAT=" << TAT << "\n";
    }
    std::cout << "Avg WT=" << totalWT / processes.size()
              << ", Avg TAT=" << totalTAT / processes.size()
              << ", CPU Utilization=" << (double)totalBurst / currentTime * 100 << "%\n";

    // Gantt Chart
    std::cout << "Gantt Chart: ";
    for (auto pid : gantt) std::cout << "P" << pid << " ";
    std::cout << "\n";
}

// Preemptive SJF (Shortest Remaining Time First)
void SRTF(std::vector<Process> processes) {
    std::cout << "\n=== SJF Preemptive (SRTF) ===\n";
    int currentTime = 0, completed = 0, n = processes.size();
    std::vector<int> gantt;
    std::vector<bool> finished(n, false);

    while (completed < n) {
        int idx = -1, minRemaining = INT_MAX;
        for (int i = 0; i < n; i++) {
            if (!finished[i] && processes[i].arrivalTime <= currentTime && processes[i].remainingTime < minRemaining) {
                minRemaining = processes[i].remainingTime;
                idx = i;
            }
        }
        if (idx == -1) { // CPU idle
            gantt.push_back(0);
            currentTime++;
            continue;
        }

        processes[idx].startTime = currentTime;
        executeProcess(processes[idx], 1);
        gantt.push_back(processes[idx].pid);
        currentTime++;

        if (processes[idx].remainingTime == 0) {
            processes[idx].completionTime = currentTime;
            finished[idx] = true;
            completed++;
        }
    }

    // Metrics
    double totalWT = 0, totalTAT = 0; int totalBurst = 0;
    for (auto &p : processes) {
        int TAT = p.completionTime - p.arrivalTime;
        int WT = TAT - p.burstTime;
        totalWT += WT; totalTAT += TAT; totalBurst += p.burstTime;
        std::cout << "Process " << p.pid << ": WT=" << WT << ", TAT=" << TAT << "\n";
    }
    std::cout << "Avg WT=" << totalWT / n << ", Avg TAT=" << totalTAT / n
              << ", CPU Utilization=" << (double)totalBurst / currentTime * 100 << "%\n";

    // Gantt Chart
    std::cout << "Gantt Chart: ";
    for (auto pid : gantt) {
        if (pid == 0) std::cout << "Idle ";
        else std::cout << "P" << pid << " ";
    }
    std::cout << "\n";
}

// Preemptive Priority Scheduling
void PreemptivePriority(std::vector<Process> processes) {
    std::cout << "\n=== Priority Scheduling (Preemptive) ===\n";
    int currentTime = 0, completed = 0, n = processes.size();
    std::vector<int> gantt; std::vector<bool> finished(n, false);

    while (completed < n) {
        int idx = -1, highestPriority = INT_MAX;
        for (int i = 0; i < n; i++) {
            if (!finished[i] && processes[i].arrivalTime <= currentTime && processes[i].remainingTime > 0) {
                if (processes[i].priority < highestPriority) {
                    highestPriority = processes[i].priority;
                    idx = i;
                }
            }
        }
        if (idx == -1) {
            gantt.push_back(0);
            currentTime++; continue;
        }

        processes[idx].startTime = currentTime;
        executeProcess(processes[idx], 1);
        gantt.push_back(processes[idx].pid);
        currentTime++;

        if (processes[idx].remainingTime == 0) {
            processes[idx].completionTime = currentTime;
            finished[idx] = true;
            completed++;
        }
    }

    // Metrics
    double totalWT = 0, totalTAT = 0; int totalBurst = 0;
    for (auto &p : processes) {
        int TAT = p.completionTime - p.arrivalTime;
        int WT = TAT - p.burstTime;
        totalWT += WT; totalTAT += TAT; totalBurst += p.burstTime;
        std::cout << "Process " << p.pid << ": WT=" << WT << ", TAT=" << TAT << "\n";
    }
    std::cout << "Avg WT=" << totalWT / n << ", Avg TAT=" << totalTAT / n
              << ", CPU Utilization=" << (double)totalBurst / currentTime * 100 << "%\n";

    std::cout << "Gantt Chart: ";
    for (auto pid : gantt) {
        if (pid == 0) std::cout << "Idle ";
        else std::cout << "P" << pid << " ";
    }
    std::cout << "\n";
}

// Round Robin
void RoundRobin(std::vector<Process> processes, int tq) {
    std::cout << "\n=== Round Robin Scheduling ===\n";
    int currentTime = 0, completed = 0, n = processes.size();
    std::queue<int> q; std::vector<int> gantt; std::vector<bool> inQueue(n, false);

    while (completed < n) {
        for (int i = 0; i < n; i++)
            if (!inQueue[i] && processes[i].arrivalTime <= currentTime && processes[i].remainingTime > 0)
                q.push(i), inQueue[i] = true;

        if (q.empty()) { gantt.push_back(0); currentTime++; continue; }

        int idx = q.front(); q.pop();
        int exec = std::min(tq, processes[idx].remainingTime);
        processes[idx].startTime = currentTime;
        executeProcess(processes[idx], exec);

        for (int i = 0; i < exec; i++) gantt.push_back(processes[idx].pid);
        currentTime += exec;

        if (processes[idx].remainingTime > 0) q.push(idx);
        else { processes[idx].completionTime = currentTime; completed++; }
    }

    // Metrics
    double totalWT = 0, totalTAT = 0; int totalBurst = 0;
    for (auto &p : processes) {
        int TAT = p.completionTime - p.arrivalTime;
        int WT = TAT - p.burstTime;
        totalWT += WT; totalTAT += TAT; totalBurst += p.burstTime;
        std::cout << "Process " << p.pid << ": WT=" << WT << ", TAT=" << TAT << "\n";
    }
    std::cout << "Avg WT=" << totalWT / n << ", Avg TAT=" << totalTAT / n
              << ", CPU Utilization=" << (double)totalBurst / currentTime * 100 << "%\n";

    std::cout << "Gantt Chart: ";
    for (auto pid : gantt) { if (pid == 0) std::cout << "Idle "; else std::cout << "P" << pid << " "; }
    std::cout << "\n";
}

// Input validation
int getPositiveInt(const std::string &msg) {
    int x;
    while (true) {
        std::cout << msg;
        if (std::cin >> x && x >= 0) break;
        std::cout << "Invalid input! Enter non-negative integer.\n";
        std::cin.clear(); std::cin.ignore(10000, '\n');
    }
    return x;
}

int main() {
    int n = getPositiveInt("Enter number of processes: ");
    std::vector<Process> processes(n);
    for (int i = 0; i < n; i++) {
        processes[i].pid = i+1;
        processes[i].arrivalTime = getPositiveInt("Enter arrival time for process " + std::to_string(i+1) + ": ");
        processes[i].burstTime = getPositiveInt("Enter burst time for process " + std::to_string(i+1) + ": ");
        processes[i].priority = getPositiveInt("Enter priority for process " + std::to_string(i+1) + " (lower number = higher priority): ");
        processes[i].remainingTime = processes[i].burstTime;
    }

    FCFS(processes); resetProcesses(processes);
    SRTF(processes); resetProcesses(processes);
    PreemptivePriority(processes); resetProcesses(processes);

    int tq = getPositiveInt("Enter time quantum for Round Robin: ");
    RoundRobin(processes, tq);

    return 0;
}
