#include "sched.h"
#include "log.h"

static uint32_t g_threadId = 0;

/*!
 * Constructor
 */
Thread::Thread(uint32_t entry, ContiguousMemoryBlock *stack)
	: m_entry(entry)
	, m_stack(stack)
	, m_id(++g_threadId)
{
    m_context.m_regs[REG_EIP] = m_entry;
	m_context.m_regs[REG_ESP] = stack->BaseAddress() + stack->Size();
	m_context.m_regs[REG_EBP] = stack->BaseAddress();
	
	// TODO: these should be set up by the kernel
	m_context.m_regs[REG_CS] = 0x08;
	m_context.m_regs[REG_SS] = 0x10;
	m_context.m_regs[REG_DS] = 0x10;
	m_context.m_regs[REG_ES] = 0x10;
	m_context.m_regs[REG_FS] = 0x20;
	m_context.m_regs[REG_GS] = 0x00;
}

/*!
 * Destructor
 */
Thread::~Thread()
{
	m_stack->Free();
	delete m_stack;
}

/*!
 * Constructor
 */
Scheduler::Scheduler(Cpu *cpu)
	: m_cpu(cpu)
	, m_nextThreadIndex(0)
	, m_currentThread(nullptr)
{
}

/*!
 * Destructor
 */
Scheduler::~Scheduler()
{
}

/*!
 * Schedule a thread for execution
 */
int Scheduler::ScheduleThread(Thread *thread)
{
	// If this is the first thread, restore its context to initialize the CPU
	// so that the debugger can pick up the initial state
	if (m_threads.empty()) {
		m_cpu->RestoreContext(&thread->m_context);
	}

    m_threads.push_back(thread);

    return 0;
}

/*!
 * Choose a thread to execute
 */
bool Scheduler::ChooseNextThread() {
	if (m_threads.empty()) {
		return false;
	}

	Thread *prevThread = m_currentThread;

	// TODO: implement proper thread scheduling
	// For now, threads will be scheduled with a simple round-robin method
	m_nextThreadIndex = (m_nextThreadIndex + 1) % m_threads.size();
	m_currentThread = m_threads[m_nextThreadIndex];

	// Restore CPU context from the thread if switched
	if (m_currentThread != prevThread) {
		// TODO: implement proper context switching
		m_cpu->RestoreContext(&m_currentThread->m_context);

		// TODO: tell the thread to update KPCR info
	}

	return true;
}

/*!
  * Save the CPU context on the current thread
  */
void Scheduler::SaveCPUContext() {
	if (nullptr != m_currentThread) {
		m_cpu->SaveContext(&m_currentThread->m_context);
	}
}

/*!
 * Begin CPU execution
 */
int Scheduler::Run()
{
    uint32_t reg;
    int result;

	// Choose next thread to execute
	if (!ChooseNextThread()) {
		log_debug("No more threads to execute. Halting.\n");
		return SCHEDULER_EXIT_EXPIRE;
	}
	log_debug("Executing thread #%d\n", m_currentThread->m_id);

    // Get current instruction pointer
    m_cpu->RegRead(REG_EIP, &reg);
    log_debug("CPU starting at %x\n", reg);

    // Allow the CPU to execute for a fixed amount of time (or shorter if the
    // guest code causes a VM exit)
    result = m_cpu->Run(100*1000); // 100ms quantum
    if (result) {
        return SCHEDULER_EXIT_ERROR;
    }

	// Read the exit instruction pointer
    m_cpu->RegRead(REG_EIP, &reg);
    log_debug("CPU stopped at %x (reason %d)\n", reg, result);

	// TODO: run all pending DPCs

	// Handle thread exit
	// TODO: find a better way to detect this
	if (reg == 0) {
		log_debug("Thread #%d exited.\n", m_currentThread->m_id);
		m_threads.erase(std::find(m_threads.begin(), m_threads.end(), m_currentThread));  // FIXME: vectors are not adequate for random removal
		delete m_currentThread;
		m_currentThread = nullptr;
	}

    // Debugging
    m_cpu->PrintRegisters();

    return 0;
}
