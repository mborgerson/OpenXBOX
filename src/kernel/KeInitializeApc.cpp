#include "common.h"

/*
 * KeInitializeApc
 *
 * Import Number:      105
 * Calling Convention: stdcall
 * Parameter 0:        PRKAPC            Apc
 * Parameter 1:        PRKTHREAD         Thread
 * Parameter 2:        PKKERNEL_ROUTINE  KernelRoutine
 * Parameter 3:        PKRUNDOWN_ROUTINE RundownRoutine
 * Parameter 4:        PKNORMAL_ROUTINE  NormalRoutine
 * Parameter 5:        KPROCESSOR_MODE   ProcessorMode
 * Parameter 6:        PVOID             NormalContext
 * Return Type:        VOID
 */
int Xbox::KeInitializeApc()
{
	K_ENTER_STDCALL();
	K_INIT_ARG_VAL(PRKAPC,            Apc);
	K_INIT_ARG_VAL(PRKTHREAD,         Thread);
	K_INIT_ARG_VAL(PKKERNEL_ROUTINE,  KernelRoutine);
	K_INIT_ARG_VAL(PKRUNDOWN_ROUTINE, RundownRoutine);
	K_INIT_ARG_VAL(PKNORMAL_ROUTINE,  NormalRoutine);
	K_INIT_ARG_VAL(KPROCESSOR_MODE,   ProcessorMode);
	K_INIT_ARG_VAL(PVOID,             NormalContext);

	m_kernel->KeInitializeApc(Apc, Thread, KernelRoutine, RundownRoutine, NormalRoutine, ProcessorMode, NormalContext);

	K_EXIT();
	return KF_OK;
}
