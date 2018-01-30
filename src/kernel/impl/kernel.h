#pragma once

#include <stdint.h>
#include "kernel/types.h"
#include "pmemmgr.h"
#include "cpu.h"
#include "sched.h"

/*
 * TODO: deal with pointers in an elegant manner
 *
 * I'm still debating on the idea of allowing direct access to structs in RAM
 * vs. having a layer that copies data between the host and guest systems.
 * The current approach relies on the usage of dumb pointers in Xbox structs
 * and involves careful usage of the _PTR_TO_ADDR and _ADDR_TO_PTR macros.
 * It kind of gets the job done, but has a major flaw: it fails to handle
 * addresses outside of the physical memory range (0x00000000 - 0x03ffffff).
 *
 * As it happens, the official kernel has a tendency to hide internal structs
 * in the upper half of the virtual memory (0x80000000 and above), so that
 * approach might not work in all cases. Despite that, kernel structs *are*
 * stored in physical memory; accessing them directly probably won't cause any
 * trouble, as long as the game code doesn't decide to provide kernel structs
 * with pointer addresses outside of the physical range to our functions.
 *
 * -----
 *
 * Direct access has the advantage of offering maximum performance, since it
 * completely avoids memory copying. However, it is difficult to deal with
 * addresses and pointers and so on, especially considering that some of them
 * might actually be outside of the physical memory range. It requires the use
 * of dumb pointers and constant translation between pointers to m_ram and
 * actual memory addresses.
 *
 * Copying the data has a big performance penalty and is quite painful to
 * implement because each and every struct needs to be handled manually,
 * including copying additional data from pointers to other structs or values,
 * as well as allocating and freeing the memory for the copies, but once done
 * the kernel functions can manipulate the data without converting pointers.
 * After the kernel is done working on them, the pointers need to be translated
 * back into the corresponding Xbox RAM addresses (which, again, needs to be
 * done for each and every struct type).
 *
 * If copying the data is the approach to be taken, then all the types defined
 * in types.h will need to be refactored to use actual pointers instead of dumb
 * pointers.
 */

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

	int Run();
	void SaveCPUContext();

	Thread *CreateThread(uint32_t entryAddress, uint32_t stackSize);
	int ScheduleThread(Thread *thread);
	int ScheduleNewThread(uint32_t entryAddress, uint32_t stackSize);
	PhysicalMemoryBlock *ReserveMemory(uint32_t baseAddress, uint32_t size, uint32_t protect);

	// ------------------------------------------------------------------------
	// Kernel function implementations
	// ------------------------------------------------------------------------

	// Memory manager (Mm)
	XboxTypes::PVOID MmAllocateContiguousMemory(XboxTypes::SIZE_T NumberOfBytes);
	XboxTypes::PVOID MmAllocateContiguousMemoryEx(XboxTypes::SIZE_T NumberOfBytes, XboxTypes::ULONG_PTR LowestAcceptableAddress, XboxTypes::ULONG_PTR HighestAcceptableAddress, XboxTypes::ULONG_PTR Alignment, XboxTypes::ULONG Protect);
	XboxTypes::VOID MmFreeContiguousMemory(XboxTypes::PVOID BaseAddress);
	XboxTypes::VOID MmPersistContiguousMemory(XboxTypes::PVOID BaseAddress, XboxTypes::SIZE_T NumberOfBytes, XboxTypes::BOOLEAN Persist);
	XboxTypes::ULONG MmQueryAddressProtect(XboxTypes::PVOID VirtualAddress);
	XboxTypes::SIZE_T MmQueryAllocationSize(XboxTypes::PVOID BaseAddress);
	XboxTypes::VOID MmSetAddressProtect(XboxTypes::PVOID BaseAddress, XboxTypes::ULONG NumberOfBytes, XboxTypes::ULONG NewProtect);

	// NT (Nt)
	XboxTypes::NTSTATUS NtAllocateVirtualMemory(XboxTypes::PPVOID BaseAddress, XboxTypes::ULONG_PTR ZeroBits, XboxTypes::PSIZE_T RegionSize, XboxTypes::ULONG AllocationType, XboxTypes::ULONG Protect);

private:
	char *m_ram;
	size_t m_ramSize;
	Cpu *m_cpu;
	Scheduler *m_sched;
	PhysicalMemoryManager *m_pmemmgr;

	/*!
	 * Initializes the GDT and data structures referenced by it.
	 */
	int InitializeGDT();
};
