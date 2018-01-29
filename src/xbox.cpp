#include <SDL.h>
#include "xbox.h"
#include "timer.h"
#include "alloc.h"
#include "pmemmgr.h"
#include "kernel/impl/gdt.h"
#include "kernel/impl/tss.h"

#define ENABLE_GDB_SERVER     0 // FIXME: Allow enable from cmdline
#define DUMP_SECTION_CONTENTS 0

// Statically generate a lookup table to quickly find the member function
// given an import id.
typedef int (Xbox::*KernelFuncPtr)();

/*!
 * Global Kernel function handler map entry structure
 */
struct KernelFuncImportMap {
    const char    *name;
    KernelFuncPtr  handler;
};

/*!
 * A generated mapping of all Kernel functions, their name, and handler function
 */
const static struct KernelFuncImportMap kKernelFuncImportMap[379] = {
	#define KERNEL_IMPORT_NULL(ID)       { NULL, NULL },
	#define KERNEL_IMPORT_DATA(ID, NAME) { NULL, NULL },
	#define KERNEL_IMPORT_FUNC(ID, NAME) { #NAME, &Xbox:: NAME },
	KERNEL_IMPORTS
	#undef KERNEL_IMPORT_NULL
	#undef KERNEL_IMPORT_DATA
	#undef KERNEL_IMPORT_FUNC
};

/*!
 * Constructor
 */
Xbox::Xbox()
{
}

/*!
 * Destructor
 */
Xbox::~Xbox()
{
    if (m_cpu) delete m_cpu;
    if (m_ram) vfree(m_ram);
    if (m_mem) delete m_mem;
}

/*!
 * Perform basic system initialization
 */
int Xbox::Initialize()
{
    MemoryRegion *rgn;

    // Initialize 4 GiB address space
    m_mem = new MemoryRegion(MEM_REGION_NONE, 0, 0x100000000ULL, NULL);

    // Create RAM region
    log_debug("Allocating RAM (%d MiB)\n", XBOX_RAM_SIZE >> 20);
    m_ram = (char *)valloc(XBOX_RAM_SIZE);
    assert(m_ram != NULL);
    memset(m_ram, 0, XBOX_RAM_SIZE);
    rgn = new MemoryRegion(MEM_REGION_RAM, 0, XBOX_RAM_SIZE, m_ram);
    assert(rgn != NULL);
    m_mem->AddSubRegion(rgn);

	// Initialize memory manager
	log_debug("Initializing Memory Manager\n");
	m_pmemmgr = new PhysicalMemoryManager(XBOX_RAM_SIZE);

    // Create kernel import function thunk handlers
    log_debug("Initializing Kernel Thunk Handlers\n");
    m_kthunk_table_base = XBOX_KIMPORT_BASE;
    memset(&m_ram[m_kthunk_table_base], 0xf4, 512); // `hlt` instruction

    // Create kernel import data structures
    uint32_t m_kvars_base = m_kthunk_table_base + 512;
    m_kvars = (struct KernelVariables *)&m_ram[m_kvars_base];
    memset(m_kvars, 0, sizeof(struct KernelVariables));

    // Create array to map import id to respective guest address for xbe patching
	#define KERNEL_IMPORT_NULL(ID)       m_import_addrs[ID] = 0;
	#define KERNEL_IMPORT_DATA(ID, NAME) m_import_addrs[ID] = (m_kvars_base + (uint32_t)offsetof(struct KernelVariables, NAME)); printf("Kernel Variable %s is at %08x\n", #NAME, m_import_addrs[ID]);
	#define KERNEL_IMPORT_FUNC(ID, NAME) m_import_addrs[ID] = (m_kthunk_table_base + ID);
	KERNEL_IMPORTS
	#undef KERNEL_IMPORT_NULL
	#undef KERNEL_IMPORT_DATA
	#undef KERNEL_IMPORT_FUNC

    // Initialize CPU
    log_debug("Initializing CPU\n");
#ifdef CPU_UNICORN
    m_cpu = new UnicornCpu();
#else
    #error Need CPU implementation
#endif
    assert(m_cpu != NULL);
    m_cpu->Initialize();

	// Initialize scheduler
    log_debug("Initializing Scheduler\n");
    m_sched = new Scheduler(m_cpu);
    assert(m_sched != NULL);

    // Initialize Video
    log_debug("Initializing Video\n");
    m_video = new Video(m_mem, rgn, m_sched);
    assert(m_video != NULL);
    m_video->Initialize();

    // Allow CPU to update memory map based on device allocation, etc
    m_cpu->MemMap(m_mem);

	// Initialize GDT
	int gdtStatus = InitializeGDT();
	if (gdtStatus != 0) {
		return gdtStatus;
	}

	// GDB Server
#if ENABLE_GDB_SERVER
    log_debug("Starting GDB Server\n");
    m_gdb = new GdbServer(m_cpu, "127.0.0.1", 9269);
    m_gdb->Initialize();
#endif

    return 0;
}

int Xbox::InitializeGDT() {
	GDTEntry gdtTable[7];

	log_debug("Initializing GDT\n");

	// Index  Offset  Description          Segment regs.   Base         Limit     Access              Flags
	// #0     0x00    unusable             -               0x00000000   0x00000   0b00000000 (0x00)   0b0000 (0x0)
	// #1     0x08    ring-0 code          CS              0x00000000   0xffffb   0b10011011 (0x9b)   0b1100 (0xc)
	// #2     0x10    ring-0 data          DS, ES, SS      0x00000000   0xfffff   0b10010011 (0x93)   0b1100 (0xc)
	// #3     0x18    TSS (current task)*  -               <&tss1>      0x00068   0b10001001 (0x89)   0b0000 (0x0)
	// #4     0x20    KPCR                 FS              <&kpcr>      0x00284   0b10010011 (0x93)   0b1100 (0xc)
	// #5     0x28    TSS (double fault)*  -               <&tss2>      0x00068   0b10001001 (0x89)   0b0000 (0x0)
	// #6     0x30    TSS (NMI)*           -               <&tss3>      0x00068   0b10001001 (0x89)   0b0000 (0x0)
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
	XboxTypes::KPCR *pKPCR = _ADDR_TO_PTR(KPCR, kpcrAddr);
	memset(pKPCR, 0, sizeof(XboxTypes::KPCR));
	pKPCR->SelfPcr = kpcrAddr;
	pKPCR->Prcb = _PTR_TO_ADDR(KPCR, &pKPCR->PrcbData);
	pKPCR->NtTib.Self = _PTR_TO_ADDR(NT_TIB, &pKPCR->NtTib);
	pKPCR->NtTib.ExceptionList = EXCEPTION_CHAIN_END;
	XboxTypes::KPRCB *pKPRCB = &pKPCR->PrcbData;
	InitializeListHead(&pKPRCB->DpcListHead);
	pKPRCB->DpcRoutineActive = 0;
	// TODO: fill in just enough data for the emulated code to work properly

	// Fill in GDT table data
	gdtTable[0].Set(0x00000000, 0x00000, 0x00, 0x0);
	gdtTable[1].Set(0x00000000, 0xffffb, 0x9b, 0xc);
	gdtTable[2].Set(0x00000000, 0xfffff, 0x93, 0xc);
	gdtTable[3].Set(tssAddr   , 0x00068, 0x89, 0x0);
	gdtTable[4].Set(kpcrAddr  , 0x00284, 0x93, 0xc);
	gdtTable[5].Set(tssDFAddr , 0x00068, 0x89, 0x0);
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

	return 0;
}

/*!
 * Load an XBE for execution
 */
int Xbox::LoadXbe(Xbe *xbe)
{
    // FIXME: Validate XBE virtual addresses!
    uint32_t base, offset, maxAddr;

    m_xbe = xbe;
    base = m_xbe->m_Header.dwBaseAddr;
    offset = 0;
	maxAddr = base;

    log_debug("XBE Base Address: 0x%08x\n", base);
    assert(base == 0x10000); // FIXME

    // Copy XBE Image Header to system memory
    log_debug("Loading XBE header @ %08x\n", base+offset);
    memcpy(&m_ram[base+offset], &m_xbe->m_Header, sizeof(m_xbe->m_Header));
    offset += sizeof(m_xbe->m_Header);
    memcpy(&m_ram[base+offset], m_xbe->m_HeaderEx, m_xbe->m_Header.dwSizeofHeaders);

    // Copy XBE Section Headers
    for(uint32 section = 0; section < m_xbe->m_Header.dwSections; section++) {
        offset = m_xbe->m_Header.dwSectionHeadersAddr + section * sizeof(m_xbe->m_SectionHeader[0]) - m_xbe->m_Header.dwBaseAddr;
        log_debug("Loading Section Header 0x%.04X @ %08x...", section, base+offset);
        memcpy(&m_ram[base+offset], &m_xbe->m_SectionHeader[section], sizeof(m_xbe->m_SectionHeader[0]));
		log_debug("OK\n");
    }

    // Copy XBE Section Data
    for(uint32_t section = 0; section < m_xbe->m_Header.dwSections; section++) {
        uint32_t RawSize = m_xbe->m_SectionHeader[section].dwSizeofRaw;
        offset = m_xbe->m_SectionHeader[section].dwVirtualAddr - m_xbe->m_Header.dwBaseAddr;
        log_debug("Loading Section 0x%.04X (%s) at %08x...", section, m_xbe->m_szSectionName[section], base+offset);
        if(RawSize > 0) {
            memcpy(&m_ram[base+offset], m_xbe->m_bzSection[section], RawSize);
			maxAddr = max(maxAddr, base + offset + RawSize);
		}
        log_debug("OK\n");

#if DUMP_SECTION_CONTENTS
        char s_name[512];
        snprintf(s_name, sizeof(s_name), "sec_%s.bin", m_xbe->m_szSectionName[section]);
        log_debug("Saving section data to %s...", s_name);
        FILE *f = fopen(s_name, "wb");
        fwrite(m_xbe->m_bzSection[section], RawSize, 1, f);
        fclose(f);
        log_debug("OK\n");
#endif
    }

    // Patch Kernel Imports
    uint32_t kernel_thunk_addr = UnscrambleAddress(m_xbe->m_Header.dwKernelImageThunkAddr, XOR_KT_DEBUG, XOR_KT_RETAIL);
    log_debug("Kernel Thunk Table Address: %08x\n", kernel_thunk_addr);
    uint32_t *imports = (uint32_t *)&m_ram[kernel_thunk_addr];
    for (int i = 0; imports[i] != 0; i++) {
        log_debug("  Patching Import %03d: %08x -> ", i, imports[i]);
        uint32_t import_num = imports[i] - 0x80000000;
        assert(import_num < 379);
        imports[i] = m_import_addrs[import_num];
        log_debug("%08x  %s\n", imports[i], kKernelFuncImportMap[import_num].name);
    }

    // Get unscrambled XBE entry point
    uint32_t entry_addr = UnscrambleAddress(m_xbe->m_Header.dwEntryAddr,
                                            XOR_EP_DEBUG, XOR_EP_RETAIL);

	// Reserve memory region for the entire XBE image
	// TODO: check Protect flags
	log_debug("Reserving memory range for XBE image @ 0x%08x - 0x%08x...", base, maxAddr);
	if (nullptr == m_pmemmgr->Reserve(base, maxAddr-base, PAGE_EXECUTE_READWRITE)) {
		log_debug("Could not reserve memory region!\n");
	}
	else {
		log_debug("OK\n");
	}

    // Create title main thread
	m_sched->ScheduleThread(CreateThread(entry_addr, XBOX_STACK_SIZE));

    return 0;
}

/*!
 * Unscramble an address using Debug and Retail keys
 * 
 * This method will XOR an address with the given debug and retail "keys". If
 * the XOR operation with the address and the retail key appears valid, it is
 * returned, otherwise the address XORed with the debug key is returned.
 */
uint32_t Xbox::UnscrambleAddress(uint32_t addr, uint32_t debug, uint32_t retail)
{
    uint32_t addr_out;

    addr_out = addr ^ retail;
    if (addr_out < XBOX_RAM_SIZE) {
        return addr_out;
    }

    return addr ^ debug;
}

/*!
 * Creates a new thread, allocating the stack in the main memory.
 */
Thread *Xbox::CreateThread(uint32_t entryAddress, uint32_t stackSize) {
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
	return new Thread(entryAddress, threadStack);
}

/*!
 * Handle VM exits caused by kernel entries
 * 
 * When the title attempts to call a Kernel function, it will trap here for
 * high-level emulation.
 */
int Xbox::HandleKernelEntry()
{
    uint32_t reg;
    uint32_t api_id;
    int result;

    // Subtract 1 from EIP because it will point to *next* instruction after the
    // `hlt` instruction that was executed
    m_cpu->RegRead(REG_EIP, &reg);
    reg -= 1;

    // Check bounds
    if (reg < m_kthunk_table_base || reg > m_kthunk_table_base + 378) return -1;

    // Identify function by the address which caused the exit
    api_id = reg - m_kthunk_table_base;
    log_debug("Kernel function %s (#%d) has been called...\n", kKernelFuncImportMap[api_id].name, api_id);

    // Call handler
    KernelFuncPtr handler;
    handler = kKernelFuncImportMap[api_id].handler;
    if (handler == NULL) {
        log_error("Call to undefined kernel function (import #%d)\n", api_id);
        assert(0);
    }
    result = (this->*handler)();
    if (result == KF_ERROR_NOT_IMPLEMENTED) {
        log_error("Kernel function has not yet been implemented! Cannot continue.\n");
        m_should_run = false;
    }
	else if (result == KF_WARN_FAKE_IMPL) {
		log_debug("Kernel function has a fake implementation.\n");
	}
	else if (result == KF_WARN_PARTIAL_IMPL) {
		log_debug("Kernel function is partially implemented.\n");
	}

    return 0;
}

/*!
 * Main loop which advances the system state
 * 
 * This is the main loop of the entire emulator.
 */
int Xbox::Run()
{
    Timer t;
    int result;
    struct CpuExitInfo *exit_info;

    m_should_run = true;

#if ENABLE_GDB_SERVER
    // Allow debugging before running so client can setup breakpoints, etc
    m_gdb->WaitForConnection();
    m_gdb->Debug(1);
#endif

    while (m_should_run) {
        SDL_PumpEvents();

        t.Start();
        m_video->FixmeLock();
        result = m_sched->Run();
        m_video->FixmeUnlock();
        t.Stop();
        log_debug("CPU Executed for %lld ms\n", t.GetMillisecondsElapsed());
		if (result == SCHEDULER_EXIT_NOTHREADS) {
			log_debug("Scheduler no longer has threads to execute\n");
			break;
		}
		else if (result != 0) {
            log_error("Error occured\n");
            break;
        }

        exit_info = m_cpu->GetExitInfo();
        if (exit_info->reason == EXIT_INTERRUPT) {
#if ENABLE_GDB_SERVER
            if (exit_info->intr_vector == 3 || exit_info->intr_vector == 1) {
                result = m_gdb->Debug(exit_info->intr_vector);
                if (result != 0) {
                    log_error("Debugger returned error %d\n", result);
                    break;
                }
#else
            if (0) {
#endif
            } else {
                log_error("Unhandled exception %d\n", exit_info->intr_vector);
                break;
            }
        } else {
            HandleKernelEntry(); // Did we stop to enter a Kernel function?
        }

		// Let the scheduler save the CPU context
		m_sched->SaveCPUContext();
		
		// t.Start();
        // m_video->Update();
        // t.Stop();
        // log_debug("Video update took %lld ms\n", t.GetMillisecondsElapsed());
    }

	if (LOG_LEVEL >= LOG_LEVEL_DEBUG) {
		log_debug("CPU registers at the end of execution:\n");
		m_cpu->PrintRegisters();
	}

    m_video->Cleanup();
#if ENABLE_GDB_SERVER
    m_gdb->Shutdown();
#endif

    return result;
}
