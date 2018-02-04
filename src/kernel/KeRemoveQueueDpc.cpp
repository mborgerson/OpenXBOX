#include "common.h"

/*
 * KeRemoveQueueDpc
 *
 * Import Number:      137
 * Calling Convention: stdcall
 * Parameter 0:        PRKDPC Dpc
 * Return Type:        BOOLEAN
 */
int Xbox::KeRemoveQueueDpc()
{
	K_ENTER_STDCALL();
	K_INIT_ARG(PRKDPC, Dpc);
	XboxTypes::BOOLEAN rval;

	rval = m_kernel->KeRemoveQueueDpc(Dpc);

	K_EXIT_WITH_VALUE(rval);
	return KF_OK;
}
