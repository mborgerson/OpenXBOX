#include "common.h"

/*
 * ObfDereferenceObject
 *
 * Import Number:      250
 * Calling Convention: fastcall
 * Parameter 0:        PVOID Object
 * Return Type:        VOID
 */
int Xbox::ObfDereferenceObject()
{
	K_ENTER_FASTCALL();
	K_INIT_ARG_PTR(VOID, Object);

	K_EXIT();
	return KF_ERROR_NOT_IMPLEMENTED;
}
