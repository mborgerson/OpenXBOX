#include "common.h"

/*
 * ObfReferenceObject
 *
 * Import Number:      251
 * Calling Convention: fastcall
 * Parameter 0:        PVOID Object
 * Return Type:        VOID
 */
int Xbox::ObfReferenceObject()
{
	K_ENTER_FASTCALL();
	K_INIT_ARG_PTR(VOID, Object);

	K_EXIT();
	return KF_ERROR_NOT_IMPLEMENTED;
}
