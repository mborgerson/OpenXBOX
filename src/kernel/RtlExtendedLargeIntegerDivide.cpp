#include "common.h"

/*
 * RtlExtendedLargeIntegerDivide
 *
 * Import Number:      282
 * Calling Convention: stdcall
 * Parameter 0:        LARGE_INTEGER Dividend
 * Parameter 1:        ULONG         Divisor
 * Parameter 2:        PULONG        Remainder
 * Return Type:        LARGE_INTEGER
 */
int Xbox::RtlExtendedLargeIntegerDivide()
{
	K_ENTER_STDCALL();
	K_INIT_ARG(LARGE_INTEGER, Dividend);
	K_INIT_ARG(ULONG,         Divisor);
	K_INIT_ARG(PULONG,        Remainder);
	XboxTypes::LARGE_INTEGER rval;

	rval = m_kernel->RtlExtendedLargeIntegerDivide(Dividend, Divisor);

	K_EXIT_WITH_VALUE(rval);
	return KF_OK;
}
