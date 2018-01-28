#include "common.h"

/*
 * KeInitializeMutant
 *
 * Import Number:      110
 * Calling Convention: stdcall
 * Parameter 0:        PRKMUTANT Mutant
 * Parameter 1:        BOOLEAN   InitialOwner
 * Return Type:        VOID
 */
int Xbox::KeInitializeMutant()
{
	K_ENTER_STDCALL();
	K_INIT_ARG_RPT(KMUTANT, Mutant);
	K_INIT_ARG_VAL(BOOLEAN, InitialOwner);

	// FIXME: let the object manager initialize this


	K_EXIT();
	return KF_ERROR_NOT_IMPLEMENTED;
}
