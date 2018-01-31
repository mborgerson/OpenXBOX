#include "common.h"

/*
 * KeBugCheckEx
 *
 * Import Number:      96
 * Calling Convention: stdcall
 * Parameter 0:        ULONG     BugCheckCode
 * Parameter 1:        ULONG_PTR BugCheckParameter1
 * Parameter 2:        ULONG_PTR BugCheckParameter2
 * Parameter 3:        ULONG_PTR BugCheckParameter3
 * Parameter 4:        ULONG_PTR BugCheckParameter4
 * Return Type:        VOID
 */
int Xbox::KeBugCheckEx()
{
	K_ENTER_STDCALL();
	K_INIT_ARG(ULONG,     BugCheckCode);
	K_INIT_ARG(ULONG_PTR, BugCheckParameter1);
	K_INIT_ARG(ULONG_PTR, BugCheckParameter2);
	K_INIT_ARG(ULONG_PTR, BugCheckParameter3);
	K_INIT_ARG(ULONG_PTR, BugCheckParameter4);

	m_kernel->KeBugCheckEx(BugCheckCode, BugCheckParameter1, BugCheckParameter2, BugCheckParameter3, BugCheckParameter4);

	m_should_run = false;

	K_EXIT();
	return KF_OK;
}
