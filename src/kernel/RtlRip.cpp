#include "common.h"

/*
 * RtlRip
 *
 * Import Number:      352
 * Calling Convention: stdcall
 * Parameter 0:        PVOID ApiName
 * Parameter 1:        PVOID Expression
 * Parameter 2:        PVOID Message
 * Return Type:        VOID
 */
int Xbox::RtlRip()
{
	K_ENTER_STDCALL();
	K_INIT_ARG(PVOID, ApiName);
	K_INIT_ARG(PVOID, Expression);
	K_INIT_ARG(PVOID, Message);

	m_kernel->RtlRip(ApiName, Expression, Message);

	K_EXIT();
	return KF_OK;
}
