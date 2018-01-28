#include "common.h"

/*
 * RtlTimeFieldsToTime
 *
 * Import Number:      304
 * Calling Convention: stdcall
 * Parameter 0:        PTIME_FIELDS   TimeFields
 * Parameter 1:        PLARGE_INTEGER Time
 * Return Type:        BOOLEAN
 */
int Xbox::RtlTimeFieldsToTime()
{
	K_ENTER_STDCALL();
	K_INIT_ARG_PTR(TIME_FIELDS,   TimeFields);
	K_INIT_ARG_PTR(LARGE_INTEGER, Time);
	XboxTypes::BOOLEAN rval;

	K_EXIT_WITH_VALUE(rval);
	return KF_ERROR_NOT_IMPLEMENTED;
}
