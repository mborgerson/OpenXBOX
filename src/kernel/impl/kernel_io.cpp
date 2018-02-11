#include "kernel/impl/kernel.h"

XboxTypes::PIRP XboxKernel::IoAllocateIrp(XboxTypes::CCHAR StackSize) {
	XboxTypes::USHORT packetSize = sizeof(XboxTypes::IRP) + StackSize * sizeof(XboxTypes::IO_STACK_LOCATION);
	XboxTypes::PIRP irp = ExAllocatePoolWithTag(packetSize, ' prI');
	XboxTypes::IRP *pIrp = ToPointer<XboxTypes::IRP>(irp);
	if (irp != NULL) {
		IoInitializeIrp(irp, packetSize, StackSize);
	}

	return irp;
}

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

XboxTypes::VOID XboxKernel::IoFreeIrp(XboxTypes::PIRP Irp) {
	XboxTypes::IRP *pIrp = ToPointer<XboxTypes::IRP>(Irp);

	if (pIrp->Type != IO_TYPE_IRP) {
		// Second parameter is reserved according to MSDN, but the code uses
		// this weird value out of nowhere
		// https://docs.microsoft.com/en-us/windows-hardware/drivers/debugger/bug-check-0x44--multiple-irp-complete-requests
		KeBugCheckEx(MULTIPLE_IRP_COMPLETE_REQUESTS, Irp, 0x7f8, 0, 0);
	}
	// Not sure why this is set to 0 if the struct is going to be freed anyway
	pIrp->Type = 0;
	ExFreePool(Irp);
}

XboxTypes::VOID XboxKernel::IoInitializeIrp(XboxTypes::PIRP Irp, XboxTypes::USHORT PacketSize, XboxTypes::CCHAR StackSize) {
	XboxTypes::IRP *pIrp = ToPointer<XboxTypes::IRP>(Irp);

	RtlZeroMemory(Irp, PacketSize);
	pIrp->Type = IO_TYPE_IRP;
	pIrp->Size = PacketSize;
	pIrp->StackCount = StackSize;
	pIrp->CurrentLocation = StackSize + 1;
	InitializeListHead(&pIrp->ThreadListEntry);
	pIrp->Tail.Overlay.CurrentStackLocation = (XboxTypes::PIO_STACK_LOCATION)((XboxTypes::UCHAR *)pIrp + sizeof(XboxTypes::IRP) + StackSize * sizeof(XboxTypes::IO_STACK_LOCATION));
}

XboxTypes::VOID XboxKernel::IoQueueThreadIrp(XboxTypes::PIRP Irp) {
	XboxTypes::IRP *pIrp = ToPointer<XboxTypes::IRP>(Irp);

	XboxTypes::KIRQL oldIRQL = KfRaiseIrql(APC_LEVEL);
	XboxTypes::ETHREAD *pEThread = ToPointer<XboxTypes::ETHREAD>(pIrp->Tail.Overlay.Thread);
	InsertHeadList(&pEThread->IrpList, &pIrp->ThreadListEntry);
	KfLowerIrql(oldIRQL);
}

XboxTypes::VOID XboxKernel::IoRemoveShareAccess(XboxTypes::PFILE_OBJECT FileObject, XboxTypes::PSHARE_ACCESS ShareAccess) {
	XboxTypes::FILE_OBJECT *pFileObject = ToPointer<XboxTypes::FILE_OBJECT>(FileObject);
	XboxTypes::SHARE_ACCESS *pShareAccess = ToPointer<XboxTypes::SHARE_ACCESS>(ShareAccess);

	if (pFileObject->ReadAccess || pFileObject->WriteAccess || pFileObject->DeleteAccess) {
		pShareAccess->OpenCount--;
		if (pFileObject->ReadAccess) {
			pShareAccess->Readers--;
		}
		if (pFileObject->WriteAccess) {
			pShareAccess->Writers--;
		}
		if (pFileObject->DeleteAccess) {
			pShareAccess->Deleters--;
		}
		
		if (pFileObject->SharedRead) {
			pShareAccess->SharedRead--;
		}
		if (pFileObject->SharedWrite) {
			pShareAccess->SharedWrite--;
		}
		if (pFileObject->SharedDelete) {
			pShareAccess->SharedDelete--;
		}
	}
}

XboxTypes::VOID XboxKernel::IoSetShareAccess(XboxTypes::ACCESS_MASK DesiredAccess, XboxTypes::ULONG DesiredShareAccess, XboxTypes::PFILE_OBJECT FileObject, XboxTypes::PSHARE_ACCESS ShareAccess) {
	XboxTypes::FILE_OBJECT *pFileObject = ToPointer<XboxTypes::FILE_OBJECT>(FileObject);
	XboxTypes::SHARE_ACCESS *pShareAccess = ToPointer<XboxTypes::SHARE_ACCESS>(ShareAccess);
	
	pFileObject->ReadAccess = (DesiredAccess & (FILE_EXECUTE | FILE_READ_DATA)) != 0;
	pFileObject->WriteAccess = (DesiredAccess & (FILE_WRITE_DATA | FILE_APPEND_DATA)) != 0;
	pFileObject->DeleteAccess = (DesiredAccess & DELETE) != 0;

	if (pFileObject->ReadAccess || pFileObject->WriteAccess || pFileObject->DeleteAccess) {
		pFileObject->SharedRead = (DesiredShareAccess & FILE_SHARE_READ) != 0;
		pFileObject->SharedWrite = (DesiredShareAccess & FILE_SHARE_WRITE) != 0;
		pFileObject->SharedDelete = (DesiredShareAccess & FILE_SHARE_DELETE) != 0;
		
		pShareAccess->OpenCount = 1;

		pShareAccess->Readers = pFileObject->ReadAccess;
		pShareAccess->Writers = pFileObject->WriteAccess;
		pShareAccess->Deleters = pFileObject->DeleteAccess;

		pShareAccess->SharedRead = pFileObject->SharedRead;
		pShareAccess->SharedWrite = pFileObject->SharedWrite;
		pShareAccess->SharedDelete = pFileObject->SharedDelete;

	}
	else {
		pShareAccess->OpenCount = 0;
		
		pShareAccess->Readers = 0;
		pShareAccess->Writers = 0;
		pShareAccess->Deleters = 0;
		
		pShareAccess->SharedRead = 0;
		pShareAccess->SharedWrite = 0;
		pShareAccess->SharedDelete = 0;
	}
}
