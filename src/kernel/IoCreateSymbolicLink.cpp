#include "common.h"

/*
 * IoCreateSymbolicLink
 *
 * Import Number:      67
 * Calling Convention: stdcall
 * Parameter 0:        POBJECT_STRING SymbolicLinkName
 * Parameter 1:        POBJECT_STRING DeviceName
 * Return Type:        NTSTATUS
 */
int Xbox::IoCreateSymbolicLink()
{
	K_ENTER_STDCALL();
	K_INIT_ARG(POBJECT_STRING, SymbolicLinkName);
	K_INIT_ARG(POBJECT_STRING, DeviceName);
	XboxTypes::NTSTATUS rval;

	rval = m_kernel->IoCreateSymbolicLink(SymbolicLinkName, DeviceName);

	K_EXIT_WITH_VALUE(rval);
	return KF_WARN_FAKE_IMPL;
}
