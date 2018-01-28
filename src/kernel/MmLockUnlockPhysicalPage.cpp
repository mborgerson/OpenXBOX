#include "common.h"

/*
 * MmLockUnlockPhysicalPage
 *
 * Import Number:      176
 * Calling Convention: stdcall
 * Parameter 0:        ULONG_PTR PhysicalAddress
 * Parameter 1:        BOOLEAN   UnlockPage
 * Return Type:        VOID
 */
int Xbox::MmLockUnlockPhysicalPage()
{
	K_ENTER_STDCALL();
	K_INIT_ARG_VAL(ULONG_PTR, PhysicalAddress);
	K_INIT_ARG_VAL(BOOLEAN,   UnlockPage);

	K_EXIT();
	return KF_ERROR_NOT_IMPLEMENTED;
}
