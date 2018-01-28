#include "common.h"

/*
 * MmPersistContiguousMemory
 *
 * Import Number:      178
 * Calling Convention: stdcall
 * Parameter 0:        PVOID   BaseAddress
 * Parameter 1:        SIZE_T  NumberOfBytes
 * Parameter 2:        BOOLEAN Persist
 * Return Type:        VOID
 */
int Xbox::MmPersistContiguousMemory()
{
	K_ENTER_STDCALL();
	K_INIT_ARG_PTR(VOID,    BaseAddress);
	K_INIT_ARG_VAL(SIZE_T,  NumberOfBytes);
	K_INIT_ARG_VAL(BOOLEAN, Persist);

	m_memmgr->SetPersist(BaseAddress, NumberOfBytes, Persist);

	K_EXIT();
	return KF_WARN_FAKE_IMPL;
}
