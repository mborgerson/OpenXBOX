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

	// Handle thread exit, identified by a special return address at the bottom
	// of the stack
	if (reg == THREAD_EXIT_RETURN_ADDRESS) {
		log_debug("Thread #%d exited.\n", m_currentThread->m_id);
		vec_erase(m_activeThreads, m_currentThread);
		m_threadsByPointer.erase(m_currentThread->m_pkthread);
		delete m_currentThread;
		m_currentThread = nullptr;
		return SCHEDULER_EXIT_THREAD_EXITED;
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
	m_threadsByPointer[thread->m_pkthread] = thread;

    return 0;
}

void Scheduler::SuspendCurrentThread(ThreadSuspensionCondition *condition) {
	SuspendThread(m_currentThread, condition);
}

void Scheduler::SuspendThread(Thread *thread, ThreadSuspensionCondition *condition) {
	// Save the CPU context of the current guest thread
	SaveCPUContext();

	// Mark the thread as suspended and set the condition
	thread->m_suspensionCondition = condition;
	m_suspendedThreads.push_back(thread);
	vec_erase(m_activeThreads, thread);

	// Suspend the host thread if the guest thread to be suspended happens to
	// be the one we're currently executing
	if (thread == m_currentThread) {
		// Start a new host thread to continue execution of the emulator
		g_thread_new("Emulation thread", m_emuThreadFunc, m_emuThreadData);

		// Wait for the condition to be signaled
		GCond *cond = &thread->m_suspensionSync;
		GMutex *mutex = &thread->m_suspensionMutex;
		g_mutex_lock(mutex);
		thread->m_suspensionSynced = true;
		g_cond_wait(cond, mutex);
		thread->m_suspensionSynced = false;
		g_mutex_unlock(mutex);
	}
}

void Scheduler::ResumeThread(XboxTypes::PKTHREAD kthread) {
	// Find thread
	if (m_threadsByPointer.count(kthread)) {
		Thread *thread = m_threadsByPointer[kthread];
		
		if (thread == m_currentThread) {
			// TODO: this shouldn't happen as the code that invokes runs on a
			// supposedly ready thread
			log_warning("Sanity check failed, current thread is being resumed");
		}
		else {
			// Make it the current thread
			m_currentThread = thread;

			// Restore the CPU context
			m_cpu->RestoreContext(&thread->m_context);

			// Mark the thread as active
			m_activeThreads.push_back(thread);
			vec_erase(m_suspendedThreads, thread);

			// Clear suspension condition
			delete thread->m_suspensionCondition;
			thread->m_suspensionCondition = nullptr;
		}
	}
	else {
		log_warning("Thread 0x%x is not mapped", kthread);
	}
}

bool Scheduler::CheckSuspendedThreads() {
	for (auto it = m_suspendedThreads.begin(); it != m_suspendedThreads.end(); it++) {
		Thread *thread = *it;
		
		// If the thread's condition is met
		if (thread->m_suspensionCondition->IsMet()) {
			// Make it the current thread
			m_currentThread = thread;

			// Restore the CPU context
			m_cpu->RestoreContext(&thread->m_context);
			
			// Mark the thread as active
			m_activeThreads.push_back(thread);
			m_suspendedThreads.erase(it);
			
			// Clear suspension condition
			delete thread->m_suspensionCondition;
			thread->m_suspensionCondition = nullptr;

			// If there is a synchronization object, signal it to wake up the original host thread,
			// and terminate execution of the current thread
			if (thread->m_suspensionSynced) {
				g_mutex_lock(&thread->m_suspensionMutex);
				g_cond_signal(&thread->m_suspensionSync);
				g_mutex_unlock(&thread->m_suspensionMutex);
				return true;
			}
		}
	}
	return false;
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
