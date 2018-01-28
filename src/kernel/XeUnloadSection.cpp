#include "common.h"

/*
 * XeUnloadSection
 *
 * Import Number:      328
 * Calling Convention: stdcall
 * Parameter 0:        PXBEIMAGE_SECTION Section
 * Return Type:        NTSTATUS
 */
int Xbox::XeUnloadSection()
{
	K_ENTER_STDCALL();
	K_INIT_ARG_PTR(XBEIMAGE_SECTION, Section);
	XboxTypes::NTSTATUS rval;

	K_EXIT_WITH_VALUE(rval);
	return KF_ERROR_NOT_IMPLEMENTED;
}
