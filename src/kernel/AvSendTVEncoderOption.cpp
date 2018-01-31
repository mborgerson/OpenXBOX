#include "common.h"

/*
 * AvSendTVEncoderOption
 *
 * Import Number:      2
 * Calling Convention: stdcall
 * Parameter 0:        PVOID  RegisterBase
 * Parameter 1:        ULONG  Option
 * Parameter 2:        ULONG  Param
 * Parameter 3:        PULONG Result
 * Return Type:        VOID
 */
int Xbox::AvSendTVEncoderOption()
{
	K_ENTER_STDCALL();
	K_INIT_ARG(PVOID,  RegisterBase);
	K_INIT_ARG(ULONG,  Option);
	K_INIT_ARG(ULONG,  Param);
	K_INIT_ARG(PULONG, Result);

	m_kernel->AvSendTVEncoderOption(RegisterBase, Option, Param, Result);

	K_EXIT();
	return KF_WARN_PARTIAL_IMPL;
}
