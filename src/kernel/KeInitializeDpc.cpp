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
	K_INIT_ARG(XboxTypes::PKDPC,              Dpc);
	K_INIT_ARG(XboxTypes::PKDEFERRED_ROUTINE, DeferredRoutine);
	K_INIT_ARG(XboxTypes::PVOID,              DeferredContext);

	XboxTypes::KDPC *dpc = _ADDR_TO_PTR(KDPC, Dpc);
	dpc->Type = XboxTypes::DpcObject;
	dpc->DeferredRoutine = DeferredRoutine;
	dpc->DeferredContext = DeferredContext;
	dpc->Inserted = FALSE;

	K_EXIT();
	return 0;
}
