#include "common.h"

/*
 * KeInitializeDpc
 *
 * Import Number:      107
 * Calling Convention: stdcall
 * Parameter 0:        PKDPC              Dpc
 * Parameter 1:        PKDEFERRED_ROUTINE DeferredRoutine
 * Parameter 2:        PVOID              DeferredContext
 * Return Type:        VOID
 */
int Xbox::KeInitializeDpc()
{
	K_ENTER_STDCALL();
	K_INIT_ARG(PKDPC,              Dpc);
	K_INIT_ARG(PKDEFERRED_ROUTINE, DeferredRoutine);
	K_INIT_ARG(PVOID,              DeferredContext);

	m_kernel->KeInitializeDpc(Dpc, DeferredRoutine, DeferredContext);

	K_EXIT();
	return KF_OK;
}
