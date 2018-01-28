#include "common.h"

/*
 * XeLoadSection
 *
 * Import Number:      327
 * Calling Convention: stdcall
 * Parameter 0:        PXBEIMAGE_SECTION Section
 * Return Type:        NTSTATUS
 */
int Xbox::XeLoadSection()
{
	K_ENTER_STDCALL();
	K_INIT_ARG_PTR(XBEIMAGE_SECTION, Section);
	XboxTypes::NTSTATUS rval;

	K_EXIT_WITH_VALUE(rval);
	return KF_ERROR_NOT_IMPLEMENTED;
}
