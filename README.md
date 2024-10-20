
# Kernel Simulator

This project is a kernel simulator designed to model and analyze the performance of the **First-Come, First-Served (FCFS)** scheduling algorithm. The simulator tracks process state transitions and produces output logs showing how processes move through the system.

## Project Overview

The kernel simulator reads a list of processes from an input file and simulates their execution using the **FCFS scheduling algorithm**, focusing on:
- Process state transitions (e.g., from **Ready** to **Running** or **Waiting**).
- CPU usage and I/O handling.
- Producing output logs that detail each process’s state transitions.

The simulator is implemented in C and is capable of:
- Managing multiple processes using data structures such as Process Control Blocks (PCB).
- Simulating time-based events like CPU bursts and I/O operations.
- Providing output logs detailing each process’s execution timeline using the FCFS algorithm.

## Features

- **First-Come, First-Served Scheduling**: The simulator uses FCFS, where processes are scheduled in the order they arrive without preemption.
- **Supports Multiple Test Cases**: Provided test cases simulate various process arrival times and CPU requirements.
- **Simulated CPU and I/O Scheduling**: The kernel tracks CPU usage and I/O events for each process, ensuring proper process transitions.
- **Outputs Detailed Logs**: The simulator generates logs that show each process’s state transitions, the time of transition, and the states involved.
- **Batch Scripts for Testing**: The project includes batch scripts (`test1.bat`, `test2.bat`, and `test3.bat`) to automate the testing of different process sets.

## Requirements

- **C Compiler**: You need a C compiler (e.g., GCC) to compile the source code.
- **Windows**: The provided batch files are designed for use on Windows systems. If using Linux or macOS, you'll need to adjust the scripts accordingly.

## Files Included

- **main.c**: The main source code file for the kernel simulator.
- **output_1.txt, output_2.txt, output_3.txt**: Sample output logs from running the simulator with the provided test cases.
- **test_case_1.csv, test_case_2.csv, test_case_3.csv**: Input test cases in CSV format, detailing processes with their arrival times, CPU bursts, and I/O frequencies.
- **test1.bat, test2.bat, test3.bat**: Batch scripts to automatically run the simulator with the provided test cases.

## Installation and Setup

1. **Clone or download the repository**:
   ```bash
   git clone https://github.com/SajaFawagreh/Kernel-Simulator.git
   ```

2. **Compile the program**:
   ```bash
   gcc -o KernelSimulator main.c
   ```

3. **Run the program**:
    ```bash
    test1.bat
    ```

## Usage

### Input Format
The simulator expects input files in CSV format, where each line represents a process with the following fields:
```
PID, Arrival Time, Total CPU Time, I/O Frequency, I/O Duration
```

### Example
For example, a process arriving at time 0, needing 100ms of CPU time, performing I/O every 50ms for 10ms would be represented as:
```
1, 0, 100, 50, 10
```

### Running the Tests
You can execute the predefined tests using the provided batch scripts:
- `test1.bat`: Runs the simulator with `test_case_1.csv`.
- `test2.bat`: Runs the simulator with `test_case_2.csv`.
- `test3.bat`: Runs the simulator with `test_case_3.csv`.

### Output Logs
The simulator will produce an output log in text format that shows process transitions. Each line in the log will include:
- **Time**: The simulation time in milliseconds.
- **PID**: The process ID.
- **Old State / New State**: The transition between states (e.g., from **Ready** to **Running**).

Sample output:
```
Time: 0, PID: 1, Old State: Ready, New State: Running
Time: 50, PID: 1, Old State: Running, New State: Waiting (I/O)
...
```

## License
This project is for educational purposes and part of the SYSC 4001 course. No formal license is provided.
