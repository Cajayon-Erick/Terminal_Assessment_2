#include <iostream>
#include <iomanip>
#include <string>
#include <limits>
#include <chrono>
#include <random>
#include <algorithm>

using namespace std;

struct Student {
    int id;
    string name;
    string serviceType;
    int priority;
};

struct ListNode {
    Student data;
    ListNode* prev;
    ListNode* next;

    explicit ListNode(const Student& s) : data(s), prev(nullptr), next(nullptr) {}
};

struct AuditAction {
    string type;
    Student before;
    Student after;
    int studentId;
};

struct StackNode {
    AuditAction action;
    StackNode* next;

    explicit StackNode(const AuditAction& a) : action(a), next(nullptr) {}
};

struct QueueNode {
    Student data;
    QueueNode* next;

    explicit QueueNode(const Student& s) : data(s), next(nullptr) {}
};

struct Metrics {
    long long comparisons = 0;
    long long swaps = 0;
};

class StudentList {
private:
    ListNode* head;
    ListNode* tail;

public:
    StudentList() : head(nullptr), tail(nullptr) {}
    ~StudentList() { clear(); }

    bool exists(int id) const {
        for (ListNode* p = head; p != nullptr; p = p->next)
            if (p->data.id == id) return true;
        return false;
    }

    Student* find(int id) {
        for (ListNode* p = head; p != nullptr; p = p->next)
            if (p->data.id == id) return &p->data;
        return nullptr;
    }

    bool add(const Student& s) {
        if (exists(s.id)) return false;
        ListNode* node = new ListNode(s);
        if (head == nullptr) {
            head = tail = node;
        } else {
            tail->next = node;
            node->prev = tail;
            tail = node;
        }
        return true;
    }

    bool remove(int id, Student& removed) {
        ListNode* p = head;
        while (p != nullptr && p->data.id != id) p = p->next;
        if (p == nullptr) return false;

        removed = p->data;
        if (p->prev != nullptr) p->prev->next = p->next;
        else head = p->next;

        if (p->next != nullptr) p->next->prev = p->prev;
        else tail = p->prev;

        delete p;
        return true;
    }

    bool update(int id, const Student& replacement, Student& oldData) {
        Student* found = find(id);
        if (found == nullptr) return false;
        oldData = *found;
        if (replacement.id != id && exists(replacement.id)) return false;
        *found = replacement;
        return true;
    }

    void display() const {
        if (head == nullptr) {
            cout << "No student records yet.\n";
            return;
        }
        cout << left << setw(10) << "ID" << setw(25) << "Name"
             << setw(22) << "Service" << "Priority\n";
        cout << string(68, '-') << '\n';
        for (ListNode* p = head; p != nullptr; p = p->next) {
            cout << left << setw(10) << p->data.id
                 << setw(25) << p->data.name
                 << setw(22) << p->data.serviceType
                 << (p->data.priority == 1 ? "Priority" : "Regular") << '\n';
        }
    }

    int copyToArray(Student* output, int capacity) const {
        int count = 0;
        for (ListNode* p = head; p != nullptr && count < capacity; p = p->next)
            output[count++] = p->data;
        return count;
    }

    void clear() {
        ListNode* p = head;
        while (p != nullptr) {
            ListNode* next = p->next;
            delete p;
            p = next;
        }
        head = tail = nullptr;
    }
};

class AuditStack {
private:
    StackNode* topNode;

public:
    AuditStack() : topNode(nullptr) {}
    ~AuditStack() { clear(); }

    void push(const AuditAction& action) {
        StackNode* node = new StackNode(action);
        node->next = topNode;
        topNode = node;
    }

    bool pop(AuditAction& action) {
        if (topNode == nullptr) return false;
        StackNode* old = topNode;
        action = old->action;
        topNode = old->next;
        delete old;
        return true;
    }

    void display() const {
        if (topNode == nullptr) {
            cout << "Audit stack is empty.\n";
            return;
        }
        cout << "Recent actions (newest first):\n";
        for (StackNode* p = topNode; p != nullptr; p = p->next)
            cout << "- " << p->action.type << " student ID "
                 << p->action.studentId << '\n';
    }

    void clear() {
        AuditAction ignored;
        while (pop(ignored)) {}
    }
};

class CircularQueue {
private:
    Student* items;
    int capacity;
    int frontIndex;
    int rearIndex;
    int count;

public:
    explicit CircularQueue(int cap = 100) :
        items(new Student[cap]), capacity(cap), frontIndex(0), rearIndex(-1), count(0) {}
    ~CircularQueue() { delete[] items; }

    bool enqueue(const Student& s) {
        if (count == capacity) return false;
        rearIndex = (rearIndex + 1) % capacity;
        items[rearIndex] = s;
        ++count;
        return true;
    }

    bool dequeue(Student& s) {
        if (count == 0) return false;
        s = items[frontIndex];
        frontIndex = (frontIndex + 1) % capacity;
        --count;
        return true;
    }

    void display() const {
        if (count == 0) {
            cout << "Regular queue is empty.\n";
            return;
        }
        cout << "Regular queue (front to rear):\n";
        for (int i = 0; i < count; ++i) {
            int index = (frontIndex + i) % capacity;
            cout << items[index].id << " - " << items[index].name << '\n';
        }
    }
};

class PriorityQueue {
private:
    QueueNode* frontNode;

public:
    PriorityQueue() : frontNode(nullptr) {}
    ~PriorityQueue() { clear(); }

    void enqueue(const Student& s) {
        QueueNode* node = new QueueNode(s);
        if (frontNode == nullptr || s.priority < frontNode->data.priority) {
            node->next = frontNode;
            frontNode = node;
            return;
        }
        QueueNode* p = frontNode;
        while (p->next != nullptr && p->next->data.priority <= s.priority)
            p = p->next;
        node->next = p->next;
        p->next = node;
    }

    bool dequeue(Student& s) {
        if (frontNode == nullptr) return false;
        QueueNode* old = frontNode;
        s = old->data;
        frontNode = old->next;
        delete old;
        return true;
    }

    void display() const {
        if (frontNode == nullptr) {
            cout << "Priority queue is empty.\n";
            return;
        }
        cout << "Priority queue (priority 1 first):\n";
        for (QueueNode* p = frontNode; p != nullptr; p = p->next)
            cout << p->data.id << " - " << p->data.name
                 << " (priority " << p->data.priority << ")\n";
    }

    void clear() {
        Student ignored;
        while (dequeue(ignored)) {}
    }
};

void printStudent(const Student& s) {
    cout << "ID: " << s.id << "\nName: " << s.name
         << "\nService: " << s.serviceType
         << "\nPriority: " << (s.priority == 1 ? "Priority" : "Regular") << '\n';
}

int readInt(const string& prompt, int minValue, int maxValue) {
    int value;
    while (true) {
        cout << prompt;
        if (cin >> value && value >= minValue && value <= maxValue) {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return value;
        }
        cout << "Invalid input. Enter a number from " << minValue
             << " to " << maxValue << ".\n";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
}

string readText(const string& prompt) {
    string value;
    do {
        cout << prompt;
        getline(cin, value);
        if (value.empty()) cout << "Input cannot be empty.\n";
    } while (value.empty());
    return value;
}

Student inputStudent() {
    Student s;
    s.id = readInt("Student ID (1-999999): ", 1, 999999);
    s.name = readText("Full name: ");
    s.serviceType = readText("Service request (Assessment/Clearance/Transcript): ");
    s.priority = readInt("Priority? (1 = Yes, 0 = No): ", 0, 1);
    return s;
}

bool undoLast(StudentList& records, AuditStack& audit) {
    AuditAction action;
    if (!audit.pop(action)) {
        cout << "Nothing to undo.\n";
        return false;
    }

    if (action.type == "ADD") {
        Student ignored;
        records.remove(action.studentId, ignored);
    } else if (action.type == "DELETE") {
        if (!records.add(action.before)) {
            cout << "Undo could not restore record because the ID is already in use.\n";
            return false;
        }
    } else if (action.type == "UPDATE") {
        Student ignored;
        if (!records.update(action.studentId, action.before, ignored)) {
            cout << "Undo failed: student record was not found.\n";
            return false;
        }
    }
    cout << "Undo completed: " << action.type << " for student ID "
         << action.studentId << ".\n";
    return true;
}

int linearSearch(Student* data, int n, int target, long long& comparisons) {
    comparisons = 0;
    for (int i = 0; i < n; ++i) {
        ++comparisons;
        if (data[i].id == target) return i;
    }
    return -1;
}

int binarySearch(Student* data, int n, int target, long long& comparisons) {
    comparisons = 0;
    int left = 0, right = n - 1;
    while (left <= right) {
        int mid = left + (right - left) / 2;
        ++comparisons;
        if (data[mid].id == target) return mid;
        if (data[mid].id < target) left = mid + 1;
        else right = mid - 1;
    }
    return -1;
}

void bubbleSort(Student* data, int n, Metrics& m) {
    m = {};
    for (int i = 0; i < n - 1; ++i) {
        bool changed = false;
        for (int j = 0; j < n - i - 1; ++j) {
            ++m.comparisons;
            if (data[j].id > data[j + 1].id) {
                swap(data[j], data[j + 1]);
                ++m.swaps;
                changed = true;
            }
        }
        if (!changed) break;
    }
}

void selectionSort(Student* data, int n, Metrics& m) {
    m = {};
    for (int i = 0; i < n - 1; ++i) {
        int minIndex = i;
        for (int j = i + 1; j < n; ++j) {
            ++m.comparisons;
            if (data[j].id < data[minIndex].id) minIndex = j;
        }
        if (minIndex != i) {
            swap(data[i], data[minIndex]);
            ++m.swaps;
        }
    }
}

void insertionSort(Student* data, int n, Metrics& m) {
    m = {};
    for (int i = 1; i < n; ++i) {
        Student key = data[i];
        int j = i - 1;
        while (j >= 0) {
            ++m.comparisons;
            if (data[j].id <= key.id) break;
            data[j + 1] = data[j];
            ++m.swaps;
            --j;
        }
        data[j + 1] = key;
    }
}

void searchMenu(StudentList& records) {
    const int capacity = 50000;
    Student* data = new Student[capacity];
    int n = records.copyToArray(data, capacity);
    if (n == 0) {
        cout << "Add student records first.\n";
        delete[] data;
        return;
    }
    cout << "1. Linear search\n2. Binary search (sorts a copy by ID first)\n";
    int choice = readInt("Choose: ", 1, 2);
    int target = readInt("Student ID to find: ", 1, 999999);
    if (choice == 2) {
        Metrics m;
        insertionSort(data, n, m);
    }
    long long comparisons = 0;
    int index = choice == 1 ? linearSearch(data, n, target, comparisons)
                            : binarySearch(data, n, target, comparisons);
    if (index >= 0) {
        cout << "Student found:\n";
        printStudent(data[index]);
    } else {
        cout << "Student not found.\n";
    }
    cout << "Comparisons: " << comparisons << '\n';
    delete[] data;
}

void sortingMenu(StudentList& records) {
    const int capacity = 50000;
    Student* data = new Student[capacity];
    int n = records.copyToArray(data, capacity);
    if (n < 2) {
        cout << "At least two student records are needed.\n";
        delete[] data;
        return;
    }
    cout << "1. Bubble Sort\n2. Selection Sort\n3. Insertion Sort\n";
    int choice = readInt("Choose: ", 1, 3);
    Metrics metrics;
    auto start = chrono::high_resolution_clock::now();
    if (choice == 1) bubbleSort(data, n, metrics);
    else if (choice == 2) selectionSort(data, n, metrics);
    else insertionSort(data, n, metrics);
    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double, milli> elapsed = end - start;

    cout << "Sorted records by student ID (first 20 shown):\n";
    cout << left << setw(10) << "ID" << setw(25) << "Name" << "Service\n";
    for (int i = 0; i < n && i < 20; ++i)
        cout << left << setw(10) << data[i].id << setw(25) << data[i].name
             << data[i].serviceType << '\n';
    cout << "Comparisons: " << metrics.comparisons
         << "\nSwaps/moves: " << metrics.swaps
         << "\nExecution time (ms): " << fixed << setprecision(3)
         << elapsed.count() << '\n';
    delete[] data;
}

void benchmarkMenu() {
    int sizes[] = {100, 1000, 10000, 50000};
    cout << "Dataset sizes: 1) 100  2) 1,000  3) 10,000  4) 50,000\n";
    int sizeChoice = readInt("Choose dataset size: ", 1, 4);
    int n = sizes[sizeChoice - 1];
    cout << "Data profile: 1) Random  2) Already sorted  3) Reverse sorted\n";
    int profile = readInt("Choose profile: ", 1, 3);
    cout << "Algorithm: 1) Bubble  2) Selection  3) Insertion\n";
    int algorithm = readInt("Choose algorithm: ", 1, 3);

    if (n >= 10000 && (algorithm == 1 || algorithm == 2 || profile != 2)) {
        cout << "Warning: This sort/profile combination may take a long time at this size.\n";
        int proceed = readInt("Continue anyway? (1 = Yes, 0 = Cancel): ", 0, 1);
        if (proceed == 0) return;
    }

    Student* data = new Student[n];
    mt19937 rng(static_cast<unsigned int>(
        chrono::high_resolution_clock::now().time_since_epoch().count()));
    uniform_int_distribution<int> dist(1, 999999);
    for (int i = 0; i < n; ++i) {
        int id = profile == 1 ? dist(rng) : (profile == 2 ? i + 1 : n - i);
        data[i] = {id, "Sample Student", "Benchmark", 0};
    }

    Metrics metrics;
    auto start = chrono::high_resolution_clock::now();
    if (algorithm == 1) bubbleSort(data, n, metrics);
    else if (algorithm == 2) selectionSort(data, n, metrics);
    else insertionSort(data, n, metrics);
    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double, milli> elapsed = end - start;

    const char* algorithmName = algorithm == 1 ? "Bubble Sort" :
                                algorithm == 2 ? "Selection Sort" : "Insertion Sort";
    const char* profileName = profile == 1 ? "Random" :
                              profile == 2 ? "Already sorted" : "Reverse sorted";
    cout << "\nBENCHMARK RESULT\n"
         << "Algorithm: " << algorithmName
         << "\nDataset size: " << n
         << "\nProfile: " << profileName
         << "\nComparisons: " << metrics.comparisons
         << "\nSwaps/moves: " << metrics.swaps
         << "\nExecution time (ms): " << fixed << setprecision(3)
         << elapsed.count() << '\n';
    delete[] data;
}

void queueMenu(StudentList& records, CircularQueue& regular, PriorityQueue& priority) {
    cout << "1. Add a student to a queue\n2. Process regular queue\n"
         << "3. Process priority queue\n4. Display regular queue\n5. Display priority queue\n";
    int choice = readInt("Choose: ", 1, 5);
    if (choice == 1) {
        int id = readInt("Existing student ID: ", 1, 999999);
        Student* s = records.find(id);
        if (s == nullptr) {
            cout << "Student ID not found. Add the student record first.\n";
            return;
        }
        if (s->priority == 1) {
            priority.enqueue(*s);
            cout << "Added to priority queue.\n";
        } else if (regular.enqueue(*s)) {
            cout << "Added to regular queue.\n";
        } else {
            cout << "Regular queue is full.\n";
        }
    } else if (choice == 2 || choice == 3) {
        Student s;
        bool ok = choice == 2 ? regular.dequeue(s) : priority.dequeue(s);
        if (ok) {
            cout << "Now processing:\n";
            printStudent(s);
        } else cout << "Queue is empty.\n";
    } else if (choice == 4) regular.display();
    else priority.display();
}

int main() {
    StudentList records;
    AuditStack audit;
    CircularQueue regularQueue(100);
    PriorityQueue priorityQueue;

    while (true) {
        cout << "\n============================================\n"
             << " UNIVERSITY STUDENT SERVICE & QUEUE SYSTEM\n"
             << "============================================\n"
             << "1. Add student record\n"
             << "2. Display all student records\n"
             << "3. Search student record\n"
             << "4. Update student record\n"
             << "5. Delete student record\n"
             << "6. Queue management\n"
             << "7. Display audit stack\n"
             << "8. Undo last record action\n"
             << "9. Sorting module\n"
             << "10. Benchmarking suite\n"
             << "0. Exit\n";

        int choice = readInt("Enter choice: ", 0, 10);
        if (choice == 0) {
            cout << "Program ended. Dynamic memory will be cleaned up.\n";
            break;
        }

        if (choice == 1) {
            Student s = inputStudent();
            if (records.add(s)) {
                audit.push({"ADD", {}, s, s.id});
                cout << "Student added successfully.\n";
            } else cout << "That student ID already exists.\n";
        } else if (choice == 2) {
            records.display();
        } else if (choice == 3) {
            int id = readInt("Student ID to search: ", 1, 999999);
            Student* s = records.find(id);
            if (s != nullptr) printStudent(*s);
            else cout << "Student not found.\n";
        } else if (choice == 4) {
            int id = readInt("Student ID to update: ", 1, 999999);
            Student* current = records.find(id);
            if (current == nullptr) {
                cout << "Student not found.\n";
            } else {
                Student replacement = inputStudent();
                Student oldData;
                if (records.update(id, replacement, oldData)) {
                    audit.push({"UPDATE", oldData, replacement, replacement.id});
                    cout << "Student updated.\n";
                } else cout << "Update failed. The replacement ID may already exist.\n";
            }
        } else if (choice == 5) {
            int id = readInt("Student ID to delete: ", 1, 999999);
            Student removed;
            if (records.remove(id, removed)) {
                audit.push({"DELETE", removed, {}, id});
                cout << "Student deleted.\n";
            } else cout << "Student not found.\n";
        } else if (choice == 6) {
            queueMenu(records, regularQueue, priorityQueue);
        } else if (choice == 7) {
            audit.display();
        } else if (choice == 8) {
            undoLast(records, audit);
        } else if (choice == 9) {
            sortingMenu(records);
        } else if (choice == 10) {
            benchmarkMenu();
        }
    }
    return 0;
}
