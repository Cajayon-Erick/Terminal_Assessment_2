# System Technical Documentation

**Project:** University Student Service & Queue System  
**Course:** CC105 – Data Structure with Laboratory  
**Assessment:** Terminal Assessment 2 / Milestone 2 — Dynamic Data Engine & Benchmarking Suite  
**Institution:** National Teachers College  
**Section:** 2.4 BSIT  
**Document focus:** System design, implementation, testing and benchmarking  
**Repository URL:** [Insert your actual GitHub repository URL]

## 1. Project Overview

The University Student Service & Queue System is a menu-driven C++ console application that manages student service records and queues. It demonstrates dynamic data structures, record management, searching, sorting, and algorithm benchmarking in a university service-office scenario.

### Objectives

1. Create, display, search, update, and delete student records.
2. Organize records in a dynamically allocated doubly linked list.
3. Track record changes using a linked stack and provide undo functionality.
4. Manage regular requests with a circular queue and priority requests with a linked priority queue.
5. Compare searching and sorting approaches and measure sorting performance using generated datasets.

## 2. Technology and Execution

- Language: C++ (single `.cpp` implementation).
- Interface: console / terminal, with numbered menus.
- Compiler: a C++ compiler supporting the features used by the uploaded source. For the reviewed original source, C++11 or later is sufficient; a newer source version may specify C++17.
- Development environment: Code::Blocks or compatible C++ toolchain.
- Data persistence: in-memory during a program run; no database or file persistence is implemented in the reviewed source.

**Example build command:**

```bash
g++ -std=c++17 -O2 -Wall -Wextra Student_Service_Queue_System.cpp -o student_service
```

Run the executable and follow the on-screen menu. Actual filenames and compiler settings should match the source committed to GitHub.

## 3. Functional Modules

| Module | Implementation | Main responsibility |
|---|---|---|
| Student record registry | `StudentList` doubly linked list | Add, display, find, update and remove records |
| Audit history / undo | `AuditStack` linked stack | Record actions and reverse the most recent supported change |
| Regular service queue | `CircularQueue` (capacity 100) | FIFO enqueue, dequeue and display |
| Priority service queue | `PriorityQueue` linked nodes | Enqueue by priority, dequeue and display |
| Search | Student-list lookup; linear/binary search routines in search menu | Locate records by student ID |
| Sorting | Bubble, Selection and Insertion Sort | Sort student data by ID |
| Benchmarking | `benchmarkMenu` and `Metrics` | Generate test datasets; report comparisons, swaps/moves and elapsed milliseconds |

## 4. Data Model and Architecture

A `Student` stores:

- `id` — integer student ID;
- `name` — student name;
- `serviceType` — requested service;
- `priority` — integer flag (`0` = regular; `1` = priority).

`ListNode` stores one `Student` with `prev` and `next` pointers. `StackNode` stores an `AuditAction` with a `next` pointer. `QueueNode` stores one `Student` with a `next` pointer.

**High-level data flow:** User → Main Menu → Record/Queue/Search/Sort/Benchmark Module → Console Output. Record updates also create audit actions. Queue entries are copied from existing student records, not dynamically synchronized with later record changes.

The repository's `Software_Architecture_Diagram.png` and `Program_Flowchart.png` should be used as visual companions to this section.

## 5. Key Workflows

**Record creation:** Choose Add Student → enter student details → check for duplicate ID → append to linked list → push an ADD audit action → display result.

**Record update/delete:** Find record by ID → modify or remove record → push an UPDATE/DELETE audit action → display result.

**Undo:** Pop the most recent audit action → apply the inverse operation if supported → display outcome. Deletion undo may restore the record at a different list position.

**Queue:** Select an existing student ID → place a copy in regular or priority queue according to the student's priority flag → display or process queue.

**Benchmark:** Choose dataset size → choose random/sorted/reverse profile → choose Bubble/Selection/Insertion Sort → generate data → execute sort → report operation counts and elapsed time.

## 6. Algorithm Complexity

| Algorithm or operation | Best | Average | Worst |
|---|---|---|---|
| Linear search | O(1) | O(n) | O(n) |
| Binary search on already sorted data | O(1) | O(log n) | O(log n) |
| Bubble Sort with early exit | O(n) | O(n²) | O(n²) |
| Selection Sort | O(n²) | O(n²) | O(n²) |
| Insertion Sort | O(n) | O(n²) | O(n²) |
| Linked-list search | O(1) | O(n) | O(n) |
| Stack push / pop | O(1) | O(1) | O(1) |
| Circular queue enqueue / dequeue | O(1) | O(1) | O(1) |
| Linked priority queue enqueue | O(1) in some cases | O(n) | O(n) |
| Linked priority queue dequeue | O(1) | O(1) | O(1) |

**Caveats:** Binary search assumes sorted input; creating the sorted data may add time. Linked-list insertion/deletion is O(1) only once the relevant position/node is known; locating it may cost O(n). Memory use depends on stored records and queue/stack entries.

## 7. Benchmark Method and Observed Results

The reviewed program offers dataset sizes **100, 1,000, 10,000 and 50,000**, three data profiles (**random**, **already sorted**, **reverse sorted**) and three sorting algorithms. Its `Metrics` structure counts comparisons and swaps/moves; the clock reports elapsed milliseconds.

**Observed user-provided console runs: Random data, n = 1,000**

| Algorithm | Comparisons | Swaps/moves | Execution time |
|---|---:|---:|---:|
| Bubble Sort | 497,154 | 248,620 | 13.644 ms |
| Selection Sort | 499,500 | 995 | 1.280 ms |
| Insertion Sort | 251,913 | 250,919 | 4.887 ms |

In these single observed runs, Selection Sort had the lowest measured execution time. These are not controlled averages: execution times vary by machine, compiler settings, workload and run-to-run conditions. See `CC105_Benchmark_Results_Actual.csv` and `CC105_Benchmark_Comparison_Chart.png` in the repository, if uploaded.

## 8. Testing and Quality Assurance

Planned / user-reported functional test cases include:

1. Add and display a student record.
2. Search by student ID.
3. Update a student record.
4. Delete a student record.
5. Enqueue and display a student in the correct queue.
6. Undo a recent record change.
7. Run sorting and benchmarking functions.

The user reported completion of the guided functional tests, but not every test has an independently inspected screenshot. The test log should distinguish **reported PASS**, **visually verified**, and **not yet verified**. Refer to `CC105_Test_Case_Results_Log.md` for the recorded results.

## 9. Known Issues and Limitations

- **Student ID input:** The original `Student` structure uses an integer ID. An attempted ID such as `2026-101` cannot be represented as an integer student ID. Use numeric IDs unless the program is intentionally redesigned to support strings.
- **Priority ordering:** The reviewed original `PriorityQueue::enqueue` condition orders lower priority flag values first, despite the display label stating priority `1` first. If the committed source is still the original version, this needs correction and retesting; do not mark it fixed without verification.
- **Undo edge cases:** Undoing a deletion may change the record's original position. Additional retesting is recommended for updates that change IDs.
- **No persistent storage:** Records disappear when the program exits.
- **Benchmark duration:** Quadratic sorts on large datasets can take a long time.

See `CC105_Bug_and_Debugging_Log.md` for findings and suggested fixes.

## 10. Diagrams, Evidence and Submission

Suggested repository artifacts:

- `Student_Service_Queue_System.cpp`
- `README.md`
- `Software_Architecture_Diagram.png`
- `Program_Flowchart.png`
- `CC105_Benchmark_Results_Actual.csv`
- `CC105_Benchmark_Comparison_Chart.png`
- `BIG_O_ANALYSIS.md`
- `CC105_Test_Case_Results_Log.md`
- `CC105_Bug_and_Debugging_Log.md`
- `CC105_System_Technical_Documentation.md` (this file)
- `CC105_Individual_Contribution_Matrix.md`
- Actual program screenshots and presentation materials

**Before final submission:** Verify all GitHub uploads, confirm the source version, include the repository URL and latest commit hash in worksheet Page 15, fill Page 14 with the group's agreed contribution percentages, and prepare the practical demonstration.
