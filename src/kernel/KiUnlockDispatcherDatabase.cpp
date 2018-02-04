#include "common.h"

/*
 * KiUnlockDispatcherDatabase
 *
 * Import Number:      163
 * Calling Convention: fastcall
 * Parameter 0:        KIRQL OldIrql
 * Return Type:        VOID
 */
int Xbox::KiUnlockDispatcherDatabase()
{
	K_ENTER_FASTCALL();
	K_INIT_ARG(KIRQL, OldIrql);

	m_kernel->KiUnlockDispatcherDatabase(OldIrql);

	K_EXIT();
	return KF_WARN_PARTIAL_IMPL;
}
