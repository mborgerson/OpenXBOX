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

	switch (Routine) {
	case XboxTypes::HalHaltRoutine:
		log_info("HalReturnToFirmware: Halt system\n");
		break;
	case XboxTypes::HalRebootRoutine:
		log_info("HalReturnToFirmware: Reboot\n");
		break;
	case XboxTypes::HalQuickRebootRoutine:
		log_info("HalReturnToFirmware: Quick reboot\n");
		break;
	case XboxTypes::HalKdRebootRoutine:
		log_info("HalReturnToFirmware: Reboot to kernel debugger\n");
		break;
	case XboxTypes::HalFatalErrorRebootRoutine:
		log_info("HalReturnToFirmware: Reboot due to fatal error\n");
		break;
	default:
		log_info("HalReturnToFirmware: Unknown routine: %d\n", Routine);
		break;
	}

	// FIXME: should actually reboot or halt the system according to the chosen routine
	m_should_run = false;

	K_EXIT();
	return KF_WARN_PARTIAL_IMPL;
}
