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
	K_INIT_ARG_PTR(KDPC,               Dpc);
	K_INIT_ARG_VAL(PKDEFERRED_ROUTINE, DeferredRoutine);
	K_INIT_ARG_PTR(VOID,               DeferredContext);

	// FIXME: let the object manager initialize this
	pDpc->Type = XboxTypes::DpcObject;
	pDpc->DeferredRoutine = DeferredRoutine;
	pDpc->DeferredContext = DeferredContext;
	pDpc->Inserted = FALSE;

	K_EXIT();
	return KF_OK;
}
