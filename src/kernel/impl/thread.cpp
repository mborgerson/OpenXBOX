#include "kernel/impl/thread.h"
#include "log.h"

// FIXME: global variable
static uint32_t g_threadId = 0;

/*!
 * Constructor
 */
Thread::Thread(uint32_t entry, PhysicalMemoryBlock *stack, XboxTypes::PKTHREAD pkthread, XboxTypes::KTHREAD *kthread)
	: m_stack(stack)
	, m_id(++g_threadId)
	, m_pkthread(pkthread)
	, m_kthread(kthread)
{
	g_cond_init(&m_suspensionSync);
	g_mutex_init(&m_suspensionMutex);

    m_context.m_regs[REG_EIP] = entry;
	m_context.m_regs[REG_ESP] = stack->BaseAddress() + stack->Size() - 4; // 4 bytes reserved for the special thread exit return address
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
	g_cond_clear(&m_suspensionSync);
	g_mutex_clear(&m_suspensionMutex);

	delete m_stack;
}



bool AlwaysTrueTSCondition::IsMet() {
	return true;
}
