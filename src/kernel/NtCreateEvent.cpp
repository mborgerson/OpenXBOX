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
	K_INIT_ARG_VAL(PHANDLE,            EventHandle);
	K_INIT_ARG_VAL(POBJECT_ATTRIBUTES, ObjectAttributes);
	K_INIT_ARG_VAL(EVENT_TYPE,         EventType);
	K_INIT_ARG_VAL(BOOLEAN,            InitialState);
	XboxTypes::NTSTATUS rval;

	rval = m_kernel->NtCreateEvent(EventHandle, ObjectAttributes, EventType, InitialState);

	K_EXIT_WITH_VALUE(rval);
	return KF_WARN_FAKE_IMPL;
}
