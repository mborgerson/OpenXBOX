#pragma once

#include "cpu.h"
#include "kernel/types.h"
#include "kernel/impl/pmemmgr.h"
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
    std::vector<Thread *>  m_activeThreads;
	std::vector<Thread *>  m_suspendedThreads;
	Thread                *m_currentThread;
	uint32_t               m_nextThreadIndex;
	XboxTypes::KPCR       *m_kpcr; // pointer to Xbox RAM, MUST NOT BE FREED

public:
	/*!
	 * Creates a new thread scheduler for the given CPU.
	 */
    Scheduler(Cpu *cpu);

    ~Scheduler();

	/*!
	 * Begin CPU execution.
	 */
	int Run();

	/*!
	 * Defines the pointer to the KPCR struct in Xbox RAM.
	 */
	void SetKPCR(XboxTypes::KPCR *kpcr);

	/*!
	 * Save the CPU context on the current thread.
	 */
	void SaveCPUContext();

	/*!
	 * Schedule a thread for execution.
	 */
	int ScheduleThread(Thread *thread);

	/*!
	 * Suspends execution of the current thread with the given condition.
	 */
	void SuspendThread(ThreadSuspensionCondition *condition);

private:
	/*!
	* Choose a thread to execute.
	*/
	bool ChooseNextThread();
};
