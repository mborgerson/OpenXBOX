#include "common.h"

/*
 * InterlockedPushEntrySList
 *
 * Import Number:      58
 * Calling Convention: fastcall
 * Parameter 0:        PSLIST_HEADER      ListHead
 * Parameter 1:        PSINGLE_LIST_ENTRY ListEntry
 * Return Type:        PSINGLE_LIST_ENTRY
 */
int Xbox::InterlockedPushEntrySList()
{
	K_ENTER_FASTCALL();
	K_INIT_ARG(PSLIST_HEADER,      ListHead);
	K_INIT_ARG(PSINGLE_LIST_ENTRY, ListEntry);
	XboxTypes::PSINGLE_LIST_ENTRY rval;

	rval = m_kernel->InterlockedPushEntrySList(ListHead, ListEntry);

	K_EXIT_WITH_VALUE(rval);
	return KF_OK;
}
