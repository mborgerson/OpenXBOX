#include "common.h"

/*
 * PsQueryStatistics
 *
 * Import Number:      256
 * Calling Convention: stdcall
 * Parameter 0:        PPS_STATISTICS ProcessStatistics
 * Return Type:        NTSTATUS
 */
int Xbox::PsQueryStatistics()
{
	K_ENTER_STDCALL();
	K_INIT_ARG_PTR(PS_STATISTICS, ProcessStatistics);
	XboxTypes::NTSTATUS rval;

	K_EXIT_WITH_VALUE(rval);
	return KF_ERROR_NOT_IMPLEMENTED;
}
