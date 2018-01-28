#include "common.h"

/*
 * HalEnableSecureTrayEject
 *
 * Import Number:      365
 * Calling Convention: stdcall
 * Return Type:        VOID
 */
int Xbox::HalEnableSecureTrayEject()
{
	K_ENTER_STDCALL();

	K_EXIT();
	return KF_ERROR_NOT_IMPLEMENTED;
}
