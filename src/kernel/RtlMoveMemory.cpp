#include "common.h"

/*
 * RtlMoveMemory
 *
 * Import Number:      298
 * Calling Convention: stdcall
 * Parameter 0:        PVOID  Destination
 * Parameter 1:        PVOID  Source
 * Parameter 2:        ULONG  Length
 * Return Type:        VOID
 */
int Xbox::RtlMoveMemory()
{
	K_ENTER_STDCALL();
	K_INIT_ARG_VAL(PVOID, Destination);
	K_INIT_ARG_VAL(PVOID, Source);
	K_INIT_ARG_VAL(ULONG, Length);

	m_kernel->RtlMoveMemory(Destination, Source, Length);

	K_EXIT();
	return KF_OK;
}
