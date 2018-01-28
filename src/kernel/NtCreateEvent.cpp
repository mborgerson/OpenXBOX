#include "common.h"

/*
 * NtCreateEvent
 *
 * Import Number:      189
 * Calling Convention: stdcall
 * Parameter 0:        PHANDLE            EventHandle
 * Parameter 1:        POBJECT_ATTRIBUTES ObjectAttributes
 * Parameter 2:        EVENT_TYPE         EventType
 * Parameter 3:        BOOLEAN            InitialState
 * Return Type:        NTSTATUS
 */
int Xbox::NtCreateEvent()
{
	K_ENTER_STDCALL();
	K_INIT_ARG_PTR(HANDLE,            EventHandle);
	K_INIT_ARG_PTR(OBJECT_ATTRIBUTES, ObjectAttributes);
	K_INIT_ARG_VAL(EVENT_TYPE,        EventType);
	K_INIT_ARG_VAL(BOOLEAN,           InitialState);
	XboxTypes::NTSTATUS rval;

	// FIXME: These events need to be handled in the scheduler.

	K_EXIT_WITH_VALUE(rval);
	return KF_WARN_FAKE_IMPL;
}
