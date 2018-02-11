#pragma once

#include <glib.h>
#include <string.h>
#include <vector>

#include "cpu.h"
#include "kernel/types.h"
#include "kernel/impl/pmemmgr.h"


// Special return address that marks the end of a thread
#define THREAD_EXIT_RETURN_ADDRESS 0x00000000

class ThreadSuspensionCondition;


/*!
 * Emulated thread
 * 
 * This class encapsulates a basic thread structure for the Xbox CPU.
 */
class Thread {
public:
    PhysicalMemoryBlock *m_stack;
	XboxTypes::PKTHREAD m_pkthread;
	XboxTypes::KTHREAD *m_kthread;  // pointer to Xbox RAM, MUST NOT BE FREED
	CpuContext m_context;
	uint32_t m_id;

	bool m_suspensionSynced; // indicates that there is a thread waiting on the GCond
	GCond m_suspensionSync;
	GMutex m_suspensionMutex;
	ThreadSuspensionCondition *m_suspensionCondition;

    Thread(uint32_t entry, PhysicalMemoryBlock *stack, XboxTypes::PKTHREAD pkthread, XboxTypes::KTHREAD *kthread);
    ~Thread();
};


/*!
 * Represents the condition that caused a thread to be suspended. When the
 * condition evaluates to true, the thread will no longer be suspended.
 */
class ThreadSuspensionCondition {
public:
	/*!
	 * Determines if the condition is met.
	 */
	virtual bool IsMet() = 0;
};

/*!
 * A special condition that is always met, so that threads waiting on this
 * condition are released immediately after a single time slice.
 * This is useful for NtYieldExecution.
 */
class AlwaysTrueTSCondition : public ThreadSuspensionCondition {
public:
	bool IsMet();
};

// TODO: implement other conditions
