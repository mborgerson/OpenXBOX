#include "kernel/impl/kernel.h"

XboxTypes::NTSTATUS XboxKernel::IoCreateFile(
	XboxTypes::PHANDLE FileHandle,
	XboxTypes::ACCESS_MASK DesiredAccess,
	XboxTypes::POBJECT_ATTRIBUTES ObjectAttributes,
	XboxTypes::PIO_STATUS_BLOCK IoStatusBlock,
	XboxTypes::PLARGE_INTEGER AllocationSize,
	XboxTypes::ULONG FileAttributes,
	XboxTypes::ULONG ShareAccess,
	XboxTypes::ULONG Disposition,
	XboxTypes::ULONG CreateOptions,
	XboxTypes::ULONG Options
) {
	// FIXME: implement properly
	XboxTypes::HANDLE *pHandle = ToPointer<XboxTypes::HANDLE>(FileHandle);

	*pHandle = 0xc;

	return STATUS_SUCCESS;
}

XboxTypes::NTSTATUS XboxKernel::IoCreateSymbolicLink(XboxTypes::POBJECT_STRING SymbolicLinkName, XboxTypes::POBJECT_STRING DeviceName) {
	// FIXME: implement properly
	return STATUS_SUCCESS;
}
