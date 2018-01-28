#include "common.h"

/*
 * READ_PORT_BUFFER_USHORT
 *
 * Import Number:      330
 * Calling Convention: stdcall
 * Parameter 0:        PUSHORT Port
 * Parameter 1:        PUSHORT Buffer
 * Parameter 2:        ULONG   Count
 * Return Type:        VOID
 */
int Xbox::READ_PORT_BUFFER_USHORT()
{
	K_ENTER_STDCALL();
	K_INIT_ARG_PTR(USHORT, Port);
	K_INIT_ARG_PTR(USHORT, Buffer);
	K_INIT_ARG_VAL(ULONG,  Count);

	K_EXIT();
	return KF_ERROR_NOT_IMPLEMENTED;
}
