# Embedded Systems & Software Development Projects

This repository contains five software projects demonstrating concepts such as **embedded simulation, shell scripting, data structures, dynamic memory, function pointers, file handling, and multi-threading**.

---

## 📌 Project 1: Smart Traffic Control and Monitoring System (C Simulation)

### Overview
This project simulates a **traffic control system** using the C programming language.  
It models traffic behavior using data structures, dynamic memory, timers, and simulated concurrency.

### Key Functionalities

1. **Traffic Signal Simulation**
   - Two intersections.
   - Each intersection uses:
     - Red / Yellow / Green states
     - Timers for each phase
   - Dynamic adjustment of signal timing based on traffic density

2. **Vehicle Detection Simulation**
   - Vehicle presence simulated using:
     - Randomized vehicle arrival generator
   - Vehicle counts stored using dynamic structures

3. **Dynamic Memory Management**
   - Each lane maintains a dynamic queue of vehicles using:
     - `malloc()`
     - `realloc()`
     - `free()`
   - Safe memory handling with null-checking

4. **Simulated Concurrency**
   - Example tasks:
     - Traffic light control
     - Vehicle addition/removal
     - Data logging
   - Allows “parallel” simulation inside a single-threaded C program

5. **Logging & File Handling**
   - Logs traffic data to a file:
     - Lane ID
     - Number of vehicles
     - Signal state
     - Timestamp
   - Appends each cycle for persistence

6. **Error Handling**
   - Protects against:
     - Memory leaks
     - Invalid pointer use
     - Failed allocations

7. **User Interface**
   - Interactive text menu:
     - Show current signal states
     - Display traffic statistics
     - Trigger manual override (emergency mode)
     - Export logs

---

## 📌 Project 2: Automated System Monitoring Shell Script

### Overview
A Linux shell script that monitors system resources and provides an interactive menu to view system status, set alert thresholds, and manage logs.

### Key Features

- **Resource Monitoring**
  - CPU, memory, disk, running processes
  - Uses `top`, `free`, `df`, `ps`
  
- **Automation & Alerts**
  - User-defined thresholds
  - Auto-logging of warnings

- **Logging**
  - All data timestamped and saved to a log file
  - Options to view or clear logs

- **Interactive Menu**
  - View system status
  - Set thresholds
  - View logs
  - Clear logs
  - Exit

- **Robustness**
  - Graceful handling of invalid inputs
  - Missing command detection

**Technologies:** Bash scripting, Cron or loop-based scheduling

---

## 📌 Project 3: Student Management System using Structures (C)

### Overview
A complete student information system using structures, dynamic memory, file handling, searching, sorting, and statistical analysis.

### Key Features

- **Dynamic Data Structures**
  - Student records: ID, name, age, course, grades[], GPA
  - Uses `malloc`, `realloc`, `free`

- **Persistent Storage**
  - Save/load records using text or binary files
  - File integrity and error handling

- **CRUD Operations**
  - Add, display, update, delete student records
  - All changes synced to file

- **Searching & Sorting**
  - Search by ID or name (linear / binary search)

- **Analytics**
  - GPA statistics: average, median, highest, lowest

- **User Interface**
  - Menu-driven CLI system

**Technologies:** C programming, Structures & memory management, File I/O

---

## 📌 Project 4: Dynamic Math & Data Processing Engine (C)

### Overview
A modular C engine using **function pointers** for dynamic runtime operation selection. Supports dataset modification and various math/data operations.

### Key Features

- **Dynamic Function Dispatch**
  - Operations selected via function pointers:
    - Sum
    - Average
    - Min / Max
    - Search
    - Ascending / descending sort

- **Dynamic Memory**
  - Dataset grows/shrinks at runtime using `malloc`, `realloc`, `free`

- **Interactive Menu**
  - Add/edit data
  - Choose operations
  - View results

- **File Integration**
  - Load dataset from file
  - Save processed results

- **Robustness**
  - Handles invalid inputs
  - Protects against empty datasets
  - Guaranteed memory cleanup

**Technologies:** C programming, Function pointers, File handling

---

## 📌 Project 5: Multi-threaded Web Scraper (C with pthreads)

### Overview
A simple but powerful multi-threaded web scraper that downloads multiple webpages in parallel using **POSIX threads**.

### Key Features

- **POSIX Threads**
  - Each URL is fetched in its own thread
  - No synchronization required

- **Content Saving**
  - Each thread stores HTML content in its own output file

- **Error Handling**
  - Unreachable URL detection
  - Failed request handling
