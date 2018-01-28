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
	K_INIT_ARG_RPT(KDPC, Dpc);
	K_INIT_ARG_PTR(VOID, SystemArgument1);
	K_INIT_ARG_PTR(VOID, SystemArgument2);
	XboxTypes::BOOLEAN rval;

	K_EXIT_WITH_VALUE(rval);
	return KF_ERROR_NOT_IMPLEMENTED;
}
