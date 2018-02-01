#pragma once

#include "cpu.h"
#include "pmemmgr.h"
#include "kernel/types.h"
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
    PhysicalMemoryBlock *m_stack;
	XboxTypes::PKTHREAD m_pkthread;
	XboxTypes::KTHREAD *m_kthread;  // pointer to Xbox RAM, MUST NOT BE FREED
	CpuContext m_context;
	uint32_t m_id;

    Thread(uint32_t entry, PhysicalMemoryBlock *stack, XboxTypes::PKTHREAD pkthread, XboxTypes::KTHREAD *kthread);
    ~Thread();
};
