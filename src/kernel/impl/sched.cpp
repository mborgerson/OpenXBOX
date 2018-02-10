#include <vector>
#include <iterator>
#include <algorithm>
#include "kernel/impl/sched.h"
#include "log.h"

Scheduler::Scheduler(Cpu *cpu, GThreadFunc emuThreadFunc, gpointer emuThreadData)
	: m_cpu(cpu)
	, m_nextThreadIndex(0)
	, m_currentThread(nullptr)
	, m_emuThreadFunc(emuThreadFunc)
	, m_emuThreadData(emuThreadData)
{
}

Scheduler::~Scheduler()
{
	// Free all threads
	std::for_each(m_suspendedThreads.begin(), m_suspendedThreads.end(), [](Thread *t) { delete t; });
	std::for_each(m_activeThreads.begin(), m_activeThreads.end(), [](Thread *t) { delete t; });
}

// Helper function to remove an item from a vector
// FIXME: vectors are not adequate for random removal
template <typename T>
void vec_erase(std::vector<T> &vec, T item) {
	vec.erase(std::find(vec.begin(), vec.end(), item));
}

int Scheduler::Run()
{
	uint32_t reg;
	int result;

	// Choose next thread to execute
	if (!ChooseNextThread()) {
		// FIXME: should do the "idle thread" work
		log_debug("No more threads to execute. Halting.\n");
		return SCHEDULER_EXIT_EXPIRE;
	}
	log_debug("Executing thread #%d\n", m_currentThread->m_id);

	// Get current instruction pointer
	m_cpu->RegRead(REG_EIP, &reg);
	log_debug("CPU starting at %x\n", reg);

	// Allow the CPU to execute for a fixed amount of time (or shorter if the
	// guest code causes a VM exit)
	result = m_cpu->Run(100 * 1000); // 100ms quantum
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
		vec_erase(m_activeThreads, m_currentThread);
		delete m_currentThread;
		m_currentThread = nullptr;
	}

	// Debugging
	m_cpu->PrintRegisters();

	return 0;
}

void Scheduler::SetKPCR(XboxTypes::KPCR *kpcr) {
	m_kpcr = kpcr;
}

void Scheduler::SaveCPUContext() {
	if (nullptr != m_currentThread) {
		m_cpu->SaveContext(&m_currentThread->m_context);
	}
}

int Scheduler::ScheduleThread(Thread *thread)
{
	// If this is the first thread, restore its context to initialize the CPU
	// so that the debugger can pick up the initial state
	if (m_activeThreads.empty()) {
		m_cpu->RestoreContext(&thread->m_context);
	}

    m_activeThreads.push_back(thread);

    return 0;
}

void Scheduler::SuspendThread(ThreadSuspensionCondition *condition) {
	// Save the CPU context of the current guest thread
	SaveCPUContext();

	// Mark the thread as suspended and set the suspension condition
	m_currentThread->m_suspensionCondition = condition;
	m_suspendedThreads.push_back(m_currentThread);
	vec_erase(m_activeThreads, m_currentThread);

	// Start a new host thread to continue execution of the emulator
	g_thread_new("Emulation thread", m_emuThreadFunc, m_emuThreadData);

	// Wait for the condition to be signaled
	GCond *cond = &m_currentThread->m_suspensionSync;
	GMutex *mutex = &m_currentThread->m_suspensionMutex;
	g_mutex_lock(mutex); 
	g_cond_wait(cond, mutex);
	g_mutex_unlock(mutex);
}


bool Scheduler::ChooseNextThread() {
	if (m_activeThreads.empty()) {
		return false;
	}

	Thread *prevThread = m_currentThread;

	// TODO: implement proper thread scheduling
	// This should actually use the fields in KPCR/KTHREAD to determine what
	// thread to run next, as they are updated by various kernel functions.

	// For now, threads will be scheduled with a simple round-robin method
	m_nextThreadIndex = (m_nextThreadIndex + 1) % m_activeThreads.size();
	m_currentThread = m_activeThreads[m_nextThreadIndex];

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
