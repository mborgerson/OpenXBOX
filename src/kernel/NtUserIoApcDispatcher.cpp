#include "common.h"

/*
 * NtUserIoApcDispatcher
 *
 * Import Number:      232
 * Calling Convention: stdcall
 * Parameter 0:        PVOID            ApcContext
 * Parameter 1:        PIO_STATUS_BLOCK IoStatusBlock
 * Parameter 2:        ULONG            Reserved
 * Return Type:        VOID
 */
int Xbox::NtUserIoApcDispatcher()
{
	K_ENTER_STDCALL();
	K_INIT_ARG_PTR(VOID,            ApcContext);
	K_INIT_ARG_PTR(IO_STATUS_BLOCK, IoStatusBlock);
	K_INIT_ARG_VAL(ULONG,           Reserved);

	K_EXIT();
	return KF_ERROR_NOT_IMPLEMENTED;
}
