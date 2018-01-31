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
	K_INIT_ARG(ULONG,  ValueIndex);
	K_INIT_ARG(PULONG, Type);
	K_INIT_ARG(PVOID,  Value);
	K_INIT_ARG(ULONG,  ValueLength);
	K_INIT_ARG(PULONG, ResultLength);
	XboxTypes::NTSTATUS rval;

	rval = m_kernel->ExQueryNonVolatileSetting(ValueIndex, Type, Value, ValueLength, ResultLength);

	K_EXIT_WITH_VALUE(rval);
	return KF_WARN_FAKE_IMPL;
}
