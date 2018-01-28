#include "common.h"

/*
 * READ_PORT_BUFFER_UCHAR
 *
 * Import Number:      329
 * Calling Convention: stdcall
 * Parameter 0:        PUCHAR Port
 * Parameter 1:        PUCHAR Buffer
 * Parameter 2:        ULONG  Count
 * Return Type:        VOID
 */
int Xbox::READ_PORT_BUFFER_UCHAR()
{
	K_ENTER_STDCALL();
	K_INIT_ARG_PTR(UCHAR, Port);
	K_INIT_ARG_PTR(UCHAR, Buffer);
	K_INIT_ARG_VAL(ULONG, Count);

	K_EXIT();
	return KF_ERROR_NOT_IMPLEMENTED;
}
