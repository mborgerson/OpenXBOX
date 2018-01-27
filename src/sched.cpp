#include "sched.h"
#include "log.h"

/*!
 * Constructor
 */
Thread::Thread(uint32_t entry, ContiguousMemoryBlock *stack)
: m_entry(entry), m_stack(stack)
{
    m_context.m_regs[REG_EIP] = m_entry;
    m_context.m_regs[REG_ESP] = stack->BaseAddress() + stack->Size();
}

/*!
 * Destructor
 */
Thread::~Thread()
{
	m_stack->Free();
}

/*!
 * Constructor
 */
Scheduler::Scheduler(Cpu *cpu)
	: m_cpu(cpu)
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
    m_threads.push_back(thread);

    // FIXME: Move this out of here after we support concurrency
    m_cpu->RegWrite(REG_ESP, thread->m_stack->BaseAddress() + thread->m_stack->Size());
    m_cpu->RegWrite(REG_EIP, thread->m_entry);

    return 0;
}

/*!
 * Begin CPU execution
 */
int Scheduler::Run()
{
    uint32_t reg;
    int result;

	// TODO: implement proper thread scheduling, context switching, etc.
	// TODO: run all pending DPCs while switching threads
	// TODO: keep an XboxTypes::KTHREAD around
	// TODO: update KPCR info with the KTHREAD

    // Get current instruction pointer
    m_cpu->RegRead(REG_EIP, &reg);
    log_debug("CPU starting at %x\n", reg);

    // Allow the CPU to execute for a fixed amount of time (or shorter if the
    // guest code causes a VM exit)
    result = m_cpu->Run(100*1000); // 100ms quantum
    if (result) {
        return SCHEDULER_EXIT_ERROR;
    }

	// TODO: When the thread exits:
	//m_threads.erase(std::find(m_threads.begin(), m_threads.end(), thread));  // FIXME: vectors are not adequate for random removal
	//delete thread;

    // Read the exit instruction pointer
    m_cpu->RegRead(REG_EIP, &reg);
    log_debug("CPU stopped at %x (reason %d)\n", reg, result);

    // Debugging
    m_cpu->PrintRegisters();

    return 0;
}
