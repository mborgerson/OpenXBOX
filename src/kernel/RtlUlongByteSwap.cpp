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
	K_INIT_ARG_VAL(ULONG, Source);
	XboxTypes::ULONG rval;

	rval = (Source >> 24) | ((Source & 0xFF0000) >> 8) | ((Source & 0xFF00) << 8) | ((Source & 0xFF) << 24);

	K_EXIT_WITH_VALUE(rval);
	return KF_OK;
}
