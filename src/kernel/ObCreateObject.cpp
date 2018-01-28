#include "common.h"

/*
 * ObCreateObject
 *
 * Import Number:      239
 * Calling Convention: stdcall
 * Parameter 0:        POBJECT_TYPE       ObjectType
 * Parameter 1:        POBJECT_ATTRIBUTES ObjectAttributes
 * Parameter 2:        ULONG              ObjectBodySize
 * Parameter 3:        PPVOID             Object
 * Return Type:        NTSTATUS
 */
int Xbox::ObCreateObject()
{
	K_ENTER_STDCALL();
	K_INIT_ARG_PTR(OBJECT_TYPE,       ObjectType);
	K_INIT_ARG_PTR(OBJECT_ATTRIBUTES, ObjectAttributes);
	K_INIT_ARG_VAL(ULONG,             ObjectBodySize);
	K_INIT_ARG_PTR(PVOID,             Object);
	XboxTypes::NTSTATUS rval;

	K_EXIT_WITH_VALUE(rval);
	return KF_ERROR_NOT_IMPLEMENTED;
}
