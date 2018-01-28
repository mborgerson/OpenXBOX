#include "common.h"

/*
 * HalRegisterShutdownNotification
 *
 * Import Number:      47
 * Calling Convention: stdcall
 * Parameter 0:        PHAL_SHUTDOWN_REGISTRATION ShutdownRegistration
 * Parameter 1:        BOOLEAN                    Register
 * Return Type:        VOID
 */
int Xbox::HalRegisterShutdownNotification()
{
	K_ENTER_STDCALL();
	K_INIT_ARG_PTR(HAL_SHUTDOWN_REGISTRATION, ShutdownRegistration);
	K_INIT_ARG_VAL(BOOLEAN,                   Register);

	K_EXIT();
	return KF_ERROR_NOT_IMPLEMENTED;
}
