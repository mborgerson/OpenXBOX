#include "common.h"

/*
 * RtlUlongByteSwap
 *
 * Import Number:      307
 * Calling Convention: fastcall
 * Parameter 0:        ULONG Source
 * Return Type:        ULONG
 */
int Xbox::RtlUlongByteSwap()
{
	K_ENTER_FASTCALL();
	K_INIT_ARG(ULONG, Source);
	XboxTypes::ULONG rval;

	rval = m_kernel->RtlUlongByteSwap(Source);

	K_EXIT_WITH_VALUE(rval);
	return KF_OK;
}
