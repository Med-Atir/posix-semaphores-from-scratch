# POSIX Semaphores from Scratch (`P` and `V`)

A from-scratch C implementation of Edsger W. Dijkstra's semaphores (`P` and `V`) on Linux to solve the classic **Producer-Consumer** problem **without using native OS synchronization primitives** (`<semaphore.h>`, `pthread_mutex_t`, `pthread_cond_t`, etc.).

This repository explores how the choice of execution paradigm shapes memory management, task suspension via POSIX signals, and race conditions on modern multi-core CPUs through two distinct architectures:
1. **Multi-Process Architecture** (`fork` + `mmap` + `SIGSTOP`/`SIGCONT`)
2. **Multi-Thread Architecture** (`pthread` + `malloc` + `pthread_sigmask`/`sigwait`)

---

## Project Structure

```text
.
├── process_version/
│   ├── main.c                      # Shared memory allocation (mmap) & process orchestration (fork/wait)
│   ├── p1.c / p1.h                 # Producer routine
│   ├── p2.c / p2.h                 # Consumer routine
│   └── semaphore.c / semaphore.h   # Static FIFO queue & P/V primitives (SIGSTOP/SIGCONT)
│
├── thread_version/
│   ├── main_2.c                    # Heap allocation (malloc), signal masking & thread creation
│   ├── p1_2.c / p1_2.h             # Producer thread routine
│   ├── p2_2.c / p2_2.h             # Consumer thread routine
│   └── semaphore_2.c / semaphore_2.h # Dynamic linked-list FIFO & P/V primitives (sigwait)
│
└── report/
    └── TP_Semaphore.pdf            # Technical report (in French)
```

---

## Architectural Comparison

| Feature | Multi-Process Version (`fork`) | Multi-Thread Version (`pthread`) |
| :--- | :--- | :--- |
| **Memory Sharing** | Explicit anonymous shared memory (`mmap` with `MAP_SHARED \| MAP_ANONYMOUS`) to bypass Copy-on-Write | Native shared address space (heap allocation via `malloc` / `free`) |
| **Waiting Queue** | Fixed-size static array (`pid[MAX]`) to avoid invalid private pointers across processes | Dynamic singly-linked list (`Maillon` with `first` and `last` pointers) in $O(1)$ |
| **Sleep Mechanism** | Self-suspension via `kill(getpid(), SIGSTOP)` (`TASK_STOPPED`) | Synchronous wait via `sigwait(&set, &sig)` with `SIGUSR1` blocked |
| **Wakeup Mechanism** | `kill(pid, SIGCONT)` | `pthread_kill(tid, SIGUSR1)` |
| **Lost Wakeup Risk** | Possible in the tiny window between `emfiler()` and `kill(p, SIGSTOP)` | Eliminated (`SIGUSR1` is masked beforehand via `pthread_sigmask`) |

---

## Compilation & Execution

### 1. Build Both Versions

```bash
# Multi-process version
gcc -Wall -Wextra process_version/main.c process_version/p1.c process_version/p2.c process_version/semaphore.c -o prod_cons_proc

# Multi-thread version
gcc -Wall -Wextra -pthread thread_version/main_2.c thread_version/p1_2.c thread_version/p2_2.c thread_version/semaphore_2.c -o prod_cons_thread
```

### 2. Run on a Single CPU Core (`taskset`)

Because user-space `P()` and `V()` operations (`(*s)--` followed by `emfiler()`) are non-atomic without hardware atomic instructions (`<stdatomic.h>`), running on multiple cores simultaneously can trigger a race condition between decrementing the counter and enqueuing the caller.

To reproduce the single-processor environment of Dijkstra's 1965 **THE** multiprogramming system (Electrologica X8), pin execution to logical core `0` using `taskset`:

```bash
taskset -c 0 ./prod_cons_proc
# or
taskset -c 0 ./prod_cons_thread
```

---

## Key Takeaways & Limitations

- **Why Two Semaphores (`plein` and `vide`)?** A single counter cannot simultaneously block the producer when the buffer is full and the consumer when the buffer is empty. Cross-signaling (`P(&plein)` / `V(&vide)` in the producer, `P(&vide)` / `V(&plein)` in the consumer) prevents deadlocks.
- **No Busy Waiting:** Unlike Dekker's or Peterson's algorithms, blocked tasks yield the CPU completely (`TASK_STOPPED` or `sigwait`) instead of spinning.
- **Future Improvements:**
  - Protect critical sections inside `P()` and `V()` with C11 atomics (`<stdatomic.h>` / spinlocks) for multi-core safety.
  - Assign a dedicated FIFO queue per semaphore to scale from 1 Producer / 1 Consumer to $N$ Producers / $M$ Consumers.

---

## Author

**Mohammed Atir** — Engineering Student in Information Systems Security & Cyberdefense (ISSIC), École des Sciences de l'Information (ESI), Rabat.
