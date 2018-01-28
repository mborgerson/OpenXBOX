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
	K_INIT_ARG(XboxTypes::PKTIMER,    Timer);
	K_INIT_ARG(XboxTypes::TIMER_TYPE, Type);

	XboxTypes::KTIMER *timer = _ADDR_TO_PTR(KTIMER, Timer);
	timer->Header.Size = sizeof(XboxTypes::KTIMER) / sizeof(LONG);
	timer->Header.Type = XboxTypes::TimerNotificationObject + Type;
	timer->Header.Inserted = FALSE;
	timer->Header.SignalState = FALSE;
	timer->DueTime.QuadPart = 0;
	timer->Period = 0;
	InitializeListHead(&timer->TimerListEntry);

	K_EXIT();
	return 0;
}
