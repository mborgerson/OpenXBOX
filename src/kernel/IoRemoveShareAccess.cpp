#include "common.h"

/*
 * IoRemoveShareAccess
 *
 * Import Number:      78
 * Calling Convention: stdcall
 * Parameter 0:        PFILE_OBJECT  FileObject
 * Parameter 1:        PSHARE_ACCESS ShareAccess
 * Return Type:        VOID
 */
int Xbox::IoRemoveShareAccess()
{
	K_ENTER_STDCALL();
	K_INIT_ARG_PTR(FILE_OBJECT,  FileObject);
	K_INIT_ARG_PTR(SHARE_ACCESS, ShareAccess);

	K_EXIT();
	return KF_ERROR_NOT_IMPLEMENTED;
}
