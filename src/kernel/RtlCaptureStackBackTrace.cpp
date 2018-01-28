#include "common.h"

/*
 * RtlCaptureStackBackTrace
 *
 * Import Number:      266
 * Calling Convention: stdcall
 * Parameter 0:        ULONG  FramesToSkip
 * Parameter 1:        ULONG  FramesToCapture
 * Parameter 2:        PPVOID BackTrace
 * Parameter 3:        PULONG BackTraceHash
 * Return Type:        USHORT
 */
int Xbox::RtlCaptureStackBackTrace()
{
	K_ENTER_STDCALL();
	K_INIT_ARG_VAL(ULONG, FramesToSkip);
	K_INIT_ARG_VAL(ULONG, FramesToCapture);
	K_INIT_ARG_PTR(PVOID, BackTrace);
	K_INIT_ARG_PTR(ULONG, BackTraceHash);
	XboxTypes::USHORT rval;

	K_EXIT_WITH_VALUE(rval);
	return KF_ERROR_NOT_IMPLEMENTED;
}
