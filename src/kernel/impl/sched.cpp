#include "kernel/impl/sched.h"
#include "log.h"

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

void Scheduler::SetKPCR(XboxTypes::KPCR *kpcr) {
	m_kpcr = kpcr;
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

		// Update KPCR info
		m_kpcr->PrcbData.CurrentThread = m_currentThread->m_pkthread;
		m_kpcr->NtTib.StackBase = m_currentThread->m_kthread->StackBase;
		m_kpcr->NtTib.StackLimit = m_currentThread->m_kthread->StackLimit;
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
