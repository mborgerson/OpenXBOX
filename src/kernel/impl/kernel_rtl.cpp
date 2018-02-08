#include "kernel/impl/kernel.h"
#include "kernel/impl/error.h"
#include <ctype.h>
#include "log.h"

XboxTypes::NTSTATUS XboxKernel::RtlAppendStringToString(XboxTypes::PSTRING Destination, XboxTypes::PSTRING Source) {
	XboxTypes::STRING *pDestination = ToPointer<XboxTypes::STRING>(Destination);
	XboxTypes::STRING *pSource = ToPointer<XboxTypes::STRING>(Source);

	XboxTypes::USHORT dstLen = pDestination->Length;
	XboxTypes::USHORT srcLen = pSource->Length;
	if (srcLen > 0) {
		if ((srcLen + dstLen) > pDestination->MaximumLength) {
			return STATUS_BUFFER_TOO_SMALL;
		}

		XboxTypes::CHAR *dstBuf = ToPointer<XboxTypes::CHAR>(pDestination->Buffer);
		XboxTypes::CHAR *srcBuf = ToPointer<XboxTypes::CHAR>(pSource->Buffer);
		memmove(dstBuf, srcBuf, srcLen);
		pDestination->Length += srcLen;
	}

	return STATUS_SUCCESS;
}

XboxTypes::NTSTATUS XboxKernel::RtlAppendUnicodeStringToString(XboxTypes::PUNICODE_STRING Destination, XboxTypes::PUNICODE_STRING Source) {
	XboxTypes::STRING *pDestination = ToPointer<XboxTypes::STRING>(Destination);
	XboxTypes::STRING *pSource = ToPointer<XboxTypes::STRING>(Source);

	XboxTypes::USHORT dstLen = pDestination->Length;
	XboxTypes::USHORT srcLen = pSource->Length;
	if (srcLen > 0) {
		if ((srcLen + dstLen) > pDestination->MaximumLength) {
			return STATUS_BUFFER_TOO_SMALL;
		}

		XboxTypes::CHAR *dstBuf = ToPointer<XboxTypes::CHAR>(pDestination->Buffer + (pDestination->Length / sizeof(XboxTypes::WCHAR)));
		XboxTypes::CHAR *srcBuf = ToPointer<XboxTypes::CHAR>(pSource->Buffer);
		memmove(dstBuf, srcBuf, srcLen);
		pDestination->Length += srcLen;
		if (pDestination->Length < pDestination->MaximumLength) {
			dstBuf[srcLen / sizeof(XboxTypes::WCHAR)] = UNICODE_NULL;
		}
	}

	return STATUS_SUCCESS;
}

XboxTypes::NTSTATUS XboxKernel::RtlAppendUnicodeToString(XboxTypes::PUNICODE_STRING Destination, XboxTypes::PCWSTR Source) {
	if (Source != NULL) {
		XboxTypes::UNICODE_STRING unicodeString;
		RtlInitUnicodeString(_PTR_TO_ADDR(UNICODE_STRING, &unicodeString), Source);
		return RtlAppendUnicodeStringToString(Destination, _PTR_TO_ADDR(UNICODE_STRING, &unicodeString));
	}

	return STATUS_SUCCESS;
}

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

XboxTypes::LONG XboxKernel::RtlCompareString(XboxTypes::PSTRING String1, XboxTypes::PSTRING String2, XboxTypes::BOOLEAN CaseInSensitive) {
	XboxTypes::STRING *pString1 = ToPointer<XboxTypes::STRING>(String1);
	XboxTypes::STRING *pString2 = ToPointer<XboxTypes::STRING>(String2);

	XboxTypes::USHORT l1 = pString1->Length;
	XboxTypes::USHORT l2 = pString2->Length;
	XboxTypes::USHORT maxLen = l1 <= l2 ? l1 : l2;

	XboxTypes::CHAR *str1 = ToPointer<XboxTypes::CHAR>(pString1->Buffer);
	XboxTypes::CHAR *str2 = ToPointer<XboxTypes::CHAR>(pString2->Buffer);

	if (CaseInSensitive) {
		return _strnicmp(str1, str2, maxLen);
	}
	return strncmp(str1, str2, maxLen);
}

XboxTypes::LONG XboxKernel::RtlCompareUnicodeString(XboxTypes::PUNICODE_STRING String1, XboxTypes::PUNICODE_STRING String2, XboxTypes::BOOLEAN CaseInSensitive) {
	XboxTypes::UNICODE_STRING *pString1 = ToPointer<XboxTypes::UNICODE_STRING>(String1);
	XboxTypes::UNICODE_STRING *pString2 = ToPointer<XboxTypes::UNICODE_STRING>(String2);

	XboxTypes::USHORT l1 = pString1->Length;
	XboxTypes::USHORT l2 = pString2->Length;
	XboxTypes::USHORT maxLen = l1 <= l2 ? l1 : l2;

	XboxTypes::WCHAR *str1 = ToPointer<XboxTypes::WCHAR>(pString1->Buffer);
	XboxTypes::WCHAR *str2 = ToPointer<XboxTypes::WCHAR>(pString2->Buffer);

	if (CaseInSensitive) {
		return _wcsnicmp(str1, str2, maxLen);
	}
	return wcsncmp(str1, str2, maxLen);
}

XboxTypes::VOID XboxKernel::RtlCopyString(XboxTypes::PSTRING DestinationString, XboxTypes::PSTRING SourceString) {
	XboxTypes::STRING *pDestinationString = ToPointer<XboxTypes::STRING>(DestinationString);
	XboxTypes::STRING *pSourceString = ToPointer<XboxTypes::STRING>(SourceString);

	if (SourceString == NULL) {
		pDestinationString->Length = 0;
		return;
	}

	XboxTypes::CHAR *pd = ToPointer<XboxTypes::CHAR>(pDestinationString->Buffer);
	XboxTypes::CHAR *ps = ToPointer<XboxTypes::CHAR>(pSourceString->Buffer);
	XboxTypes::USHORT len = pSourceString->Length;
	if ((XboxTypes::USHORT)len > pDestinationString->MaximumLength) {
		len = pDestinationString->MaximumLength;
	}
	pDestinationString->Length = (XboxTypes::USHORT)len;
	memcpy(pd, ps, len);
}

XboxTypes::VOID XboxKernel::RtlCopyUnicodeString(XboxTypes::PUNICODE_STRING DestinationString, XboxTypes::PUNICODE_STRING SourceString) {
	XboxTypes::UNICODE_STRING *pDestinationString = ToPointer<XboxTypes::UNICODE_STRING>(DestinationString);
	XboxTypes::UNICODE_STRING *pSourceString = ToPointer<XboxTypes::UNICODE_STRING>(SourceString);

	if (SourceString == NULL) {
		pDestinationString->Length = 0;
		return;
	}

	XboxTypes::WCHAR *pd = ToPointer<XboxTypes::WCHAR>(pDestinationString->Buffer);
	XboxTypes::WCHAR *ps = ToPointer<XboxTypes::WCHAR>(pSourceString->Buffer);
	XboxTypes::USHORT len = pSourceString->Length;
	if ((XboxTypes::USHORT)len > pDestinationString->MaximumLength) {
		len = pDestinationString->MaximumLength;
	}
	pDestinationString->Length = (XboxTypes::USHORT)len;
	memcpy(pd, ps, len);
	if (pDestinationString->Length < pDestinationString->MaximumLength) {
		pd[len / sizeof(XboxTypes::WCHAR)] = UNICODE_NULL;
	}
}

XboxTypes::BOOLEAN XboxKernel::RtlCreateUnicodeString(XboxTypes::PUNICODE_STRING DestinationString, XboxTypes::PCWSTR SourceString) {
	XboxTypes::UNICODE_STRING *pDestinationString = ToPointer<XboxTypes::UNICODE_STRING>(DestinationString);
	XboxTypes::WCHAR *pSourceString = ToPointer<XboxTypes::WCHAR>(SourceString);

	XboxTypes::ULONG bufferSize = (wcslen(pSourceString) + 1) * sizeof(XboxTypes::WCHAR);
	pDestinationString->Buffer = ExAllocatePoolWithTag(bufferSize, 'grtS');
	if (!pDestinationString->Buffer) {
		return FALSE;
	}

	RtlMoveMemory(pDestinationString->Buffer, SourceString, bufferSize);
	pDestinationString->MaximumLength = (XboxTypes::USHORT)bufferSize;
	pDestinationString->Length = (XboxTypes::USHORT)(bufferSize - sizeof(UNICODE_NULL));
	return TRUE;
}

XboxTypes::WCHAR XboxKernel::RtlDowncaseUnicodeChar(XboxTypes::WCHAR SourceCharacter) {
	return towlower(SourceCharacter);
}

XboxTypes::NTSTATUS XboxKernel::RtlDowncaseUnicodeString(XboxTypes::PUNICODE_STRING DestinationString, XboxTypes::PUNICODE_STRING SourceString, XboxTypes::BOOLEAN AllocateDestinationString) {
	XboxTypes::UNICODE_STRING *pDestinationString = ToPointer<XboxTypes::UNICODE_STRING>(DestinationString);
	XboxTypes::UNICODE_STRING *pSourceString = ToPointer<XboxTypes::UNICODE_STRING>(SourceString);

	if (AllocateDestinationString) {
		pDestinationString->MaximumLength = pSourceString->Length;
		pDestinationString->Buffer = ExAllocatePoolWithTag((XboxTypes::ULONG)pDestinationString->MaximumLength, 'grtS');
		if (pDestinationString->Buffer == NULL) {
			return STATUS_NO_MEMORY;
		}
	}
	else {
		if (pSourceString->Length > pDestinationString->MaximumLength) {
			return STATUS_BUFFER_OVERFLOW;
		}
	}

	XboxTypes::ULONG length = ((XboxTypes::ULONG)pSourceString->Length) / sizeof(XboxTypes::WCHAR);
	XboxTypes::WCHAR *pDst = ToPointer<XboxTypes::WCHAR>(pDestinationString->Buffer);
	XboxTypes::WCHAR *pSrc = ToPointer<XboxTypes::WCHAR>(pSourceString->Buffer);
	for (XboxTypes::ULONG i = 0; i < length; i++) {
		pDst[i] = (XboxTypes::WCHAR)towlower(pSrc[i]);
	}

	pDestinationString->Length = pSourceString->Length;

	return STATUS_SUCCESS;
}

XboxTypes::VOID XboxKernel::RtlEnterCriticalSection(XboxTypes::PRTL_CRITICAL_SECTION CriticalSection) {
	XboxTypes::RTL_CRITICAL_SECTION *pCriticalSection = ToPointer<XboxTypes::RTL_CRITICAL_SECTION>(CriticalSection);
	
	pCriticalSection->LockCount++;
	if (pCriticalSection->LockCount == 0) {
		pCriticalSection->OwningThread = m_pKPCR->PrcbData.CurrentThread;
		pCriticalSection->RecursionCount = 1;
		return;
	}

	if (pCriticalSection->OwningThread == m_pKPCR->PrcbData.CurrentThread) {
		pCriticalSection->RecursionCount++;
		return;
	}

	// FIXME: at this point, we should somehow switch to another thread
	// TODO: KeWaitForSingleObject(CriticalSection, XboxTypes::WrExecutive, 0, 0, 0);
	// FIXME: once we switch back to the original thread, we should continue
	// execution from this point on
	// See https://github.com/StrikerX3/OpenXBOX/issues/1
	pCriticalSection->OwningThread = m_pKPCR->PrcbData.CurrentThread;
	pCriticalSection->RecursionCount = 1;
}

XboxTypes::VOID XboxKernel::RtlEnterCriticalSectionAndRegion(XboxTypes::PRTL_CRITICAL_SECTION CriticalSection) {
	XboxTypes::KTHREAD *pCurrentThread = ToPointer<XboxTypes::KTHREAD>(m_pKPCR->PrcbData.CurrentThread);
	pCurrentThread->KernelApcDisable--;

	RtlEnterCriticalSection(CriticalSection);
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
				c1 = RtlUpperChar(c1);
				c2 = RtlUpperChar(c2);
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

XboxTypes::BOOLEAN XboxKernel::RtlEqualUnicodeString(XboxTypes::PCUNICODE_STRING String1, XboxTypes::PCUNICODE_STRING String2, XboxTypes::BOOLEAN CaseInSensitive) {
	XboxTypes::UNICODE_STRING *pString1 = ToPointer<XboxTypes::UNICODE_STRING>(String1);
	XboxTypes::UNICODE_STRING *pString2 = ToPointer<XboxTypes::UNICODE_STRING>(String2);

	XboxTypes::USHORT l1 = pString1->Length;
	XboxTypes::USHORT l2 = pString2->Length;
	if (l1 != l2) {
		return FALSE;
	}

	XboxTypes::WCHAR *p1 = ToPointer<XboxTypes::WCHAR>(pString1->Buffer);
	XboxTypes::WCHAR *p2 = ToPointer<XboxTypes::WCHAR>(pString2->Buffer);
	XboxTypes::WCHAR *last = p1 + l1;

	if (CaseInSensitive) {
		while (p1 < last) {
			XboxTypes::WCHAR c1 = *p1++;
			XboxTypes::WCHAR c2 = *p2++;
			if (c1 != c2) {
				c1 = towupper(c1);
				c2 = towupper(c2);
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

XboxTypes::LARGE_INTEGER XboxKernel::RtlExtendedIntegerMultiply(XboxTypes::LARGE_INTEGER Multiplicand, XboxTypes::LONG Multiplier) {
	XboxTypes::LARGE_INTEGER result;
	result.QuadPart = Multiplicand.QuadPart * (XboxTypes::LONGLONG)Multiplier;
	return result;
}

XboxTypes::LARGE_INTEGER XboxKernel::RtlExtendedLargeIntegerDivide(XboxTypes::LARGE_INTEGER Dividend, XboxTypes::LONG Divisor) {
	XboxTypes::LARGE_INTEGER result;
	result.QuadPart = Dividend.QuadPart / (XboxTypes::LONGLONG)Divisor;
	return result;
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

XboxTypes::VOID XboxKernel::RtlFreeAnsiString(XboxTypes::PANSI_STRING AnsiString) {
	XboxTypes::ANSI_STRING *pAnsiString = ToPointer<XboxTypes::ANSI_STRING>(AnsiString);

	if (pAnsiString->Buffer) {
		ExFreePool(pAnsiString->Buffer);
		memset(pAnsiString, 0, sizeof(*pAnsiString));
	}
}

XboxTypes::VOID XboxKernel::RtlFreeUnicodeString(XboxTypes::PUNICODE_STRING UnicodeString) {
	XboxTypes::UNICODE_STRING *pUnicodeString = ToPointer<XboxTypes::UNICODE_STRING>(UnicodeString);

	if (pUnicodeString->Buffer) {
		ExFreePool(pUnicodeString->Buffer);
		memset(pUnicodeString, 0, sizeof(*pUnicodeString));
	}
}

XboxTypes::VOID XboxKernel::RtlInitAnsiString(XboxTypes::PANSI_STRING DestinationString, XboxTypes::PCSZ SourceString) {
	XboxTypes::ANSI_STRING *pDestinationString = ToPointer<XboxTypes::ANSI_STRING>(DestinationString);

	pDestinationString->Buffer = SourceString;
	if (SourceString != NULL) {
		XboxTypes::CCHAR *pSourceString = ToPointer<XboxTypes::CCHAR>(SourceString);
		pDestinationString->Buffer = SourceString;
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

XboxTypes::VOID XboxKernel::RtlInitUnicodeString(XboxTypes::PUNICODE_STRING DestinationString, XboxTypes::PWSZ SourceString) {
	XboxTypes::UNICODE_STRING *pDestinationString = ToPointer<XboxTypes::UNICODE_STRING>(DestinationString);

	pDestinationString->Buffer = SourceString;
	if (SourceString != NULL) {
		XboxTypes::WCHAR *pSourceString = ToPointer<XboxTypes::WCHAR>(SourceString);
		pDestinationString->Buffer = SourceString;
		pDestinationString->Length = wcslen(pSourceString) * 2;
		pDestinationString->MaximumLength = pDestinationString->Length + 2;
	}
	else {
		pDestinationString->Length = pDestinationString->MaximumLength = 0;
	}
}

XboxTypes::VOID XboxKernel::RtlLeaveCriticalSection(XboxTypes::PRTL_CRITICAL_SECTION CriticalSection) {
	XboxTypes::RTL_CRITICAL_SECTION *pCriticalSection = ToPointer<XboxTypes::RTL_CRITICAL_SECTION>(CriticalSection);

	pCriticalSection->RecursionCount--;
	if (pCriticalSection->RecursionCount != 0) {
		pCriticalSection->LockCount--;
		return;
	}

	pCriticalSection->OwningThread = NULL;
	pCriticalSection->LockCount--;
	if (pCriticalSection->LockCount >= 0) {
		// TODO: KeSetEvent(CriticalSection, 1, 0);
	}
}

XboxTypes::VOID XboxKernel::RtlLeaveCriticalSectionAndRegion(XboxTypes::PRTL_CRITICAL_SECTION CriticalSection) {
	XboxTypes::RTL_CRITICAL_SECTION *pCriticalSection = ToPointer<XboxTypes::RTL_CRITICAL_SECTION>(CriticalSection);

	pCriticalSection->RecursionCount--;
	if (pCriticalSection->RecursionCount != 0) {
		pCriticalSection->LockCount--;
		return;
	}

	pCriticalSection->OwningThread = NULL;
	pCriticalSection->LockCount--;
	if (pCriticalSection->LockCount >= 0) {
		// TODO: KeSetEvent(CriticalSection, 1, 0);
	}

	XboxTypes::KTHREAD *pCurrentThread = ToPointer<XboxTypes::KTHREAD>(m_pKPCR->PrcbData.CurrentThread);
	XboxTypes::KAPC_STATE *apcState = &pCurrentThread->ApcState;
	XboxTypes::LIST_ENTRY *apcListHead = &apcState->ApcListHead[XboxTypes::KernelMode];
	pCurrentThread->KernelApcDisable++;
	if (pCurrentThread->KernelApcDisable == 0 && apcListHead->Flink != _PTR_TO_ADDR(LIST_ENTRY, apcListHead)) {
		apcState->KernelApcPending = TRUE;
		// TODO: HalRequestSoftwareInterrupt(APC_LEVEL);
	}
}

XboxTypes::CHAR XboxKernel::RtlLowerChar(XboxTypes::CHAR Character) {
	return tolower(Character);
}

XboxTypes::VOID XboxKernel::RtlMapGenericMask(XboxTypes::PACCESS_MASK AccessMask, XboxTypes::PGENERIC_MAPPING GenericMapping) {
	XboxTypes::ACCESS_MASK *pAccessMask = ToPointer<XboxTypes::ACCESS_MASK>(AccessMask);
	XboxTypes::GENERIC_MAPPING *pGenericMapping = ToPointer<XboxTypes::GENERIC_MAPPING>(GenericMapping);

	if (*pAccessMask & GENERIC_READ) {
		*pAccessMask |= pGenericMapping->GenericRead;
	}
	if (*pAccessMask & GENERIC_WRITE) {
		*pAccessMask |= pGenericMapping->GenericWrite;
	}
	if (*pAccessMask & GENERIC_EXECUTE) {
		*pAccessMask |= pGenericMapping->GenericExecute;
	}
	if (*pAccessMask & GENERIC_ALL) {
		*pAccessMask |= pGenericMapping->GenericAll;
	}

	*pAccessMask &= ~(GENERIC_READ | GENERIC_WRITE | GENERIC_EXECUTE | GENERIC_ALL);
}

XboxTypes::VOID XboxKernel::RtlMoveMemory(XboxTypes::PVOID Destination, XboxTypes::PPVOID Source, XboxTypes::ULONG Length) {
	XboxTypes::VOID *pDestination = ToPointer<XboxTypes::VOID>(Destination);
	XboxTypes::VOID *pSource = ToPointer<XboxTypes::VOID>(Source);
	memmove(pDestination, pSource, Length);
}

XboxTypes::NTSTATUS XboxKernel::RtlMultiByteToUnicodeN(XboxTypes::PWSTR UnicodeString, XboxTypes::ULONG MaxBytesInUnicodeString, XboxTypes::PULONG BytesInUnicodeString, XboxTypes::PCHAR MultiByteString, XboxTypes::ULONG BytesInMultiByteString) {
	XboxTypes::WCHAR *pUnicodeString = ToPointer<XboxTypes::WCHAR>(UnicodeString);
	XboxTypes::ULONG *pBytesInUnicodeString = ToPointer<XboxTypes::ULONG>(BytesInUnicodeString);
	XboxTypes::CHAR *pMultiByteString = ToPointer<XboxTypes::CHAR>(MultiByteString);

	XboxTypes::ULONG maxUnicodeChars = MaxBytesInUnicodeString / sizeof(XboxTypes::WCHAR);
	XboxTypes::ULONG numChars = (maxUnicodeChars < BytesInMultiByteString) ? maxUnicodeChars : BytesInMultiByteString;

	if (BytesInUnicodeString != NULL) {
		*pBytesInUnicodeString = numChars * sizeof(XboxTypes::WCHAR);
	}

	while (numChars) {
		*pUnicodeString = (XboxTypes::WCHAR)(*pMultiByteString);

		pUnicodeString++;
		pMultiByteString++;
		numChars--;
	}

	return STATUS_SUCCESS;
}

XboxTypes::NTSTATUS XboxKernel::RtlMultiByteToUnicodeSize(XboxTypes::PULONG BytesInUnicodeString, XboxTypes::PCHAR MultiByteString, XboxTypes::ULONG BytesInMultiByteString) {
	XboxTypes::ULONG *pBytesInUnicodeString = ToPointer<XboxTypes::ULONG>(BytesInUnicodeString);
	
	*pBytesInUnicodeString = BytesInMultiByteString * sizeof(XboxTypes::WCHAR);

	return STATUS_SUCCESS;
}

XboxTypes::ULONG XboxKernel::RtlNtStatusToDosError(XboxTypes::NTSTATUS Status) {
	return NTSTATUStoDOSerror(Status);
}

XboxTypes::VOID XboxKernel::RtlRip(XboxTypes::PVOID ApiName, XboxTypes::PVOID Expression, XboxTypes::PVOID Message) {
	XboxTypes::CHAR *pApiName = ToPointer<XboxTypes::CHAR>(ApiName);
	XboxTypes::CHAR *pExpression = ToPointer<XboxTypes::CHAR>(Expression);
	XboxTypes::CHAR *pMessage = ToPointer<XboxTypes::CHAR>(Message);
	
	// Doesn't do anything at all?
	// Maybe only in debug build or devkits?
	
	log_spew("RtlRip:\n");
	log_spew("  ApiName    = %s\n", pApiName);
	log_spew("  Expression = %s\n", pExpression);
	log_spew("  Message    = %s\n", pMessage);
}

XboxTypes::ULONG XboxKernel::RtlTryEnterCriticalSection(XboxTypes::PRTL_CRITICAL_SECTION CriticalSection) {
	XboxTypes::RTL_CRITICAL_SECTION *pCriticalSection = ToPointer<XboxTypes::RTL_CRITICAL_SECTION>(CriticalSection);

	// NOTE: This is supposed to be an interlocked compare and exchange, but
	// since we're running outside of the emulated context in a single thread,
	// the code executed here is essentially atomic and therefore doesn't need
	// any kind of synchronization mechanism
	if (pCriticalSection->LockCount == -1) {
		pCriticalSection->LockCount = 0;
		pCriticalSection->OwningThread = m_pKPCR->PrcbData.CurrentThread;
		pCriticalSection->RecursionCount = 1;

		return TRUE;
	}

	if (pCriticalSection->OwningThread != m_pKPCR->PrcbData.CurrentThread) {
		return FALSE;
	}

	pCriticalSection->LockCount++;
	pCriticalSection->RecursionCount++;
	return TRUE;
}

XboxTypes::ULONG XboxKernel::RtlUlongByteSwap(XboxTypes::ULONG Source) {
	return (Source >> 24) | ((Source & 0xFF0000) >> 8) | ((Source & 0xFF00) << 8) | ((Source & 0xFF) << 24);
}

XboxTypes::NTSTATUS XboxKernel::RtlUnicodeToMultiByteN(XboxTypes::PCHAR MultiByteString, XboxTypes::ULONG MaxBytesInMultiByteString, XboxTypes::PULONG BytesInMultiByteString, XboxTypes::PWSTR UnicodeString, XboxTypes::ULONG BytesInUnicodeString) {
	XboxTypes::CHAR *pMultiByteString = ToPointer<XboxTypes::CHAR>(MultiByteString);
	XboxTypes::ULONG *pBytesInMultiByteString = ToPointer<XboxTypes::ULONG>(BytesInMultiByteString);
	XboxTypes::WCHAR *pUnicodeString = ToPointer<XboxTypes::WCHAR>(UnicodeString);

	XboxTypes::ULONG maxUnicodeChars = BytesInUnicodeString / sizeof(XboxTypes::WCHAR);
	XboxTypes::ULONG numChars = (maxUnicodeChars < MaxBytesInMultiByteString) ? maxUnicodeChars : MaxBytesInMultiByteString;

	if (BytesInMultiByteString != NULL) {
		*pBytesInMultiByteString = numChars;
	}

	while (numChars) {
		*pMultiByteString = (*pUnicodeString < 0xff) ? (XboxTypes::CHAR)(*pUnicodeString) : '?';

		pUnicodeString++;
		pMultiByteString++;
		numChars--;
	}

	return STATUS_SUCCESS;
}

XboxTypes::NTSTATUS XboxKernel::RtlUnicodeToMultiByteSize(XboxTypes::PULONG BytesInMultiByteString, XboxTypes::PWSTR UnicodeString, XboxTypes::ULONG BytesInUnicodeString) {
	XboxTypes::ULONG *pBytesInMultiByteString = ToPointer<XboxTypes::ULONG>(BytesInMultiByteString);

	*pBytesInMultiByteString = BytesInUnicodeString * sizeof(XboxTypes::WCHAR);

	return STATUS_SUCCESS;
}

XboxTypes::WCHAR XboxKernel::RtlUpcaseUnicodeChar(XboxTypes::WCHAR SourceCharacter) {
	return towupper(SourceCharacter);
}

XboxTypes::NTSTATUS XboxKernel::RtlUpcaseUnicodeString(XboxTypes::PUNICODE_STRING DestinationString, XboxTypes::PUNICODE_STRING SourceString, XboxTypes::BOOLEAN AllocateDestinationString) {
	XboxTypes::UNICODE_STRING *pDestinationString = ToPointer<XboxTypes::UNICODE_STRING>(DestinationString);
	XboxTypes::UNICODE_STRING *pSourceString = ToPointer<XboxTypes::UNICODE_STRING>(SourceString);

	if (AllocateDestinationString) {
		pDestinationString->MaximumLength = pSourceString->Length;
		pDestinationString->Buffer = ExAllocatePoolWithTag((XboxTypes::ULONG)pDestinationString->MaximumLength, 'grtS');
		if (pDestinationString->Buffer == NULL) {
			return STATUS_NO_MEMORY;
		}
	}
	else {
		if (pSourceString->Length > pDestinationString->MaximumLength) {
			return STATUS_BUFFER_OVERFLOW;
		}
	}

	XboxTypes::ULONG length = ((XboxTypes::ULONG)pSourceString->Length) / sizeof(XboxTypes::WCHAR);
	XboxTypes::WCHAR *pDst = ToPointer<XboxTypes::WCHAR>(pDestinationString->Buffer);
	XboxTypes::WCHAR *pSrc = ToPointer<XboxTypes::WCHAR>(pSourceString->Buffer);
	for (XboxTypes::ULONG i = 0; i < length; i++) {
		pDst[i] = (XboxTypes::WCHAR)towupper(pSrc[i]);
	}

	pDestinationString->Length = pSourceString->Length;

	return STATUS_SUCCESS;
}

XboxTypes::NTSTATUS XboxKernel::RtlUpcaseUnicodeToMultiByteN(XboxTypes::PCHAR MultiByteString, XboxTypes::ULONG MaxBytesInMultiByteString, XboxTypes::PULONG BytesInMultiByteString, XboxTypes::PWSTR UnicodeString, XboxTypes::ULONG BytesInUnicodeString) {
	XboxTypes::CHAR *pMultiByteString = ToPointer<XboxTypes::CHAR>(MultiByteString);
	XboxTypes::ULONG *pBytesInMultiByteString = ToPointer<XboxTypes::ULONG>(BytesInMultiByteString);
	XboxTypes::WCHAR *pUnicodeString = ToPointer<XboxTypes::WCHAR>(UnicodeString);

	XboxTypes::ULONG maxUnicodeChars = BytesInUnicodeString / sizeof(XboxTypes::WCHAR);
	XboxTypes::ULONG numChars = (maxUnicodeChars < MaxBytesInMultiByteString) ? maxUnicodeChars : MaxBytesInMultiByteString;

	if (BytesInMultiByteString != NULL) {
		*pBytesInMultiByteString = numChars;
	}

	while (numChars > 0) {
		XboxTypes::WCHAR unicodeChar = (*pUnicodeString < 256) ? *pUnicodeString : L'?';
		unicodeChar = towupper(unicodeChar);

		*pMultiByteString = (unicodeChar < 256) ? unicodeChar : '?';

		UnicodeString++;
		MultiByteString++;
		numChars--;
	}

	return STATUS_SUCCESS;
}

XboxTypes::CHAR XboxKernel::RtlUpperChar(XboxTypes::CHAR Character) {
	return toupper(Character);
}

XboxTypes::VOID XboxKernel::RtlUpperString(XboxTypes::PSTRING DestinationString, XboxTypes::PSTRING SourceString) {
	XboxTypes::STRING *pDestinationString = ToPointer<XboxTypes::STRING>(DestinationString);
	XboxTypes::STRING *pSourceString = ToPointer<XboxTypes::STRING>(SourceString);

	XboxTypes::CHAR *pDst = ToPointer<XboxTypes::CHAR>(pDestinationString->Buffer);
	XboxTypes::CHAR *pSrc = ToPointer<XboxTypes::CHAR>(pSourceString->Buffer);
	XboxTypes::ULONG length = pSourceString->Length;
	if ((XboxTypes::USHORT)length > pDestinationString->MaximumLength) {
		length = pDestinationString->MaximumLength;
	}
	pDestinationString->Length = (XboxTypes::USHORT)length;
	while (length > 0) {
		*pDst++ = RtlUpperChar(*pSrc++);
		length--;
	}
}

XboxTypes::USHORT XboxKernel::RtlUshortByteSwap(XboxTypes::USHORT Source) {
	return (Source >> 8) | ((Source & 0xFF) << 8);
}

XboxTypes::VOID XboxKernel::RtlZeroMemory(XboxTypes::PVOID Destination, XboxTypes::SIZE_T Length) {
	XboxTypes::VOID *pDestination = ToPointer<XboxTypes::VOID>(Destination);
	memset(pDestination, 0, Length);
}
