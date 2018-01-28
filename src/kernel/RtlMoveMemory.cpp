#include "common.h"

/*
 * RtlMoveMemory
 *
 * Import Number:      298
 * Calling Convention: stdcall
 * Parameter 0:        PVOID  Destination
 * Parameter 1:        PPVOID Source
 * Parameter 2:        ULONG  Length
 * Return Type:        VOID
 */
int Xbox::RtlMoveMemory()
{
	K_ENTER_STDCALL();
	K_INIT_ARG_PTR(VOID,  Destination);
	K_INIT_ARG_PTR(PVOID, Source);
	K_INIT_ARG_VAL(ULONG, Length);

	memmove(pDestination, pSource, Length);

	K_EXIT();
	return KF_OK;
}
