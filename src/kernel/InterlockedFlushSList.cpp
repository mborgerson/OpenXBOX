#include "common.h"

/*
 * InterlockedFlushSList
 *
 * Import Number:      56
 * Calling Convention: fastcall
 * Parameter 0:        PSLIST_HEADER ListHead
 * Return Type:        PSINGLE_LIST_ENTRY
 */
int Xbox::InterlockedFlushSList()
{
	K_ENTER_FASTCALL();
	K_INIT_ARG(PSLIST_HEADER, ListHead);
	XboxTypes::PSINGLE_LIST_ENTRY rval;

	rval = m_kernel->InterlockedFlushSList(ListHead);

	K_EXIT_WITH_VALUE(rval);
	return KF_ERROR_NOT_IMPLEMENTED;
}
