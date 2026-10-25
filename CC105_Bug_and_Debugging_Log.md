# Bug & Debugging Log

**Project:** University Student Service & Queue System  
**Course:** CC105 – Data Structures with Laboratory  
**Section:** 2.4 BSIT  
**Documentation contributor:** Renedel B. Amores  
**Related worksheet:** Page 13 (Bug & Debugging Log), Page 15 (Deliverables Checklist)

## Status legend
- **Observed:** The issue appeared in a manual test or screenshot.
- **Code-review finding:** The problem is supported by source inspection, but its effect has not yet been reproduced in a program run.
- **Open:** A fix and successful retest have not been verified.

## Findings

| Bug ID | Area | Evidence | Expected behavior | Actual behavior / finding | Likely root cause | Status |
|---|---|---|---|---|---|---|
| BUG-01 | Student ID input | **Observed** during Add/Display test | A student ID is either accepted in full or rejected with a clear validation message | Entering `2026-101` resulted in displayed ID `2026` | `Student.id` is an `int`; `readInt()` uses `cin >> value`, which accepts the numeric prefix before `-` and then discards the rest of the line | **Open — not verified fixed** |
| BUG-02 | Priority queue ordering | **Code-review finding** in the submitted C++ source | Higher-priority records (`priority = 1`) should appear before regular records (`priority = 0`) if both are placed in the same priority queue | `PriorityQueue::enqueue()` compares priorities in ascending order (`<` and `<=`), which would put `0` ahead of `1` | Comparison direction conflicts with the stated priority convention | **Open — correction/retest not verified** |

## BUG-01 — Student ID input is silently truncated

**Steps to reproduce**
1. Run the program and choose **Add Student Record**.
2. Enter `2026-101` at the Student ID prompt.
3. Complete the remaining fields and display the student records.
4. Observe that the ID appears as `2026` instead of the complete input.

**Code inspection:** The original source declares `Student.id` as `int` and uses `readInt("Student ID (1-999999): ", ...)`. The `readInt()` function reads with `cin >> value` and ignores the remainder of the line after accepting a valid numeric prefix.

**Suggested resolution:** Decide and document the intended ID format. If IDs must be numeric, reject input containing extra characters (read the whole line, validate it, and parse the entire string). If hyphenated IDs are required, change the ID type to `string` consistently across record management, searches, queues, and audit actions.

**Retest:** Try `2026-101`, `2026`, and an invalid alphabetic ID. Confirm that input is accepted completely or rejected clearly, without silently shortening it. **Retest pending.**

## BUG-02 — Priority queue comparisons are reversed

**Code inspection:** In the original `PriorityQueue::enqueue()` method:

```cpp
if (frontNode == nullptr || s.priority < frontNode->data.priority) {
    // insert at front
}
// ...
while (p->next != nullptr && p->next->data.priority <= s.priority)
    p = p->next;
```

The display method describes the queue as `priority 1 first`, but the comparisons above sort in ascending order. In the current user-facing queue menu, records with `priority = 0` are routed to a separate circular regular queue; therefore, the mixed-priority case may not occur through the normal menu. This remains a source-code inconsistency to address rather than a confirmed user-visible failure.

**Suggested resolution:** If the linked priority queue is intended to support mixed priorities with `1` first, use descending comparisons:

```cpp
if (frontNode == nullptr || s.priority > frontNode->data.priority) {
    node->next = frontNode;
    frontNode = node;
    return;
}
QueueNode* p = frontNode;
while (p->next != nullptr && p->next->data.priority >= s.priority)
    p = p->next;
node->next = p->next;
p->next = node;
```

**Retest:** In a unit test, enqueue IDs `100` (priority `0`), `101` (priority `1`), and `102` (priority `1`) into the *same* priority queue. Verify dequeue order `101`, `102`, `100` (FIFO among equal priorities). **Retest pending.**

## Debugging activity record

| Activity | Outcome |
|---|---|
| Manual Add/Display test with hyphenated ID | Observed and documented the shortened ID |
| Inspected `Student` structure and `readInt()` | Identified integer parsing and ignored trailing characters as the likely cause |
| Inspected `PriorityQueue::enqueue()` and its display label | Identified a mismatch between comparison direction and `priority 1 first` |
| Applied fixes to the GitHub source | **Not confirmed** |
| Reran regression tests after fixes | **Not confirmed** |

## Verification notes

The functional tests for search, update, delete, queue operations, and undo were reported as completed by the tester, but screenshots or detailed outputs were not provided for every case. They should not be presented as independently verified automated tests. This log records **two findings**, not two verified fixes.

**Recommended next action:** Confirm the exact source version uploaded to GitHub, correct the issues as appropriate, rerun the relevant tests, and update each bug's status with the fix commit and test evidence.
