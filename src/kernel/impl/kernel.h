#pragma once

#include <stdint.h>
#include "kernel/types.h"
#include "pmemmgr.h"
#include "cpu.h"
#include "sched.h"
#ifdef _WIN32
	#include "kernel/undef_win.h"
#endif

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
 *
 * -----
 *
 * Another (somewhat farfetched) solution is to write x86-32 machine code for
 * kernel functions that don't need access to host resources (such as files)
 * directly into the Xbox RAM. We would need to reserve a portion of memory for
 * that and make modifications to the kernel thunk mapper to not intercept
 * calls made to those functions, but instead point them to the right spot in
 * the binary image.
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
	void SaveCPUContext(); // FIXME: shouldn't be necessary

	Thread *CreateThread(uint32_t entryAddress, uint32_t stackSize);
	int ScheduleThread(Thread *thread);
	int ScheduleNewThread(uint32_t entryAddress, uint32_t stackSize);
	PhysicalMemoryBlock *ReserveMemory(uint32_t baseAddress, uint32_t size, uint32_t protect);

	// ------------------------------------------------------------------------
	// Kernel function implementations
	// ------------------------------------------------------------------------

	// Audio and video support functions (Av)
	XboxTypes::VOID AvSendTVEncoderOption(XboxTypes::PVOID RegisterBase, XboxTypes::ULONG Option, XboxTypes::ULONG Param, XboxTypes::PULONG Result);
	XboxTypes::ULONG AvSetDisplayMode(XboxTypes::PVOID RegisterBase, XboxTypes::ULONG Step, XboxTypes::ULONG DisplayMode, XboxTypes::ULONG SourceColorFormat, XboxTypes::ULONG Pitch, XboxTypes::ULONG FrameBuffer);

	// Debugger support functions (Dbg)

	// Executive (Ex)
	XboxTypes::NTSTATUS ExQueryNonVolatileSetting(XboxTypes::ULONG ValueIndex, XboxTypes::PULONG Type, XboxTypes::PVOID Value, XboxTypes::ULONG ValueLength, XboxTypes::PULONG ResultLength);

	// File system cache (Fsc)

	// Hardware abstraction layer (Hal)
	XboxTypes::ULONG HalGetInterruptVector(XboxTypes::ULONG BusInterruptLevel, XboxTypes::PKIRQL Irql);
	XboxTypes::VOID HalReturnToFirmware(XboxTypes::FIRMWARE_REENTRY Routine);

	// I/O manager (Io/Iof)
	XboxTypes::NTSTATUS IoCreateSymbolicLink(XboxTypes::POBJECT_STRING SymbolicLinkName, XboxTypes::POBJECT_STRING DeviceName);

	// Kernel core (Ke) and internal kernel functions (Kf/Ki)
	XboxTypes::VOID KeBugCheck(XboxTypes::ULONG BugCheckCode);
	XboxTypes::VOID KeBugCheckEx(XboxTypes::ULONG BugCheckCode, XboxTypes::ULONG_PTR BugCheckParameter1, XboxTypes::ULONG_PTR BugCheckParameter2, XboxTypes::ULONG_PTR BugCheckParameter3, XboxTypes::ULONG_PTR BugCheckParameter4);
	XboxTypes::BOOLEAN KeConnectInterrupt(XboxTypes::PKINTERRUPT Interrupt);
	XboxTypes::VOID KeInitializeApc(XboxTypes::PRKAPC Apc, XboxTypes::PRKTHREAD Thread, XboxTypes::PKKERNEL_ROUTINE KernelRoutine, XboxTypes::PKRUNDOWN_ROUTINE RundownRoutine, XboxTypes::PKNORMAL_ROUTINE NormalRoutine, XboxTypes::KPROCESSOR_MODE ProcessorMode, XboxTypes::PVOID NormalContext);
	XboxTypes::VOID KeInitializeDeviceQueue(XboxTypes::PKDEVICE_QUEUE DeviceQueue);
	XboxTypes::VOID KeInitializeDpc(XboxTypes::PKDPC Dpc, XboxTypes::PKDEFERRED_ROUTINE DeferredRoutine, XboxTypes::PVOID DeferredContext);
	XboxTypes::VOID KeInitializeEvent(XboxTypes::PRKEVENT Event, XboxTypes::EVENT_TYPE Type, XboxTypes::BOOLEAN State);
	XboxTypes::VOID KeInitializeInterrupt(XboxTypes::PKINTERRUPT Interrupt, XboxTypes::PKSERVICE_ROUTINE ServiceRoutine, XboxTypes::PVOID ServiceContext, XboxTypes::ULONG Vector, XboxTypes::KIRQL Irql, XboxTypes::KINTERRUPT_MODE InterruptMode, XboxTypes::BOOLEAN ShareVector);
	XboxTypes::VOID KeInitializeQueue(XboxTypes::PRKQUEUE Queue, XboxTypes::ULONG Count);
	XboxTypes::VOID KeInitializeSemaphore(XboxTypes::PRKSEMAPHORE Semaphore, XboxTypes::LONG Count, XboxTypes::LONG Limit);
	XboxTypes::VOID KeInitializeTimerEx(XboxTypes::PKTIMER Timer, XboxTypes::TIMER_TYPE Type);
	XboxTypes::BOOLEAN KeSetTimer(XboxTypes::PKTIMER Timer, XboxTypes::LARGE_INTEGER DueTime, XboxTypes::PKDPC Dpc);
	XboxTypes::BOOLEAN KeSetTimerEx(XboxTypes::PKTIMER Timer, XboxTypes::LARGE_INTEGER DueTime, XboxTypes::LONG Period, XboxTypes::PKDPC Dpc);

	// Memory manager (Mm)
	XboxTypes::PVOID MmAllocateContiguousMemory(XboxTypes::SIZE_T NumberOfBytes);
	XboxTypes::PVOID MmAllocateContiguousMemoryEx(XboxTypes::SIZE_T NumberOfBytes, XboxTypes::ULONG_PTR LowestAcceptableAddress, XboxTypes::ULONG_PTR HighestAcceptableAddress, XboxTypes::ULONG_PTR Alignment, XboxTypes::ULONG Protect);
	XboxTypes::PVOID MmClaimGpuInstanceMemory(XboxTypes::SIZE_T NumberOfBytes, XboxTypes::PSIZE_T NumberOfPaddingBytes);
	XboxTypes::VOID MmFreeContiguousMemory(XboxTypes::PVOID BaseAddress);
	XboxTypes::VOID MmPersistContiguousMemory(XboxTypes::PVOID BaseAddress, XboxTypes::SIZE_T NumberOfBytes, XboxTypes::BOOLEAN Persist);
	XboxTypes::ULONG MmQueryAddressProtect(XboxTypes::PVOID VirtualAddress);
	XboxTypes::SIZE_T MmQueryAllocationSize(XboxTypes::PVOID BaseAddress);
	XboxTypes::VOID MmSetAddressProtect(XboxTypes::PVOID BaseAddress, XboxTypes::ULONG NumberOfBytes, XboxTypes::ULONG NewProtect);

	// Native system services (Nt)
	XboxTypes::NTSTATUS NtAllocateVirtualMemory(XboxTypes::PPVOID BaseAddress, XboxTypes::ULONG_PTR ZeroBits, XboxTypes::PSIZE_T RegionSize, XboxTypes::ULONG AllocationType, XboxTypes::ULONG Protect);
	XboxTypes::NTSTATUS NtClose(XboxTypes::HANDLE Handle);
	XboxTypes::NTSTATUS NtCreateEvent(XboxTypes::PHANDLE EventHandle, XboxTypes::POBJECT_ATTRIBUTES ObjectAttributes, XboxTypes::EVENT_TYPE EventType, XboxTypes::BOOLEAN InitialState);
	XboxTypes::NTSTATUS NtCreateFile(XboxTypes::PHANDLE FileHandle, XboxTypes::ACCESS_MASK DesiredAccess, XboxTypes::POBJECT_ATTRIBUTES ObjectAttributes, XboxTypes::PIO_STATUS_BLOCK IoStatusBlock, XboxTypes::PLARGE_INTEGER AllocationSize, XboxTypes::ULONG FileAttributes, XboxTypes::ULONG ShareAccess, XboxTypes::ULONG CreateDisposition, XboxTypes::ULONG CreateOptions);
	XboxTypes::NTSTATUS NtDeviceIoControlFile(XboxTypes::HANDLE FileHandle, XboxTypes::HANDLE Event, XboxTypes::PIO_APC_ROUTINE ApcRoutine, XboxTypes::PVOID ApcContext, XboxTypes::PIO_STATUS_BLOCK, XboxTypes::ULONG IoControlCode, XboxTypes::PVOID InputBuffer, XboxTypes::ULONG InputBufferLength, XboxTypes::PVOID OutputBuffer, XboxTypes::ULONG OutputBufferLength);
	XboxTypes::NTSTATUS NtOpenFile(XboxTypes::PHANDLE FileHandle, XboxTypes::ACCESS_MASK DesiredAccess, XboxTypes::POBJECT_ATTRIBUTES ObjectAttributes, XboxTypes::PIO_STATUS_BLOCK IoStatusBlock, XboxTypes::ULONG ShareAccess, XboxTypes::ULONG OpenOptions);
	XboxTypes::NTSTATUS NtQueryVolumeInformationFile(XboxTypes::HANDLE FileHandle, XboxTypes::PIO_STATUS_BLOCK IoStatusBlock, XboxTypes::PVOID FsInformation, XboxTypes::ULONG Length, XboxTypes::FS_INFORMATION_CLASS FsInformationClass);

	// Object manager (Ob/Obf)

	// Ethernet functions (Phy)

	// Process management support functions (Ps)
	XboxTypes::NTSTATUS PsCreateSystemThreadEx(
		XboxTypes::PHANDLE ThreadHandle,
		XboxTypes::SIZE_T ThreadExtensionSize,
		XboxTypes::SIZE_T KernelStackSize,
		XboxTypes::SIZE_T TlsDataSize,
		XboxTypes::PHANDLE ThreadId,
		XboxTypes::PKSTART_ROUTINE StartRoutine,
		XboxTypes::PVOID StartContext,
		XboxTypes::BOOLEAN CreateSuspended,
		XboxTypes::BOOLEAN DebuggerThread,
		XboxTypes::PKSYSTEM_ROUTINE SystemRoutine
	);

	// Runtime Library functions (Rtl)
	XboxTypes::SIZE_T RtlCompareMemory(XboxTypes::PVOID Source1, XboxTypes::PVOID Source2, XboxTypes::SIZE_T Length);
	XboxTypes::SIZE_T RtlCompareMemoryUlong(XboxTypes::PVOID Source, XboxTypes::SIZE_T Length, XboxTypes::ULONG Pattern);
	XboxTypes::BOOLEAN RtlEqualString(XboxTypes::PSTRING String1, XboxTypes::PSTRING String2, XboxTypes::BOOLEAN CaseInSensitive);
	XboxTypes::VOID RtlFillMemory(XboxTypes::PVOID Destination, XboxTypes::ULONG Length, XboxTypes::UCHAR Fill);
	XboxTypes::VOID RtlFillMemoryUlong(XboxTypes::PVOID Destination, XboxTypes::SIZE_T Length, XboxTypes::ULONG Pattern);
	XboxTypes::VOID RtlInitAnsiString(XboxTypes::PANSI_STRING DestinationString, XboxTypes::PCSZ SourceString);
	XboxTypes::VOID RtlInitializeCriticalSection(XboxTypes::PRTL_CRITICAL_SECTION CriticalSection);
	XboxTypes::VOID RtlMoveMemory(XboxTypes::PVOID Destination, XboxTypes::PPVOID Source, XboxTypes::ULONG Length);
	XboxTypes::ULONG RtlNtStatusToDosError(XboxTypes::NTSTATUS Status);
	XboxTypes::ULONG RtlUlongByteSwap(XboxTypes::ULONG Source);
	XboxTypes::USHORT RtlUshortByteSwap(XboxTypes::USHORT Source);
	XboxTypes::VOID RtlZeroMemory(XboxTypes::PVOID Destination, XboxTypes::SIZE_T Length);

	// Xbox cryptography functions (Xc)

	// Xbox executable support functions (Xe)

	// Miscellaneous functions

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

	/*!
	 * Converts an address into a pointer to data in the physical Xbox memory,
	 * allowing direct manipulation of data.
	 *
	 * WARNING: The function does not perform bounds check nor does it map
	 * virtual addresses to physical addresses before obtaining the pointer.
	 * FIXME
	 */
	template<class T> T* ToPointer(uint32_t address) {
		return (T*)(address + m_ram);
	}
};
