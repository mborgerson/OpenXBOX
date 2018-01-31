#include "common.h"

/*
 * HalReturnToFirmware
 *
 * Import Number:      49
 * Calling Convention: stdcall
 * Parameter 0:        FIRMWARE_REENTRY Routine
 * Return Type:        VOID
 */
int Xbox::HalReturnToFirmware()
{
	K_ENTER_STDCALL();
	K_INIT_ARG_VAL(FIRMWARE_REENTRY, Routine);

	m_kernel->HalReturnToFirmware(Routine);

	// FIXME: should actually reboot or halt the system according to the chosen routine
	m_should_run = false;

	K_EXIT();
	return KF_WARN_PARTIAL_IMPL;
}
