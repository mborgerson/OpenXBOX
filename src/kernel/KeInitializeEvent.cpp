#include "common.h"

/*
 * KeInitializeEvent
 *
 * Import Number:      108
 * Calling Convention: stdcall
 * Parameter 0:        PRKEVENT   Event
 * Parameter 1:        EVENT_TYPE Type
 * Parameter 2:        BOOLEAN    State
 * Return Type:        VOID
 */
int Xbox::KeInitializeEvent()
{
	K_ENTER_STDCALL();
	K_INIT_ARG_RPT(KEVENT,     Event);
	K_INIT_ARG_VAL(EVENT_TYPE, Type);
	K_INIT_ARG_VAL(BOOLEAN,    State);

	// FIXME: let the object manager initialize this
	pEvent->Header.Size = sizeof(XboxTypes::KEVENT) / sizeof(XboxTypes::LONG);
	pEvent->Header.Type = (XboxTypes::UCHAR)Type;
	pEvent->Header.SignalState = State;
	InitializeListHead(&pEvent->Header.WaitListHead);

	K_EXIT();
	return KF_OK;
}
