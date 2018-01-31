#include "kernel/impl/kernel.h"
#include "kernel/impl/error.h"
#include <ctype.h>

XboxTypes::SIZE_T XboxKernel::RtlCompareMemory(XboxTypes::PVOID Source1, XboxTypes::PVOID Source2, XboxTypes::SIZE_T Length) {
	XboxTypes::VOID *pSource1 = ToPointer<XboxTypes::VOID>(Source1);
	XboxTypes::VOID *pSource2 = ToPointer<XboxTypes::VOID>(Source2);
	XboxTypes::SIZE_T result = Length;

	uint8_t *pBytes1 = (uint8_t *)pSource1;
	uint8_t *pBytes2 = (uint8_t *)pSource2;
	for (uint32_t i = 0; i < Length; i++) {
		if (pBytes1[i] != pBytes2[i]) {
			result = i;
			break;
		}
	}
	return result;
}

XboxTypes::SIZE_T XboxKernel::RtlCompareMemoryUlong(XboxTypes::PVOID Source, XboxTypes::SIZE_T Length, XboxTypes::ULONG Pattern) {
	XboxTypes::VOID *pSource = ToPointer<XboxTypes::VOID>(Source);

	// Compare 32 bits at a time
	// Any extra bytes are ignored
	uint32_t numDwords = Length >> 2;
	uint32_t *pDwords = (uint32_t *)pSource;
	XboxTypes::SIZE_T result = Length;
	for (uint32_t i = 0; i < numDwords; i++) {
		if (pDwords[i] != Pattern) {
			result = i;
			break;
		}
	}
	return result;
}

XboxTypes::BOOLEAN XboxKernel::RtlEqualString(XboxTypes::PSTRING String1, XboxTypes::PSTRING String2, XboxTypes::BOOLEAN CaseInSensitive) {
	XboxTypes::STRING *pString1 = ToPointer<XboxTypes::STRING>(String1);
	XboxTypes::STRING *pString2 = ToPointer<XboxTypes::STRING>(String2);

	XboxTypes::USHORT l1 = pString1->Length;
	XboxTypes::USHORT l2 = pString2->Length;
	if (l1 != l2) {
		return FALSE;
	}
	
	XboxTypes::CHAR *p1 = ToPointer<XboxTypes::CHAR>(pString1->Buffer);
	XboxTypes::CHAR *p2 = ToPointer<XboxTypes::CHAR>(pString2->Buffer);
	XboxTypes::CHAR *last = p1 + l1;

	if (CaseInSensitive) {
		while (p1 < last) {
			XboxTypes::CHAR c1 = *p1++;
			XboxTypes::CHAR c2 = *p2++;
			if (c1 != c2) {
				// TODO: use RtlUpperChar instead of toupper
				// The issue with RtlUpperChar is that its parameter is a
				// register parameter, which the calling convention helper
				// classes cannot handle yet
				c1 = toupper(c1);
				c2 = toupper(c2);
				if (c1 != c2) {
					return FALSE;
				}
			}
		}
		return TRUE;
	}

	while (p1 < last) {
		if (*p1++ != *p2++) {
			return FALSE;
		}
	}
	return TRUE;
}

XboxTypes::VOID XboxKernel::RtlFillMemory(XboxTypes::PVOID Destination, XboxTypes::ULONG Length, XboxTypes::UCHAR Fill) {
	XboxTypes::VOID *pDestination = ToPointer<XboxTypes::VOID>(Destination);
	memset(pDestination, Fill, Length);
}

XboxTypes::VOID XboxKernel::RtlFillMemoryUlong(XboxTypes::PVOID Destination, XboxTypes::SIZE_T Length, XboxTypes::ULONG Pattern) {
	XboxTypes::VOID *pDestination = ToPointer<XboxTypes::VOID>(Destination);

	// Fill 32 bits at a time
	// Any extra bytes are ignored
	uint32_t numDwords = Length >> 2;
	uint32_t *lastAddr = (uint32_t *)pDestination + numDwords;
	for (uint32_t *p = (uint32_t *)pDestination; p < lastAddr; p++) {
		*p = Pattern;
	}
}

XboxTypes::VOID XboxKernel::RtlInitAnsiString(XboxTypes::PANSI_STRING DestinationString, XboxTypes::PCSZ SourceString) {
	XboxTypes::ANSI_STRING *pDestinationString = ToPointer<XboxTypes::ANSI_STRING>(DestinationString);

	pDestinationString->Buffer = SourceString;
	if (SourceString != NULL) {
		XboxTypes::CCHAR *pSourceString = ToPointer<XboxTypes::CCHAR>(SourceString);
		pDestinationString->Length = strlen(pSourceString);
		pDestinationString->MaximumLength = pDestinationString->Length + 1;
	}
	else {
		pDestinationString->Length = pDestinationString->MaximumLength = 0;
	}
}

XboxTypes::VOID XboxKernel::RtlInitializeCriticalSection(XboxTypes::PRTL_CRITICAL_SECTION CriticalSection) {
	XboxTypes::RTL_CRITICAL_SECTION *pCriticalSection = ToPointer<XboxTypes::RTL_CRITICAL_SECTION>(CriticalSection);
	
	// FIXME: let the object manager initialize this
	pCriticalSection->OwningThread = 0;
	pCriticalSection->LockCount = -1;
	pCriticalSection->RecursionCount = 0;
	// FIXME: what can we do about pointers to structs within structs?
	KeInitializeEvent(_PTR_TO_ADDR(RKEVENT, &pCriticalSection->Synchronization.Event), XboxTypes::SynchronizationEvent, FALSE);
}

XboxTypes::VOID XboxKernel::RtlMoveMemory(XboxTypes::PVOID Destination, XboxTypes::PPVOID Source, XboxTypes::ULONG Length) {
	XboxTypes::VOID *pDestination = ToPointer<XboxTypes::VOID>(Destination);
	XboxTypes::VOID *pSource = ToPointer<XboxTypes::VOID>(Source);
	memmove(pDestination, pSource, Length);
}

XboxTypes::ULONG XboxKernel::RtlNtStatusToDosError(XboxTypes::NTSTATUS Status) {
	return NTSTATUStoDOSerror(Status);
}

XboxTypes::ULONG XboxKernel::RtlUlongByteSwap(XboxTypes::ULONG Source) {
	return (Source >> 24) | ((Source & 0xFF0000) >> 8) | ((Source & 0xFF00) << 8) | ((Source & 0xFF) << 24);
}

XboxTypes::USHORT XboxKernel::RtlUshortByteSwap(XboxTypes::USHORT Source) {
	return (Source >> 8) | ((Source & 0xFF) << 8);
}

XboxTypes::VOID XboxKernel::RtlZeroMemory(XboxTypes::PVOID Destination, XboxTypes::SIZE_T Length) {
	XboxTypes::VOID *pDestination = ToPointer<XboxTypes::VOID>(Destination);
	memset(pDestination, 0, Length);
}
