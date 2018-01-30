#pragma once

#include <stdint.h>
#include "kernel/types.h"
#include "pmemmgr.h"
#include "cpu.h"
#include "sched.h"


/*!
 * A custom implementation of the Xbox kernel that runs on the host machine and
 * manages kernel objects in the virtual Xbox machine.
 *
 * Just like a real kernel, this class handles the low-level mechanisms between
 * applications and the hardware, except that it runs outside of the emulated
 * environment. The goal is to respond to kernel function invocations and
 * manage the Xbox hardware so that the emulated applications believe they're
 * running on a real kernel.
 */
class XboxKernel {
public:
	XboxKernel(char *ram, size_t ramSize, Cpu *cpu);

	int Initialize();

	Thread *CreateThread(uint32_t entryAddress, uint32_t stackSize);
	int ScheduleThread(Thread *thread);
	int ScheduleNewThread(uint32_t entryAddress, uint32_t stackSize);
	PhysicalMemoryBlock *ReserveMemory(uint32_t baseAddress, uint32_t size, uint32_t protect);

	// FIXME: public for now, to get things to compile
	PhysicalMemoryManager *m_pmemmgr;
	Scheduler *m_sched;

private:
	char *m_ram;
	size_t m_ramSize;
	Cpu *m_cpu;
	
	/*!
	 * Initializes the GDT and data structures referenced by it.
	 */
	int InitializeGDT();

	/*!
	 * Converts an address into a pointer to data in the physical Xbox memory,
	 * allowing direct manipulation of data.
	 *
	 * Returns true if the address points to a valid region in the physical
	 * memory, updating the pointer to the corresponding address of the data.
	 */
	template<class T> bool ToPointer(uint32_t address, T** pointer) {
		if (address + sizeof(T) <= m_ramSize) {
			*pointer = (T*)m_ram[address];
			return true;
		}
		return false;
	}

	/*!
	 * Converts a a pointer to data in Xbox memory into an address.
	 *
	 * Returns true if the pointer is contained within the Xbox memory region,
	 * updating the contents of the address argument to the value
	 * corresponding to the physical address of the data in Xbox RAM.
	 */
	template<class T> bool ToAddress(T* pointer, uint32_t *address) {
		char *p = (char *)pointer;
		if (p >= m_ram && p < m_ram + m_ramSize) {
			*address = (uint32_t)((char *)pointer - m_ram);
			return true;
		}
		return false;
	}
};
