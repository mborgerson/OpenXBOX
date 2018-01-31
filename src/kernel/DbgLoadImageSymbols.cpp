#include "common.h"

/*
 * DbgLoadImageSymbols
 *
 * Import Number:      7
 * Calling Convention: stdcall
 * Parameter 0:        PSTRING   FileName
 * Parameter 1:        PVOID     ImageBase
 * Parameter 2:        ULONG_PTR ProcessId
 * Return Type:        VOID
 */
int Xbox::DbgLoadImageSymbols()
{
	K_ENTER_STDCALL();
	K_INIT_ARG(PSTRING,   FileName);
	K_INIT_ARG(PVOID,     ImageBase);
	K_INIT_ARG(ULONG_PTR, ProcessId);

	K_EXIT();
	return KF_ERROR_NOT_IMPLEMENTED;
}
