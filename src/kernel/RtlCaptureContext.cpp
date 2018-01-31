#include "common.h"

/*
 * RtlCaptureContext
 *
 * Import Number:      265
 * Calling Convention: stdcall
 * Parameter 0:        PCONTEXT ContextRecord
 * Return Type:        VOID
 */
int Xbox::RtlCaptureContext()
{
	K_ENTER_STDCALL();
	K_INIT_ARG(PCONTEXT, ContextRecord);

	K_EXIT();
	return KF_ERROR_NOT_IMPLEMENTED;
}
