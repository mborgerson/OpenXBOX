#include "common.h"

/*
 * ExQueryNonVolatileSetting
 *
 * Import Number:      24
 * Calling Convention: stdcall
 * Parameter 0:        ULONG  ValueIndex
 * Parameter 1:        PULONG Type
 * Parameter 2:        PVOID  Value
 * Parameter 3:        ULONG  ValueLength
 * Parameter 4:        PULONG ResultLength
 * Return Type:        NTSTATUS
 */
int Xbox::ExQueryNonVolatileSetting()
{
	K_ENTER_STDCALL();
	K_INIT_ARG_VAL(ULONG, ValueIndex);
	K_INIT_ARG_PTR(ULONG, Type);
	K_INIT_ARG_PTR(VOID,  Value);
	K_INIT_ARG_VAL(ULONG, ValueLength);
	K_INIT_ARG_PTR(ULONG, ResultLength);
	XboxTypes::NTSTATUS rval;

	rval = 0;
	K_EXIT_WITH_VALUE(rval);
	return KF_WARN_FAKE_IMPL;
}
