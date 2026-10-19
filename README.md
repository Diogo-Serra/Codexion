*This project has been created as part of the 42 curriculum by diosoare*

# Codexion

Concurrency simulation written in C

## Description

Codexion is a concurrency simulation written in C. It models coders seated in a
circular co-working hub who must compile quantum code using two USB dongles.
Each coder is represented by a thread. The simulation enforces dongle cooldown,
fair arbitration (FIFO or EDF), and precise burnout detection. The goal is to
demonstrate correct use of POSIX threads, mutexes, condition variables, and
priority queues without deadlock, starvation, or data races. The simulation
stops either when every coder has compiled a required number of times or when
any coder burns out.

## Instructions

### Compilation

```sh
make
```

The `Makefile` defines the variables `NAME`, `CC`, `CFLAGS`, `SRCS`, `OBJS`,
`HDR` and the rules `all`, `clean`, `fclean`, and `re`. The project compiles
with `cc -Wall -Wextra -Werror -pthread` and produces a single binary named
`codexion`.

### Execution

```sh
./codexion number_of_coders time_to_burnout time_to_compile time_to_debug time_to_refactor number_of_compiles_required dongle_cooldown scheduler
```

Example (benchmark case from the subject):

```sh
./codexion 5 2000 200 200 200 10 0 fifo
```

Arguments:

- `number_of_coders`: number of coders and number of dongles.
- `time_to_burnout`: milliseconds before a coder burns out if they do not start
  compiling.
- `time_to_compile`: milliseconds a coder spends compiling while holding two
  dongles.
- `time_to_debug`: milliseconds a coder spends debugging.
- `time_to_refactor`: milliseconds a coder spends refactoring.
- `number_of_compiles_required`: simulation stops after all coders reach this
  count. Must be **≥ 1** — passing `0` makes every coder trivially "done" at
  startup and the simulation would stop immediately, so the parser rejects
  `0` (and any negative value) with an error message.
- `dongle_cooldown`: milliseconds a dongle is unavailable after being released.
- `scheduler`: `fifo` or `edf`.

The program logs state changes to standard output in the format specified by
the subject.

## Resources

### POSIX Threads and Synchronization

- **POSIX Threads Programming** – Lawrence Livermore National Laboratory
  (LLNL). The canonical tutorial for `pthread_create`, `pthread_mutex_t`, and
  `pthread_cond_t`. Covers thread creation, joining, mutexes, and condition
  variables with complete C examples.
  [hpc-tutorials.llnl.gov/posix](https://hpc-tutorials.llnl.gov/posix/)

- **Condition Variables Example** – LLNL. A minimal, runnable example showing
  `pthread_cond_wait` inside a `while` loop, the correct pattern for handling
  spurious wakeups.
  [hpc-tutorials.llnl.gov/posix/example_using_cond_vars](https://hpc-tutorials.llnl.gov/posix/example_using_cond_vars/)

- **The Little Book of Semaphores** – Allen B. Downey (free online). Chapter 4
  covers the Dining Philosophers and other classical synchronization problems
  using semaphores; the patterns translate directly to mutexes and condition
  variables.
  [Green Tea Press](https://greenteapress.com/wp/semaphores/)

### Dining Philosophers and Deadlock

- **Dijkstra, E. W. (1965). Cooperating Sequential Processes.** The original
  technical report that introduced the Dining Philosophers problem.
  [cs.utexas.edu](https://www.cs.utexas.edu/users/EWD/ewd01xx/EWD123.PDF)

- **The Coffman Conditions** – A concise explanation of the four necessary
  conditions for deadlock (mutual exclusion, hold and wait, no preemption,
  circular wait) and how to break each one.
  [Wikipedia](https://en.wikipedia.org/wiki/Deadlock_(computer_science)#Necessary_conditions)

### Real-Time Scheduling and Priority Queues

- **Earliest Deadline First (EDF) Scheduling** – A practical overview of EDF,
  why it is optimal for uniprocessor real-time systems, and how deadlines are
  assigned.
  [Wikipedia](https://en.wikipedia.org/wiki/Earliest_deadline_first_scheduling)

- **Binary Heap Priority Queue in C** – A well-commented, minimal
  implementation of a binary heap used as a priority queue. Demonstrates
  `heapify`, `push`, `pop`, and priority updates.
  [GitHub](https://github.com/Shuvam-Banerji-Seal/C-Programming-for-Beginners/blob/main/12-advanced-data-structures/heap_priority_queue.c)

### Man Pages

- `pthread_create(3)`, `pthread_mutex_lock(3)`, `pthread_cond_wait(3)`,
  `pthread_cond_timedwait(3)`, `gettimeofday(2)`, `usleep(3)`.
  Available locally via `man <function>` on any Linux system.

### AI Usage

A local AI model (`LFM2.5-2.6B-GGUF`, Q8_0 quantization) was used throughout
development for:

- Scaffolding the initial project structure (Makefile, source files, headers).
- Explaining the Dining Philosophers problem and its mapping to the Codexion
  simulation.
- Explaining the priority queue (binary heap) for FIFO and EDF scheduling.
- Running the simulation end-to-end and checking for deadlocks, starvation,
  and data races.
- Drafting and iterating on this README.

All AI-generated content was reviewed, tested, and understood by the author.

## Blocking cases handled

- **Deadlock**: prevented by breaking **hold-and-wait**. A coder never holds one
  dongle while waiting for the other. It enqueues on both dongles' request
  heaps, and only takes both **atomically** — flipping both `taken` flags and
  popping both heaps under both dongle mutexes — when it is simultaneously the
  head of both queues and both dongles are free and off cooldown. Dongle locks
  are always acquired in pointer order to avoid lock-order inversion. Because
  hold-and-wait cannot occur, no circular-wait cycle can form.
- **Starvation**: FIFO ensures arrival-order fairness. EDF prioritizes the
  earliest deadline and guarantees liveness under feasible parameters.
- **Cooldown**: each dongle tracks its release time (`available_at`) and
  remains unavailable for `dongle_cooldown` milliseconds; `is_ready` refuses
  until `get_time() >= available_at`.
- **Burnout detection**: a dedicated monitor thread checks deadlines and logs
  burnout promptly when a deadline is violated. A coder that has already
  started compiling is never declared burned out during that compile (the
  monitor skips coders whose `is_compiling` flag is set): burnout is only
  declared when a coder fails to *start* a compile in time.
- **Log serialization**: a dedicated `write` mutex protects all output so that
  two messages never interleave on a single line. The three-line group
  `has taken a dongle` × 2 + `is compiling` is printed atomically inside a
  single critical section, so no other coder's log can appear between a
  `compiling` line and its two `take` lines.
- **Deterministic shutdown**: a coder sets `coder->finished` under its own lock
  only after its **last full cycle** (compile + debug + refactor) has finished.
  The monitor stops the simulation once every coder has `finished == true`, so
  the last `is refactoring` line is never dropped.

## Thread synchronization mechanisms

- `pthread_mutex_t`: one mutex per dongle protects dongle state and its request
  heap. A dedicated `write` mutex serialises logging. A scheduler mutex
  protects the termination flag and the FIFO ticket counter, and every coder
  has its own mutex protecting its state (`num_compiles`, `burnout_time`,
  `is_compiling`, `finished`).
- `pthread_cond_t`: one condition variable per dongle is used to wait for
  availability. Every wake-up is followed by a full predicate re-check:
  `acquire_dongles` re-runs `attempt`, which re-locks both dongles and
  re-tests the condition. This retry loop is what makes the code safe against
  spurious wake-ups and against broadcasts that were not aimed at the woken
  coder.
- **Monitor thread**: reads each coder's state under mutex protection and
  checks the termination flag.
- **Timed waits**: when a coder is at the head of the queue but its dongle is
  still inside the cooldown window, `wait_on` uses `pthread_cond_timedwait`
  targeting `available_at`, so the coder is woken exactly when the cooldown
  expires instead of busy-waiting.
- **Race condition prevention**: all shared data is accessed under the
  appropriate mutex. When a coder takes dongles it locks both dongle mutexes
  in a fixed pointer order, verifies that it is the head of *both* request
  heaps and that both dongles are free, then flips both `taken` flags and
  pops both heaps atomically. The monitor locks a coder's own mutex before
  reading its status, so it never observes a torn value.

---

# Processes and Threads: Codexion Simulation

## 1. Introduction

The **Dining Philosophers problem**, originally formulated by Edsger Dijkstra
in 1965, is one of the most enduring pedagogical tools in computer science for
illustrating the fundamental challenges of concurrent programming. The problem
presents a scenario in which multiple agents compete for a limited set of
shared resources, and the core objective is to design a protocol that
guarantees progress without deadlock or starvation.

The **Codexion simulation** is a modern variant of this classic problem. In
place of philosophers, coders sit in a circular co-working hub and must
compile quantum code. Compiling requires two USB dongles, one for each hand,
and there are exactly as many dongles as there are coders. After compiling, a
coder debugs, then refactors, and then attempts to compile again. A coder
burns out if they fail to begin compiling within a specified timeout. The
simulation stops either when all coders have compiled a required number of
times or when any coder burns out.

This section examines the theoretical foundations of processes and threads as
they manifest in this simulation. The focus is on the concurrency concepts
that matter: thread creation and lifecycle, synchronization primitives,
deadlock and starvation, resource scheduling, and liveness guarantees.

## 2. Processes and Threads: Foundational Concepts

### 2.1 Processes

A **process** is an instance of a program in execution. It possesses its own
virtual address space, which includes the program code, data segment, heap,
and stack. Each process is isolated from other processes; communication
between processes requires explicit inter-process communication (IPC)
mechanisms such as pipes, shared memory, or message queues.

### 2.2 Threads

A **thread** is a sequential execution stream within a process. Unlike
processes, multiple threads within the same process share the same address
space. They see the same heap and global variables, but each thread maintains
its own stack and program counter. This shared memory model makes threads
efficient for concurrent tasks that need to operate on common data, but it
also introduces the need for synchronization to prevent race conditions.

In the Codexion simulation, each coder is represented by a thread. This is a
natural mapping: coders share access to dongles (shared resources) and must
coordinate their actions. The coder threads are created using
`pthread_create`, and the main thread uses `pthread_join` to wait for them to
terminate.

### 2.3 Why Threads Over Processes?

The choice of threads over processes in this simulation is deliberate. Threads
are lighterweight to create and context-switch than processes, and they share
memory, which simplifies the representation of shared dongles and shared
state. However, this shared memory model demands rigorous synchronization,
which is the central challenge of the simulation.

## 3. Concurrency and Synchronization

### 3.1 The Need for Synchronization

In a multi-threaded program, threads execute concurrently. When multiple
threads access shared data without synchronization, **race conditions** arise.
The outcome depends on the unpredictable order in which threads execute. In
the Codexion simulation, shared state includes:

- The availability status of each dongle (free, held, or in cooldown).
- The request heaps for each dongle.
- Each coder's burnout deadline, compile count, `is_compiling` flag, and
  `finished` flag.
- The termination flag for the simulation.
- The FIFO ticket counter.

Without proper synchronization, a dongle could be granted to two coders
simultaneously, or a coder could read a stale value of another coder's
deadline. These are **data races**, which are undefined behavior in C.

### 3.2 Mutual Exclusion with Mutexes

The primary tool for preventing data races is the **mutex** (mutual
exclusion). A mutex is a lock that ensures only one thread can access a
critical section of code at a time. In the Codexion simulation, each dongle is
protected by its own mutex. When a coder wishes to take a dongle, it must
first lock the dongle's mutex, modify the dongle's state, and then unlock the
mutex. This guarantees that dongle state changes are atomic with respect to
other threads.

Similarly, a **logging mutex** serializes all output. Without it, two threads
could interleave their `printf` calls, producing garbled output. The logging
mutex ensures that each log line is printed atomically; the three-line
acquire group (two `has taken a dongle` plus one `is compiling`) is printed
inside a single critical section, so no other coder's log can slip in between.

### 3.3 Condition Variables

Mutexes alone are insufficient for all synchronization needs. Often, a thread
must wait for some condition to become true — for example, a coder waiting for
a dongle to become available. **Condition variables** provide a mechanism for
a thread to block until it is signaled by another thread.

The correct usage pattern for condition variables is to wait inside a
**while loop** that checks the condition predicate. This is necessary because
condition variables can experience **spurious wakeups**. The thread may be
awakened even though the condition it is waiting for is not true.
Additionally, if multiple threads are waiting on the same condition variable,
`pthread_cond_signal` may wake any one of them, not necessarily the one that
should proceed. Wrapping the wait in a while loop ensures that a thread only
proceeds when its specific predicate is satisfied.

In the Codexion simulation, each dongle has an associated condition variable.
When a dongle is released, the releasing thread broadcasts on its condition
variable. A coder that is at the head of the queue but still inside the
cooldown uses `pthread_cond_timedwait` to sleep until `available_at`, so it is
woken exactly when the cooldown expires instead of busy-waiting.

## 4. Deadlock and Starvation

### 4.1 Deadlock

**Deadlock** occurs when a set of processes are each blocked, waiting for a
resource held by another process in the set. In the Dining Philosophers
problem, deadlock arises when every philosopher picks up their left fork
simultaneously. Each holds one fork and waits forever for the other, which is
held by their neighbour.

The four **Coffman conditions** are necessary for deadlock to occur:

1. **Mutual exclusion**: Resources cannot be shared; a resource is either held
   by one process or available.
2. **Hold and wait**: A process holding at least one resource is waiting to
   acquire additional resources held by other processes.
3. **No preemption**: A resource can only be released voluntarily by the
   process holding it.
4. **Circular wait**: A closed chain of processes exists where each process
   holds a resource that the next process in the chain is waiting for.

In the Codexion simulation, mutual exclusion is enforced by the dongle
mutexes. No-preemption means dongles cannot be forcibly taken away. The key
design decision is that **hold-and-wait is broken by construction** — see
§4.2.

### 4.2 Deadlock Prevention in Practice

Two textbook approaches exist for the Dining Philosophers problem:

- **Impose a global ordering** on resource acquisition — for example, always
  acquire the lower-numbered dongle first. This breaks the *circular wait*
  condition.
- **Use an arbitrator** — a central authority that grants dongles only when
  both are available simultaneously. This breaks the *hold-and-wait*
  condition.

The Codexion simulation uses the second approach. A coder is enqueued on both
of its dongles' request heaps, and it proceeds only when it is the head of
**both** queues and both dongles are simultaneously free and off cooldown.
Only then does it take both dongles atomically — flipping both `taken` flags
and popping both heaps while holding both dongle mutexes. The coder never
holds one dongle while waiting for the other, so the hold-and-wait condition
is never satisfied, and a circular-wait cycle can therefore never form.

Additionally, all call sites that lock two dongles at once do so in **pointer
order** (`if (a > b) swap(a, b)`), which prevents a lock-order inversion — a
different deadlock that could otherwise occur on the mutexes themselves,
independently of the resource-acquisition protocol.

The scheduler (FIFO/EDF) determines *which* coder is at the head of each
queue, but it is the atomic both-or-nothing acquisition that guarantees
deadlock freedom regardless of the scheduler.

### 4.3 Starvation

**Starvation** occurs when a process is indefinitely postponed, even though
the system as a whole is making progress. Unlike deadlock, starvation does not
require a circular wait. It can arise from unfair scheduling. In the Codexion
simulation, starvation would manifest as a coder repeatedly losing the
competition for dongles and eventually burning out.

The simulation addresses starvation through its scheduling policies: **FIFO**
ensures that requests are served in arrival order, which prevents a coder from
being repeatedly overtaken by later arrivals. **EDF** prioritizes coders with
the most urgent deadlines, which is a form of dynamic priority scheduling that
guarantees liveness under feasible parameters.

## 5. Scheduling Algorithms

The Codexion simulation supports two scheduling policies for dongle
arbitration: FIFO and EDF. Both are implemented using a **priority queue**
(a binary min-heap) to efficiently determine which waiting coder should
receive a dongle.

### 5.1 FIFO Scheduling

**First-In, First-Out** is the simplest scheduling policy. When multiple
coders request the same dongle, the dongle is granted to the coder whose
request arrived earliest. FIFO is inherently fair in the sense that it
prevents starvation: as long as requests are eventually served, every coder
will eventually reach the front of the queue.

FIFO does not consider urgency. A coder with a very tight deadline may be
stuck behind a coder with a generous deadline. In the Codexion simulation,
this could lead to burnout even when a feasible schedule exists.

Implementation: each enqueue draws a globally incrementing **ticket** from a
scheduler-wide counter, and the heap is keyed on the ticket value.

### 5.2 EDF Scheduling

**Earliest Deadline First** is a dynamic priority scheduling algorithm used in
real-time systems. Each coder has a deadline equal to `time of its last
successful dongle acquisition + time_to_burnout` (initially `start_of_run +
time_to_burnout`). When a dongle becomes available, EDF grants it to the coder
with the earliest deadline. The deadline is captured at the moment the coder
makes its request; it only changes when the coder next acquires dongles, at
which point it re-enqueues a fresh request, so no in-place priority update is
ever needed.

EDF is optimal for uniprocessor real-time scheduling: if any scheduling
algorithm can meet all deadlines, EDF can. However, this optimality holds only
under certain conditions, such as when the system is not overloaded. In the
Codexion simulation, EDF is designed to guarantee liveness: no coder should
starve of dongles and burn out, provided the parameters are feasible.

### 5.3 Implementation Considerations

Both FIFO and EDF require a priority queue. The heap supports insertion and
extraction of the highest-priority element in O(log n), where n is the number
of pending requests (which is at most 2 per dongle, since each dongle has only
two neighbours). Because a coder's deadline only changes when it acquires
dongles again (and therefore re-enqueues), no in-place priority update is
required. The queue is protected by the dongle's mutex: enqueue and dequeue
operations are atomic with respect to dongle state changes.

Because the heap size is bounded at 2, its memory is embedded directly inside
the `t_dongle` struct — no dynamic allocation per dongle.

## 6. Resource Management: Dongle Cooldown

A distinctive feature of the Codexion simulation is the **dongle cooldown**.
After a dongle is released by a coder, it cannot be acquired by any coder
until `dongle_cooldown` milliseconds have elapsed. This models a physical or
administrative delay — for example, the time required for a hardware dongle
to be reset or re-registered.

Cooldown introduces a subtle challenge. During the cooldown period, the dongle
is unavailable, but it is not held by any coder. If the cooldown is not
properly enforced, a coder could acquire a dongle that is still cooling down,
violating the simulation's rules. The implementation tracks the release time
of each dongle (`available_at`) and compares it against the current time in
`is_ready`.

The cooldown also interacts with scheduling. If a dongle is in cooldown,
pending requests must wait. Rather than signalling only once the cooldown has
expired, the releasing thread broadcasts immediately, and the head coder uses
a timed wait (`pthread_cond_timedwait`) that targets `available_at`; this
reaches the exact expiry instant without polling.

## 7. Burnout Detection and Liveness

### 7.1 Burnout as a Deadline Violation

In the Codexion simulation, **burnout** occurs when a coder fails to begin
compiling within `time_to_burnout` milliseconds since their last successful
compile start or the start of the simulation. This is a **deadline
violation**. The simulation must detect burnout precisely and log it within
10 milliseconds of the actual burnout time.

Burnout detection is the responsibility of a dedicated **monitor thread**.
The monitor polls every millisecond, examines each coder's stored burnout
deadline under mutex protection, and computes whether it has passed. If a
deadline is violated, the monitor logs the burnout and signals the simulation
to terminate.

A coder that has already started compiling is never declared burned out
during that compile — the monitor only considers a deadline expired when
`is_compiling == false`. Burnout is therefore only reported when a coder fails
to *start* a compile in time.

### 7.2 Liveness Guarantees

**Liveness** is the property that something good eventually happens — in this
case, that every coder eventually compiles and the simulation terminates. The
Codexion simulation must guarantee liveness under the EDF scheduler: no coder
should be starved of dongles and burn out if the parameters are feasible.

Liveness is ensured through several mechanisms:

- **Fair arbitration**: FIFO ensures that requests are served in order. EDF
  ensures that the most urgent requests are served first.
- **Deadlock prevention**: The atomic-both acquisition mechanism avoids
  hold-and-wait, which rules out circular-wait deadlock.
- **Cooldown handling**: The cooldown is enforced consistently, preventing
  dongles from being acquired prematurely.
- **Precise burnout detection**: The monitor thread ensures that the
  simulation stops promptly when a coder burns out, preventing other coders
  from waiting indefinitely.
- **Deterministic shutdown**: A coder sets its `finished` flag only after
  completing its last full cycle (compile + debug + refactor). The monitor
  stops the simulation only when every coder has `finished == true`, so no
  final phase line is ever dropped.

### 7.3 The Monitor Thread

The monitor thread operates concurrently with the coder threads. It reads
shared state — specifically each coder's `finished` and `is_compiling` flags
and its `burnout_time` deadline — under the coder's own mutex, to avoid stale
reads. It also checks the scheduler's termination flag to know when to exit.

The monitor thread is essential for two reasons. First, a coder that is
blocked waiting for a dongle cannot examine its own deadline without polling;
only the monitor, which is never blocked on a dongle, can observe that the
deadline has passed. Second, the monitor ensures that the simulation
terminates in a timely manner, both on burnout (as soon as any coder is
detected) and on completion (as soon as every coder is `finished`).

## 8. Summary of Key Concepts

| Concept | Relevance to Codexion |
| - | - |
| Thread | Each coder is a thread; concurrency is inherent. |
| Shared memory | Dongles, coder state, and logs are shared across threads. |
| Mutex | Protects dongle state and serializes logging. |
| Condition variable | Wakes waiting coders when a dongle becomes available. |
| Deadlock | Prevented by breaking hold-and-wait via atomic both-or-nothing acquisition. |
| Starvation | Prevented by FIFO fairness and EDF urgency prioritization. |
| Coffman conditions | Analyzed; hold-and-wait is broken by design. |
| FIFO scheduling | Serves requests in arrival (ticket) order; fair but not urgency-aware. |
| EDF scheduling | Serves the most urgent request first; optimal for deadlines. |
| Priority queue | Efficiently orders waiting requests for both schedulers. |
| Cooldown | Enforces a delay between dongle release and reacquisition. |
| Burnout | A deadline violation detected by the monitor thread. |
| Liveness | Guaranteed by fair scheduling and deadlock prevention. |
| Monitor thread | Detects burnout and terminates the simulation. |

The Codexion simulation is a rich exercise in concurrent programming. It
requires not only the mechanical skills of using `pthread_create`,
`pthread_mutex_lock`, and `pthread_cond_wait`, but also a deep understanding
of why these primitives exist and how they interact. The challenges of
deadlock, starvation, race conditions, and liveness are not abstract — they
manifest concretely in the behaviour of the simulation. A correct solution
must demonstrate both technical proficiency and conceptual clarity.
