<<<<<<< HEAD
*This project has been created as part of the 42 curriculum by diosoare*

# Codexion

Codexion is a concurrency simulation written in C that models coders in a circular co-working hub compiling quantum code with USB dongles, demonstrating proper use of POSIX threads, mutexes, condition variables, and priority queues without deadlock or starvation.

## Description

Codexion is a concurrency simulation written in C. It models coders seated in a circular co-working hub who must compile quantum code using two USB dongles. Each coder is represented by a thread. The simulation enforces dongle cooldown, fair arbitration (FIFO or EDF), and precise burnout detection. The goal is to demonstrate correct use of POSIX threads, mutexes, condition variables, and priority queues without deadlock, starvation, or data races. The simulation stops either when every coder has compiled a required number of times or when any coder burns out.

## Instructions

### Compilation

```sh
make
```

The Makefile compiles the project with `cc -Wall -Wextra -Werror -pthread`. It contains the rules `NAME`, `all`, `clean`, `fclean`, and `re`.

### Execution

```sh
./codexion number_of_coders time_to_burnout time_to_compile time_to_debug time_to_refactor number_of_compiles_required dongle_cooldown scheduler
```

Example:

```sh
./codexion 5 800 200 200 200 7 100 edf
```

Arguments:

- `number_of_coders`: number of coders and number of dongles.
- `time_to_burnout`: milliseconds before a coder burns out if they do not start compiling.
- `time_to_compile`: milliseconds a coder spends compiling while holding two dongles.
- `time_to_debug`: milliseconds a coder spends debugging.
- `time_to_refactor`: milliseconds a coder spends refactoring.
- `number_of_compiles_required`: simulation stops after all coders reach this count.
- `dongle_cooldown`: milliseconds a dongle is unavailable after being released.
- `scheduler`: `fifo` or `edf`.

The program logs state changes to standard output in the format specified by the subject.

## Resources

### POSIX Threads and Synchronization

- **POSIX Threads Programming** – Lawrence Livermore National Laboratory (LLNL). The canonical tutorial for `pthread_create`, `pthread_mutex_t`, and `pthread_cond_t`. Covers thread creation, joining, mutexes, and condition variables with complete C examples.  
  [https://hpc-tutorials.llnl.gov/](https://hpc-tutorials.llnl.gov/posix/)

- **Condition Variables Example** – LLNL. A minimal, runnable example showing `pthread_cond_wait` inside a `while` loop, the correct pattern for handling spurious wakeups.  
  [https://hpc-tutorials.llnl.gov/](https://hpc-tutorials.llnl.gov/posix/example_using_cond_vars/)

- **The Little Book of Semaphores** – Allen B. Downey (free online). Chapter 4 covers the Dining Philosophers and other classical synchronization problems using semaphores, but the patterns translate directly to mutexes and condition variables.  
  [Green Tea Press](https://greenteapress.com/wp/semaphores/)

### Dining Philosophers and Deadlock

- **Dijkstra, E. W. (1965). Cooperating Sequential Processes.** The original technical report that introduced the Dining Philosophers problem.  
 [cs.utexas.edu](https://www.cs.utexas.edu/users/EWD/ewd01xx/EWD123.PDF)

- **The Coffman Conditions** – A concise explanation of the four necessary conditions for deadlock (mutual exclusion, hold and wait, no preemption, circular wait) and how to break each one.  
  [Wikipedia](https://en.wikipedia.org/wiki/Deadlock_(computer_science)#Necessary_conditions)

### Real-Time Scheduling and Priority Queues

- **Earliest Deadline First (EDF) Scheduling** – A practical overview of EDF, why it is optimal for uniprocessor real-time systems, and how deadlines are assigned.  
  [Wikipedia](https://en.wikipedia.org/wiki/Earliest_deadline_first_scheduling)

- **Binary Heap Priority Queue in C** – A well-commented, minimal implementation of a binary heap used as a priority queue. Demonstrates `heapify`, `push`, `pop`, and priority updates.  
  [Github](https://github.com/Shuvam-Banerji-Seal/C-Programming-for-Beginners/blob/main/12-advanced-data-structures/heap_priority_queue.c)

### Man Pages

- `pthread_create(3)`, `pthread_mutex_lock(3)`, `pthread_cond_wait(3)`, `pthread_cond_timedwait(3)`, `gettimeofday(2)`, `usleep(3)`.  
  Available locally via `man <function>` on any Linux system.

### AI Usage

A local AI model (`Qwen3.8-4B-Distill-GGUF`, Q8_0 quantization) was used throughout development for:

- Scaffolding the initial project structure (Makefile, source files, headers).
- Explaining the Dining Philosophers problem and its mapping to the Codexion simulation.
- Implementing and explaining the priority queue (binary heap) for FIFO and EDF scheduling.
- Implementing and explaining the dongle cooldown logic.
- Implementing the monitor thread for precise burnout detection.
- Running the simulation end-to-end and checking for deadlocks, starvation, and data races.
- Drafting and iterating on this README.

All AI-generated content was reviewed, tested, and understood by the authors.

## Blocking cases handled

- **Deadlock**: prevented by scheduler-aware granting and by breaking the circular wait condition. The four Coffman conditions are analyzed in the theoretical appendix.
- **Starvation**: FIFO ensures arrival-order fairness. EDF prioritizes the earliest deadline and guarantees liveness under feasible parameters.
- **Cooldown**: each dongle tracks its release time and remains unavailable for `dongle_cooldown` milliseconds.
- **Burnout detection**: a dedicated monitor thread checks deadlines and logs burnout within 10 ms of the actual deadline violation.
- **Log serialization**: a dedicated mutex protects all output so that two messages never interleave on a single line.

## Thread synchronization mechanisms

- `pthread_mutex_t`: one mutex per dongle protects dongle state and request queue. A separate mutex protects logging. A mutex protects monitor state.
- `pthread_cond_t`: one condition variable per dongle is used to wait for availability. Wait operations are wrapped in `while` loops to handle spurious wakeups.
- **Monitor thread**: reads each coder's `last_compile_start` under mutex protection and checks a termination flag.
- **Custom event implementation**: condition variables combined with predicates signal state changes, such as a dongle being released and its cooldown expiring.
- **Race condition prevention**: all shared data is accessed under the appropriate mutex. For example, when a coder takes a dongle, it locks the dongle mutex, checks availability, updates state, and unlocks. The monitor locks each coder's state mutex before reading `last_compile_start`.


## How AI was used on this project

A local AI model (Qwen3.8 4B Distill, Qwen3.8-4B-Distill-GGUF, Q8_0 quantization) was used throughout development for:

- Scaffolding the initial project structure (Makefile, source files, headers).
- Explaining the Dining Philosophers problem and its mapping to the Codexion simulation.
- Explaining the priority queue (binary heap) for FIFO and EDF scheduling.
- Running the simulation end-to-end and checking for deadlocks, starvation, and data races.
- Drafting and iterating on this README.

---

# Processes and Threads: Codexion Simulation

## 1. Introduction

The **Dining Philosophers problem**, originally formulated by Edsger Dijkstra in 1965, is one of the most enduring pedagogical tools in computer science for illustrating the fundamental challenges of concurrent programming. The problem presents a scenario in which multiple agents compete for a limited set of shared resources, and the core objective is to design a protocol that guarantees progress without deadlock or starvation.

The **Codexion simulation** is a modern variant of this classic problem. In place of philosophers, coders sit in a circular co-working hub and must compile quantum code. Compiling requires two USB dongles, one for each hand, and there are exactly as many dongles as there are coders. After compiling, a coder debugs, then refactors, and then attempts to compile again. A coder burns out if they fail to begin compiling within a specified timeout. The simulation stops either when all coders have compiled a required number of times or when any coder burns out.

This lecture examines the theoretical foundations of processes and threads as they manifest in this simulation. The focus is on the concurrency concepts that matter: thread creation and lifecycle, synchronization primitives, deadlock and starvation, resource scheduling, and liveness guarantees.

## 2. Processes and Threads: Foundational Concepts

### 2.1 Processes

A **process** is an instance of a program in execution. It possesses its own virtual address space, which includes the program code, data segment, heap, and stack. Each process is isolated from other processes; communication between processes requires explicit inter-process communication (IPC) mechanisms such as pipes, shared memory, or message queues.

### 2.2 Threads

A **thread** is a sequential execution stream within a process. Unlike processes, multiple threads within the same process share the same address space. They see the same heap and global variables, but each thread maintains its own stack and program counter. This shared memory model makes threads efficient for concurrent tasks that need to operate on common data, but it also introduces the need for synchronization to prevent race conditions.

In the Codexion simulation, each coder is represented by a thread. This is a natural mapping: coders share access to dongles (shared resources) and must coordinate their actions. The coder threads are created using `pthread_create`, and the main thread may use `pthread_join` to wait for them to terminate.

### 2.3 Why Threads Over Processes?

The choice of threads over processes in this simulation is deliberate. Threads are lighterweight to create and context-switch than processes, and they share memory, which simplifies the representation of shared dongles and shared state. However, this shared memory model demands rigorous synchronization, which is the central challenge of the simulation.

## 3. Concurrency and Synchronization

### 3.1 The Need for Synchronization

In a multi-threaded program, threads execute concurrently. When multiple threads access shared data without synchronization, **race conditions** arise. The outcome depends on the unpredictable order in which threads execute. In the Codexion simulation, shared state includes:

- The availability status of each dongle (free, held, or in cooldown).
- The request queues for each dongle.
- Each coder's last compile start time and compile count.
- The termination flag for the simulation.

Without proper synchronization, a dongle could be granted to two coders simultaneously, or a coder could read a stale value of another coder's deadline. These are **data races**, which are undefined behavior in C.

### 3.2 Mutual Exclusion with Mutexes

The primary tool for preventing data races is the **mutex** (mutual exclusion). A mutex is a lock that ensures only one thread can access a critical section of code at a time. In the Codexion simulation, each dongle is protected by its own mutex. When a coder wishes to take a dongle, they must first lock the dongle's mutex, modify the dongle's state, and then unlock the mutex. This guarantees that dongle state changes are atomic with respect to other threads.

Similarly, a **logging mutex** serializes all output. Without it, two threads could interleave their `printf` calls, producing garbled output. The logging mutex ensures that each log line is printed atomically.

### 3.3 Condition Variables

Mutexes alone are insufficient for all synchronization needs. Often, a thread must wait for some condition to become true, for example, a coder waiting for a dongle to become available. **Condition variables** provide a mechanism for a thread to block until it is signaled by another thread.

The correct usage pattern for condition variables is to wait inside a **while loop** that checks the condition predicate. This is necessary because condition variables can experience **spurious wakeups**. The thread may be awakened even though the condition it is waiting for is not true. Additionally, if multiple threads are waiting on the same condition variable, `pthread_cond_signal` may wake any one of them, not necessarily the one that should proceed. Wrapping the wait in a while loop ensures that a thread only proceeds when its specific predicate is satisfied.

In the Codexion simulation, each dongle has an associated condition variable. When a dongle is released and its cooldown has passed, the releasing thread signals the condition variable to wake waiting coders.

## 4. Deadlock and Starvation

### 4.1 Deadlock

**Deadlock** occurs when a set of processes are each blocked, waiting for a resource held by another process in the set. In the Dining Philosophers problem, deadlock arises when every philosopher picks up their left fork simultaneously. Each holds one fork and waits forever for the other, which is held by their neighbor.

The four **Coffman conditions** are necessary for deadlock to occur:

1. **Mutual exclusion**: Resources cannot be shared; a resource is either held by one process or available.
2. **Hold and wait**: A process holding at least one resource is waiting to acquire additional resources held by other processes.
3. **No preemption**: A resource can only be released voluntarily by the process holding it.
4. **Circular wait**: A closed chain of processes exists where each process holds a resource that the next process in the chain is waiting for.

In the Codexion simulation, mutual exclusion is enforced by the dongle mutexes. Hold-and-wait occurs when a coder acquires one dongle and waits for the second. No-preemption means dongles cannot be forcibly taken away. The critical design challenge is to prevent the circular wait condition from producing a deadlock.

### 4.2 Deadlock Prevention in Practice

A common approach to preventing deadlock in the Dining Philosophers problem is to **impose a global ordering** on resource acquisition. For example, coders could be required to always acquire the lower-numbered dongle first. This breaks the circular wait condition. Another approach is to use an **arbitrator**, a central authority that grants dongles only when both are available simultaneously.

The Codexion simulation addresses this through **scheduler-aware granting**. The dongles use a priority queue to determine which coder should receive a dongle when multiple requests are pending. By controlling the order of grants, the system can avoid the circular wait that would otherwise arise.

### 4.3 Starvation

**Starvation** occurs when a process is indefinitely postponed, even though the system as a whole is making progress. Unlike deadlock, starvation does not require a circular wait. It can arise from unfair scheduling. In the Codexion simulation, starvation would manifest as a coder repeatedly losing the competition for dongles and eventually burning out.

The simulation addresses starvation through its scheduling policies: **FIFO** (first-in, first-out) ensures that requests are served in arrival order, which prevents a coder from being repeatedly overtaken by later arrivals. **EDF** (earliest deadline first) prioritizes coders with the most urgent deadlines, which is a form of dynamic priority scheduling that guarantees liveness under feasible parameters.

## 5. Scheduling Algorithms

The Codexion simulation supports two scheduling policies for dongle arbitration: FIFO and EDF. Both are implemented using a **priority queue** (a binary heap) to efficiently determine which waiting coder should receive a dongle.

### 5.1 FIFO Scheduling

**First-In, First-Out** is the simplest scheduling policy. When multiple coders request the same dongle, the dongle is granted to the coder whose request arrived earliest. FIFO is inherently fair in the sense that it prevents starvation: as long as requests are eventually served, every coder will eventually reach the front of the queue.

FIFO does not consider urgency. A coder with a very tight deadline may be stuck behind a coder with a generous deadline. In the Codexion simulation, this could lead to burnout even when a feasible schedule exists.

### 5.2 EDF Scheduling

**Earliest Deadline First** is a dynamic priority scheduling algorithm used in real-time systems. Each coder has a deadline equal to `last_compile_start + time_to_burnout`. When a dongle becomes available, EDF grants it to the coder with the earliest deadline. The priority queue is dynamically updated as coders' deadlines change.

EDF is optimal for uniprocessor real-time scheduling: if any scheduling algorithm can meet all deadlines, EDF can. However, this optimality holds only under certain conditions, such as when the system is not overloaded. In the Codexion simulation, EDF is designed to guarantee liveness: no coder should starve of dongles and burn out, provided the parameters are feasible.

### 5.3 Implementation Considerations

Both FIFO and EDF require a priority queue. The heap must support insertion, extraction of the highest-priority element, and, for EDF, updates when a coder's deadline changes. The queue is protected by the dongle's mutex: enqueue and dequeue operations are atomic with respect to dongle state changes.

## 6. Resource Management: Dongle Cooldown

A distinctive feature of the Codexion simulation is the **dongle cooldown**. After a dongle is released by a coder, it cannot be acquired by any coder until `dongle_cooldown` milliseconds have elapsed. This models a physical or administrative delay, for example, the time required for a hardware dongle to be reset or re-registered.

Cooldown introduces a subtle challenge. During the cooldown period, the dongle is unavailable, but it is not held by any coder. If the cooldown is not properly enforced, a coder could acquire a dongle that is still cooling down, violating the simulation's rules. The implementation must track the release time of each dongle and compare it against the current time when a request is made.

The cooldown also interacts with scheduling. If a dongle is in cooldown, pending requests must wait. The condition variable associated with the dongle should not be signaled until the cooldown has expired. This may require a timed wait or a periodic check.

## 7. Burnout Detection and Liveness

### 7.1 Burnout as a Deadline Violation

In the Codexion simulation, **burnout** occurs when a coder fails to begin compiling within `time_to_burnout` milliseconds since their last compile start or the start of the simulation. This is a **deadline violation**. The simulation must detect burnout precisely and log it within 10 milliseconds of the actual burnout time.

Burnout detection is the responsibility of a dedicated **monitor thread**. The monitor periodically examines each coder's last compile start time and computes whether their deadline has passed. If a deadline is violated, the monitor logs the burnout and signals the simulation to terminate.

### 7.2 Liveness Guarantees

**Liveness** is the property that something good eventually happens. In this case, every coder eventually compiles and the simulation terminates. The Codexion simulation must guarantee liveness under the EDF scheduler: no coder should be starved of dongles and burn out if the parameters are feasible.

Liveness is ensured through several mechanisms:

- **Fair arbitration**: FIFO ensures that requests are served in order. EDF ensures that the most urgent requests are served first.
- **Deadlock prevention**: The scheduler-aware granting mechanism avoids the circular wait that would otherwise produce deadlock.
- **Cooldown handling**: The cooldown is enforced consistently, preventing dongles from being acquired prematurely.
- **Precise burnout detection**: The monitor thread ensures that the simulation stops promptly when a coder burns out, preventing other coders from waiting indefinitely.

### 7.3 The Monitor Thread

The monitor thread operates concurrently with the coder threads. It reads shared state, specifically each coder's last compile start time, under mutex protection to avoid stale reads. It also checks a termination flag to know when to exit. The monitor thread is essential for two reasons: it detects burnout that the coders themselves cannot detect, since coders do not communicate with each other, and it ensures that the simulation terminates in a timely manner.

## 8. Summary of Key Concepts

| Concept | Relevance to Codexion |
| - | - |
| Thread | Each coder is a thread; concurrency is inherent. |
| Shared memory | Dongles, coder state, and logs are shared across threads. |
| Mutex | Protects dongle state and serializes logging. |
| Condition variable | Wakes waiting coders when a dongle becomes available. |
| Deadlock | Prevented by scheduler-aware granting and ordered acquisition. |
| Starvation | Prevented by FIFO fairness and EDF urgency prioritization. |
| Coffman conditions | Analyze the deadlock potential in the resource sharing model. |
| FIFO scheduling | Serves requests in arrival order; fair but not urgency-aware. |
| EDF scheduling | Serves the most urgent request first; optimal for deadlines. |
| Priority queue | Efficiently orders waiting requests for both schedulers. |
| Cooldown | Enforces a delay between dongle release and reacquisition. |
| Burnout | A deadline violation detected by the monitor thread. |
| Liveness | Guaranteed by fair scheduling and deadlock prevention. |
| Monitor thread | Detects burnout and terminates the simulation. |

The Codexion simulation is a rich exercise in concurrent programming. It requires not only the mechanical skills of using `pthread_create`, `pthread_mutex_lock`, and `pthread_cond_wait`, but also a deep understanding of why these primitives exist and how they interact. The challenges of deadlock, starvation, race conditions, and liveness are not abstract. They manifest concretely in the behavior of the simulation. A correct solution must demonstrate both technical proficiency and conceptual clarity.

=======
Codexion
>>>>>>> 071fb4ff5224e5e481bde7f45e6916e1387d31e5
