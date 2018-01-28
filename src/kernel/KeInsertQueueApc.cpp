#include "common.h"

/*
 * KeInsertQueueApc
 *
 * Import Number:      118
 * Calling Convention: stdcall
 * Parameter 0:        PRKAPC    Apc
 * Parameter 1:        PVOID     SystemArgument1
 * Parameter 2:        PVOID     SystemArgument2
 * Parameter 3:        KPRIORITY Increment
 * Return Type:        BOOLEAN
 */
int Xbox::KeInsertQueueApc()
{
	K_ENTER_STDCALL();
	K_INIT_ARG_RPT(KAPC,      Apc);
	K_INIT_ARG_PTR(VOID,      SystemArgument1);
	K_INIT_ARG_PTR(VOID,      SystemArgument2);
	K_INIT_ARG_VAL(KPRIORITY, Increment);
	XboxTypes::BOOLEAN rval;

	K_EXIT_WITH_VALUE(rval);
	return KF_ERROR_NOT_IMPLEMENTED;
}
