#include "common.h"

/*
 * IoFreeIrp
 *
 * Import Number:      72
 * Calling Convention: stdcall
 * Parameter 0:        PIRP Irp
 * Return Type:        VOID
 */
int Xbox::IoFreeIrp()
{
	K_ENTER_STDCALL();
	K_INIT_ARG(PIRP, Irp);

	m_kernel->IoFreeIrp(Irp);

	K_EXIT();
	return KF_OK;
}
