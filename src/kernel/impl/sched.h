#pragma once

#include "cpu.h"
#include "pmemmgr.h"
#include "kernel/types.h"
#include "kernel/impl/thread.h"
#include <string.h>
#include <vector>

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
	XboxTypes::KPCR       *m_kpcr; // pointer to Xbox RAM, MUST NOT BE FREED

public:
    Scheduler(Cpu *cpu);
    ~Scheduler();
	void SetKPCR(XboxTypes::KPCR *kpcr);
    int ScheduleThread(Thread *thread);
    int Run();
	bool ChooseNextThread();
	void SaveCPUContext();
};
