#include "common.h"

/*
 * KeInsertQueue
 *
 * Import Number:      117
 * Calling Convention: stdcall
 * Parameter 0:        PRKQUEUE    Queue
 * Parameter 1:        PLIST_ENTRY Entry
 * Return Type:        LONG
 */
int Xbox::KeInsertQueue()
{
	K_ENTER_STDCALL();
	K_INIT_ARG(PRKQUEUE,    Queue);
	K_INIT_ARG(PLIST_ENTRY, Entry);
	XboxTypes::LONG rval;

	K_EXIT_WITH_VALUE(rval);
	return KF_ERROR_NOT_IMPLEMENTED;
}
