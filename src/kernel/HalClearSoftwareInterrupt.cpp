#include "common.h"

/*
 * HalClearSoftwareInterrupt
 *
 * Import Number:      38
 * Calling Convention: fastcall
 * Parameter 0:        KIRQL RequestIrql
 * Return Type:        VOID
 */
int Xbox::HalClearSoftwareInterrupt()
{
	K_ENTER_FASTCALL();
	K_INIT_ARG_VAL(KIRQL, RequestIrql);

	K_EXIT();
	return KF_ERROR_NOT_IMPLEMENTED;
}
