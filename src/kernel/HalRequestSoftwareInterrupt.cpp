#include "common.h"

/*
 * HalRequestSoftwareInterrupt
 *
 * Import Number:      48
 * Calling Convention: fastcall
 * Parameter 0:        KIRQL RequestIrql
 * Return Type:        VOID
 */
int Xbox::HalRequestSoftwareInterrupt()
{
	K_ENTER_FASTCALL();
	K_INIT_ARG(KIRQL, RequestIrql);

	K_EXIT();
	return KF_ERROR_NOT_IMPLEMENTED;
}
