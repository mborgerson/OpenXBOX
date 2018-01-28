#pragma once

#include "cpu.h"
#include "memmgr.h"
#include <string.h>
#include <vector>

/*!
 * Emulated thread
 * 
 * This class encapsulates a basic thread structure for the Xbox CPU.
 */
class Thread {
public:
    uint32_t m_entry;
    ContiguousMemoryBlock *m_stack;
    CpuContext m_context;
	uint32_t m_id;

    Thread(uint32_t entry, ContiguousMemoryBlock *stack);
    ~Thread();
};

#define SCHEDULER_EXIT_ERROR    (-1)
#define SCHEDULER_EXIT_HLT        0
#define SCHEDULER_EXIT_EXPIRE     1
#define SCHEDULER_EXIT_NOTHREADS  2

/*!
 * CPU Scheduler
 * 
 * Responsible for switching the CPU between the various execution threads.
 */
class Scheduler {
protected:
    Cpu                   *m_cpu;
    std::vector<Thread *>  m_threads;
	Thread                *m_currentThread;
	uint32_t               m_nextThreadIndex;

public:
    Scheduler(Cpu *cpu);
    ~Scheduler();
    int ScheduleThread(Thread *thread);
    int Run();
	bool ChooseNextThread();
	void SaveCPUContext();
};
