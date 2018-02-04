#include "common.h"

/*
 * KeInsertQueueDpc
 *
 * Import Number:      119
 * Calling Convention: stdcall
 * Parameter 0:        PRKDPC Dpc
 * Parameter 1:        PVOID  SystemArgument1
 * Parameter 2:        PVOID  SystemArgument2
 * Return Type:        BOOLEAN
 */
int Xbox::KeInsertQueueDpc()
{
	K_ENTER_STDCALL();
	K_INIT_ARG(PRKDPC, Dpc);
	K_INIT_ARG(PVOID,  SystemArgument1);
	K_INIT_ARG(PVOID,  SystemArgument2);
	XboxTypes::BOOLEAN rval;

	rval = m_kernel->KeInsertQueueDpc(Dpc, SystemArgument1, SystemArgument2);

	K_EXIT_WITH_VALUE(rval);
	return KF_WARN_PARTIAL_IMPL;
}
