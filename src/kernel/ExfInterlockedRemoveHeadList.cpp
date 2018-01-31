#include "common.h"

/*
 * ExfInterlockedRemoveHeadList
 *
 * Import Number:      34
 * Calling Convention: fastcall
 * Parameter 0:        PLIST_ENTRY ListHead
 * Return Type:        PLIST_ENTRY
 */
int Xbox::ExfInterlockedRemoveHeadList()
{
	K_ENTER_FASTCALL();
	K_INIT_ARG(PLIST_ENTRY, ListHead);
	XboxTypes::PLIST_ENTRY rval;

	K_EXIT_WITH_VALUE(rval);
	return KF_ERROR_NOT_IMPLEMENTED;
}
