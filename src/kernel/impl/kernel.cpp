#include <assert.h>
#include "kernel/impl/kernel.h"
#include "kernel/impl/kernel_ki.h"
#include "kernel/impl/gdt.h"
#include "kernel/impl/tss.h"
#include "kernel/impl/tls.h"
#include "log.h"

XboxKernel::XboxKernel(char *ram, size_t ramSize, Cpu *cpu)
	: m_ram(ram)
	, m_ramSize(ramSize)
	, m_cpu(cpu)
{
}

XboxKernel::~XboxKernel() {
	// These objects must be deleted in this order
	delete m_objmgr;
	delete m_pmemmgr;
	delete m_sched;
}

int XboxKernel::Initialize(GThreadFunc emuThreadFunc, gpointer emuThreadData) {
	// Initialize scheduler
	log_debug("Initializing Scheduler\n");
	m_sched = new Scheduler(m_cpu, emuThreadFunc, emuThreadData);
	assert(m_sched != NULL);

	// Initialize memory manager
	log_debug("Initializing Memory Manager\n");
	m_pmemmgr = new PhysicalMemoryManager(XBOX_RAM_SIZE);
	assert(m_pmemmgr != NULL);

	// Initialize GDT
	int gdtStatus = InitializeGDT();
	if (gdtStatus != 0) {
		return gdtStatus;
	}

	// Initialize the idle and system processes
	int procStatus = InitializeKernel();
	if (procStatus != 0) {
		return procStatus;
	}

	// Initialize object manager
	log_debug("Initializing Object Manager\n");
	m_objmgr = new ObjectManager();
	assert(m_objmgr != NULL);

	return 0;
}

int XboxKernel::InitializeGDT() {
	GDTEntry gdtTable[7];

	log_debug("Initializing GDT\n");

	// Index  Offset  Description          Segment regs.   Base         Limit     Access              Flags
	// #0     0x00    unusable             -               0x00000000   0x00000   0b00000000 (0x00)   0b0000 (0x0)
	// #1     0x08    ring-0 code          CS              0x00000000   0xffffb   0b10011011 (0x9b)   0b1100 (0xc)
	// #2     0x10    ring-0 data          DS, ES, SS      0x00000000   0xfffff   0b10010011 (0x93)   0b1100 (0xc)
	// #3     0x18    TSS (current task)*  -               <&tss>       0x00068   0b10001001 (0x89)   0b0000 (0x0)
	// #4     0x20    KPCR                 FS              <&kpcr>      0x00284   0b10010011 (0x93)   0b1100 (0xc)
	// #5     0x28    TSS (double fault)*  -               <&tssDF>     0x00068   0b10001001 (0x89)   0b0000 (0x0)
	// #6     0x30    TSS (NMI)*           -               <&tssNMI>    0x00068   0b10001001 (0x89)   0b0000 (0x0)
	//
	// * informed assumptions based on https://www.geoffchappell.com/studies/windows/km/ntoskrnl/structs/kpcr.htm

	// Allocate memory for the KPCR and TSS data structures
#define ALLOC(var, size) \
	uint32_t var;\
	uint32_t var##_size;\
	{\
		PhysicalMemoryBlock *block = m_pmemmgr->AllocateContiguous((size));\
		if (nullptr == block) {\
			log_debug("Could not allocate memory for " #var "\n");\
			return 1;\
		}\
		var = block->BaseAddress();\
		var##_size = block->Size();\
	}

	ALLOC(kpcrAddr, sizeof(XboxTypes::KPCR));
	ALLOC(tssAddr, sizeof(TSS));
	ALLOC(tssDFAddr, sizeof(TSS));
	ALLOC(tssNMIAddr, sizeof(TSS));
	ALLOC(gdtAddr, sizeof(gdtTable));
#undef ALLOC

	log_debug("  KPCR located at 0x%08x\n", kpcrAddr);
	log_debug("  Current Task TSS located at 0x%08x\n", tssAddr);
	log_debug("  Double Fault TSS located at 0x%08x\n", tssDFAddr);
	log_debug("  NMI TSS located at 0x%08x\n", tssNMIAddr);
	log_debug("  GDT table located at 0x%08x\n", gdtAddr);

	// Fill in TSS data with basic values
	TSS tss;
	memset(&tss, 0, sizeof(TSS));
	tss.SS0 = tss.DS = tss.ES = tss.SS = 0x10;
	tss.CS = 0x08;
	tss.FS = 0x20;

	// Write TSS to memory
	m_cpu->MemWrite(tssAddr, sizeof(TSS), &tss);
	m_cpu->MemWrite(tssDFAddr, sizeof(TSS), &tss);
	m_cpu->MemWrite(tssNMIAddr, sizeof(TSS), &tss);

	// Fill in basic KPCR data directly in memory
	m_pKPCR = _ADDR_TO_PTR(KPCR, kpcrAddr);
	memset(m_pKPCR, 0, sizeof(XboxTypes::KPCR));
	m_pKPCR->SelfPcr = kpcrAddr;
	m_pKPCR->Prcb = _PTR_TO_ADDR(KPCR, &m_pKPCR->PrcbData);
	m_pKPCR->NtTib.Self = _PTR_TO_ADDR(NT_TIB, &m_pKPCR->NtTib);
	m_pKPCR->NtTib.ExceptionList = EXCEPTION_CHAIN_END;
	XboxTypes::KPRCB *pKPRCB = &m_pKPCR->PrcbData;
	InitializeListHead(&pKPRCB->DpcListHead);
	pKPRCB->DpcRoutineActive = 0;
	pKPRCB->CurrentThread = kpcrAddr + sizeof(XboxTypes::KPCR);
	// TODO: fill in just enough data for the emulated code to work properly

	// Fill in GDT table data
	gdtTable[0].Set(0x00000000, 0x00000, 0x00, 0x0);
	gdtTable[1].Set(0x00000000, 0xffffb, 0x9b, 0xc);
	gdtTable[2].Set(0x00000000, 0xfffff, 0x93, 0xc);
	gdtTable[3].Set(tssAddr, 0x00068, 0x89, 0x0);
	gdtTable[4].Set(kpcrAddr, 0x00284, 0x93, 0xc);
	gdtTable[5].Set(tssDFAddr, 0x00068, 0x89, 0x0);
	gdtTable[6].Set(tssNMIAddr, 0x00068, 0x89, 0x0);

	// Write GDT to memory
	m_cpu->MemWrite(gdtAddr, sizeof(gdtTable), gdtTable);
	m_pmemmgr->SetProtect(gdtAddr, gdtAddr_size, PAGE_READONLY);

	// Set GDT register
	m_cpu->SetGDT(gdtAddr, gdtAddr_size);

	// Set segment registers
	m_cpu->RegWrite(REG_CS, 0x08);
	m_cpu->RegWrite(REG_DS, 0x10);
	m_cpu->RegWrite(REG_ES, 0x10);
	m_cpu->RegWrite(REG_SS, 0x10);
	m_cpu->RegWrite(REG_FS, 0x20);
	m_cpu->RegWrite(REG_GS, 0x00);

	// Point scheduler to the newly created KPCR
	m_sched->SetKPCR(m_pKPCR);

	return 0;
}

int XboxKernel::InitializeKernel() {
	// TODO: KfLowerIrql(APC_LEVEL);
	KiInitSystem();

	PhysicalMemoryBlock *block = m_pmemmgr->AllocateContiguous(sizeof(XboxTypes::KPROCESS) * 2);
	if (nullptr == block) {
		log_debug("Could not allocate memory for the KPROCESS objects\n");
		return 1;
	}
	m_KiIdleProcess = block->BaseAddress();
	m_KiSystemProcess = m_KiIdleProcess + sizeof(XboxTypes::KPROCESS);
	m_pKiIdleProcess = ToPointer<XboxTypes::KPROCESS>(m_KiIdleProcess);
	m_pKiSystemProcess = ToPointer<XboxTypes::KPROCESS>(m_KiSystemProcess);

	KeInitializeProcess(m_KiIdleProcess, (XboxTypes::KPRIORITY)0);
	m_pKiIdleProcess->ThreadQuantum = CHAR_MAX;

	KeInitializeProcess(m_KiSystemProcess, NORMAL_BASE_PRIORITY);
	m_pKiIdleProcess->ThreadQuantum = THREAD_QUANTUM;

	// FIXME: incomplete

	if (m_pKPCR->PrcbData.NextThread == NULL) {
		SetMember(0, m_KiIdleSummary);
	}

	KfRaiseIrql(HIGH_LEVEL);

	return 0;
}


int XboxKernel::Run() {
	return m_sched->Run();
}

void XboxKernel::SaveCPUContext() {
	m_sched->SaveCPUContext();
}

/*!
 * Creates a new thread, allocating the stack in the main memory.
 */
Thread *XboxKernel::CreateThread(uint32_t entryAddress, uint32_t stackSize) {
	// TODO: Should probably differentiate between system and user threads
	log_debug("Creating a thread with entry address 0x%08x and stack size 0x%x\n", entryAddress, stackSize);

	// Allocate memory for the thread stack
	// TODO: check Protect flags
	PhysicalMemoryBlock *threadStack = m_pmemmgr->AllocateContiguous(stackSize, IMAGE_BASE_ADDRESS, ULONG_MAX, PAGE_SIZE, PAGE_READWRITE);
	if (nullptr == threadStack) {
		log_debug("Could not allocate memory for the thread stack!\n");
		return nullptr;
	}
	log_debug("...stack allocated at 0x%08x\n", threadStack->BaseAddress());

	// Allocate memory for the KTHREAD struct and TLS data array
	PhysicalMemoryBlock *kthreadBlock = m_pmemmgr->AllocateContiguous(sizeof(XboxTypes::KTHREAD) + TLS_SIZE);
	if (nullptr == kthreadBlock) {
		log_debug("Could not allocate memory for KTHREAD + TLS table!\n");
		delete threadStack;
		return nullptr;
	}
	log_debug("...KTHREAD + TLS table allocated at 0x%08x\n", kthreadBlock->BaseAddress());

	XboxTypes::PKTHREAD pkthread = kthreadBlock->BaseAddress();
	XboxTypes::KTHREAD *pThread = ToPointer<XboxTypes::KTHREAD>(pkthread);
	KeInitializeThread(pkthread,
		threadStack->BaseAddress() + threadStack->TotalSize(),
		threadStack->TotalSize(),
		TLS_SIZE,
		NULL, // FIXME: almost certainly incorrect
		NULL, // FIXME: almost certainly incorrect
		NULL, // FIXME: almost certainly incorrect
		m_KiSystemProcess
	);

	return new Thread(entryAddress, threadStack, pkthread, pThread);
}

/*!
 * Schedule a thread for execution
 */
int XboxKernel::ScheduleThread(Thread *thread) {
	return m_sched->ScheduleThread(thread);
}

int XboxKernel::ScheduleNewThread(uint32_t entryAddress, uint32_t stackSize) {
	return ScheduleThread(CreateThread(entryAddress, stackSize));
}

/*!
 * Reserves a region of memory.
 */
PhysicalMemoryBlock *XboxKernel::ReserveMemory(uint32_t baseAddress, uint32_t size, uint32_t protect) {
	return m_pmemmgr->Reserve(baseAddress, size, protect);
}
