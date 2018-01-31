#include "common.h"

/*
 * WRITE_PORT_BUFFER_UCHAR
 *
 * Import Number:      332
 * Calling Convention: stdcall
 * Parameter 0:        PUCHAR Port
 * Parameter 1:        PUCHAR Buffer
 * Parameter 2:        ULONG  Count
 * Return Type:        VOID
 */
int Xbox::WRITE_PORT_BUFFER_UCHAR()
{
	K_ENTER_STDCALL();
	K_INIT_ARG(PUCHAR, Port);
	K_INIT_ARG(PUCHAR, Buffer);
	K_INIT_ARG(ULONG,  Count);

	K_EXIT();
	return KF_ERROR_NOT_IMPLEMENTED;
}
