# Test-Case Results Log

**Project:** University Student Service & Queue System  
**Course:** CC105 – Data Structures with Laboratory  
**Section:** 2.4 BSIT  
**Contributor:** Renedel B. Amores – Benchmarking and Documentation

## Functional Test Results

The statuses below are based on the tester's reported outcomes during manual testing. Only the Add/Display test was partially supported by a screenshot shared during testing; the other outcomes were reported verbally and should be accompanied by screenshots if required by the instructor.

| Test ID | Function | Test Input / Procedure | Expected Result | Reported Result | Status |
|---|---|---|---|---|---|
| TC-01 | Add and display student | Enter ID `2026-101`, name `Maria Santos`, service `Enrollment`, priority `0`; display records | Full ID and details are stored and displayed correctly | Record was added and displayed, but ID appeared as `2026` rather than `2026-101` | **Partial pass / input-format issue** |
| TC-02 | Search student | Search for student ID `2026` | Matching student record is displayed | User reported successful completion | **Reported pass** |
| TC-03 | Update student | Change student name from `Maria Santos` to `Maria Cruz`, then display records | Updated name appears | User reported successful completion | **Reported pass** |
| TC-04 | Delete student | Delete ID `2026`, then display records | Record no longer appears | User reported successful completion | **Reported pass** |
| TC-05 | Queue management | Enqueue a student and display the relevant queue | Student appears in the appropriate queue | User reported successful completion | **Reported pass** |
| TC-06 | Undo | Add student ID `3001`, perform Undo, then display records | Recently added record is removed | User reported successful completion | **Reported pass** |

## Observed Issue

**ID-01 — Hyphenated Student ID input:** The input `2026-101` appeared as `2026` in the student list. The student ID field may be numeric rather than a string, or input parsing may stop at the hyphen. Confirm the code's expected ID type and either require numeric-only IDs or change the input/data model to support hyphenated IDs. Retest after correction.

## Benchmark Test Evidence

For random input of 1,000 records, observed execution times were:

| Algorithm | Comparisons | Swaps / Moves | Execution Time |
|---|---:|---:|---:|
| Bubble Sort | 497,154 | 248,620 | 13.644 ms |
| Selection Sort | 499,500 | 995 | 1.280 ms |
| Insertion Sort | 251,913 | 250,919 | 4.887 ms |

These are individual observed runs, not averages of repeated trials. See the benchmark CSV and chart for comparison.

## Test Evidence Notes

- Retain actual program screenshots as supporting evidence.
- Recheck the Student ID input-format issue before declaring all tests fully passed.
- Confirm priority ordering with a separate test that enqueues regular and priority students together.
