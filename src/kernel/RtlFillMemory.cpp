#include "common.h"

/*
 * RtlFillMemory
 *
 * Import Number:      284
 * Calling Convention: stdcall
 * Parameter 0:        PVOID Destination
 * Parameter 1:        ULONG Length
 * Parameter 2:        UCHAR Fill
 * Return Type:        VOID
 */
int Xbox::RtlFillMemory()
{
	K_ENTER_STDCALL();
	K_INIT_ARG_VAL(PVOID, Destination);
	K_INIT_ARG_VAL(ULONG, Length);
	K_INIT_ARG_VAL(UCHAR, Fill);

	m_kernel->RtlFillMemory(Destination, Length, Fill);
	
	K_EXIT();
	return KF_OK;
}
