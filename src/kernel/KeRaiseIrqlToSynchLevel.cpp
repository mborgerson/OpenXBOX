#include "common.h"

/*
 * KeRaiseIrqlToSynchLevel
 *
 * Import Number:      130
 * Calling Convention: stdcall
 * Return Type:        KIRQL
 */
int Xbox::KeRaiseIrqlToSynchLevel()
{
	K_ENTER_STDCALL();
	XboxTypes::KIRQL rval;

	rval = m_kernel->KeRaiseIrqlToSynchLevel();

	K_EXIT_WITH_VALUE(rval);
	return KF_OK;
}
