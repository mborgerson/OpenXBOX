#include "common.h"

/*
 * KeInitializeTimerEx
 *
 * Import Number:      113
 * Calling Convention: stdcall
 * Parameter 0:        PKTIMER    Timer
 * Parameter 1:        TIMER_TYPE Type
 * Return Type:        VOID
 */
int Xbox::KeInitializeTimerEx()
{
	K_ENTER_STDCALL();
	K_INIT_ARG_PTR(KTIMER,     Timer);
	K_INIT_ARG_VAL(TIMER_TYPE, Type);

	// FIXME: let the object manager initialize this
	pTimer->Header.Size = sizeof(XboxTypes::KTIMER) / sizeof(XboxTypes::LONG);
	pTimer->Header.Type = XboxTypes::TimerNotificationObject + Type;
	pTimer->Header.Inserted = FALSE;
	pTimer->Header.SignalState = FALSE;
	pTimer->DueTime.QuadPart = 0;
	pTimer->Period = 0;
	InitializeListHead(&pTimer->TimerListEntry);

	K_EXIT();
	return KF_OK;
}
