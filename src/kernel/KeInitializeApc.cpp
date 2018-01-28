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
	K_INIT_ARG_RPT(KAPC,              Apc);
	K_INIT_ARG_RPT(KTHREAD,           Thread);
	K_INIT_ARG_VAL(PKKERNEL_ROUTINE,  KernelRoutine);
	K_INIT_ARG_VAL(PKRUNDOWN_ROUTINE, RundownRoutine);
	K_INIT_ARG_VAL(PKNORMAL_ROUTINE,  NormalRoutine);
	K_INIT_ARG_VAL(KPROCESSOR_MODE,   ProcessorMode);
	K_INIT_ARG_PTR(VOID,              NormalContext);

	// FIXME: let the object manager initialize this
	pApc->Type = XboxTypes::ApcObject;
	pApc->Inserted = FALSE;
	pApc->Thread = Thread;

	pApc->KernelRoutine = KernelRoutine;
	pApc->RundownRoutine = RundownRoutine;
	pApc->NormalRoutine = NormalRoutine;
	if (NULL != NormalRoutine) {
		pApc->ApcMode = ProcessorMode;
		pApc->NormalContext = NormalContext;
	}
	else {
		pApc->ApcMode = XboxTypes::KernelMode;
		pApc->NormalContext = NULL;
	}

	K_EXIT();
	return KF_OK;
}
