#include "common.h"

/*
 * ObMakeTemporaryObject
 *
 * Import Number:      242
 * Calling Convention: stdcall
 * Parameter 0:        PVOID Object
 * Return Type:        VOID
 */
int Xbox::ObMakeTemporaryObject()
{
	K_ENTER_STDCALL();
	K_INIT_ARG_PTR(VOID, Object);

	K_EXIT();
	return KF_ERROR_NOT_IMPLEMENTED;
}
