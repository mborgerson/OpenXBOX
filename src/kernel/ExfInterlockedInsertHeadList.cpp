#include "common.h"

/*
 * ExfInterlockedInsertHeadList
 *
 * Import Number:      32
 * Calling Convention: fastcall
 * Parameter 0:        PLIST_ENTRY ListHead
 * Parameter 1:        PLIST_ENTRY ListEntry
 * Return Type:        PLIST_ENTRY
 */
int Xbox::ExfInterlockedInsertHeadList()
{
	K_ENTER_FASTCALL();
	K_INIT_ARG(PLIST_ENTRY, ListHead);
	K_INIT_ARG(PLIST_ENTRY, ListEntry);
	XboxTypes::PLIST_ENTRY rval;

	rval = m_kernel->ExfInterlockedInsertHeadList(ListHead, ListEntry);

	K_EXIT_WITH_VALUE(rval);
	return KF_OK;
}
