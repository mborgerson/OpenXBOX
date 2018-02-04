#include "kernel/impl/kernel.h"

// The Interlocked* functions can be implemented without regard for actual
// locking mechanisms because, as far as the emulated CPU is concerned, the
// entire operations are atomic.

XboxTypes::LONG XboxKernel::InterlockedCompareExchange(XboxTypes::PLONG Destination, XboxTypes::LONG ExChange, XboxTypes::LONG Comparand) {
	XboxTypes::LONG *pDestination = ToPointer<XboxTypes::LONG>(Destination);

	XboxTypes::LONG oldValue = *pDestination;
	if (*pDestination == Comparand) {
		*pDestination = ExChange;
	}

	return oldValue;
}

XboxTypes::LONG XboxKernel::InterlockedDecrement(XboxTypes::PLONG Addend) {
	XboxTypes::LONG *pAddend = ToPointer<XboxTypes::LONG>(Addend);

	*pAddend--;
	return *pAddend;
}

XboxTypes::LONG XboxKernel::InterlockedExchange(XboxTypes::PLONG Target, XboxTypes::LONG Value) {
	XboxTypes::LONG *pTarget = ToPointer<XboxTypes::LONG>(Target);

	XboxTypes::LONG oldValue = *pTarget;
	*pTarget = Value;
	return oldValue;
}

XboxTypes::LONG XboxKernel::InterlockedExchangeAdd(XboxTypes::PLONG Addend, XboxTypes::LONG Increment) {
	XboxTypes::LONG *pAddend = ToPointer<XboxTypes::LONG>(Addend);

	XboxTypes::LONG oldValue = *pAddend;
	*pAddend += Increment;
	return oldValue;
}

XboxTypes::LONG XboxKernel::InterlockedIncrement(XboxTypes::PLONG Addend) {
	XboxTypes::LONG *pAddend = ToPointer<XboxTypes::LONG>(Addend);

	*pAddend++;
	return *pAddend;
}

XboxTypes::PSINGLE_LIST_ENTRY XboxKernel::InterlockedFlushSList(XboxTypes::PSLIST_HEADER ListHead) {
	XboxTypes::SLIST_HEADER *pListHead = ToPointer<XboxTypes::SLIST_HEADER>(ListHead);
	XboxTypes::PSINGLE_LIST_ENTRY currList = _PTR_TO_ADDR(SINGLE_LIST_ENTRY, &pListHead->Next);

	if (pListHead->Next.Next != NULL) {
		pListHead->Next.Next = NULL;
		pListHead->Depth = 0;
		pListHead->Sequence = 0;
	}
	
	return currList;
}

XboxTypes::PSINGLE_LIST_ENTRY XboxKernel::InterlockedPopEntrySList(XboxTypes::PSLIST_HEADER ListHead) {
	XboxTypes::SLIST_HEADER *pListHead = ToPointer<XboxTypes::SLIST_HEADER>(ListHead);
	XboxTypes::PSINGLE_LIST_ENTRY currNext = pListHead->Next.Next;
	XboxTypes::SINGLE_LIST_ENTRY *pCurrNext = ToPointer<XboxTypes::SINGLE_LIST_ENTRY>(currNext);

	if (pListHead->Next.Next != NULL) {
		pListHead->Next = *pCurrNext;
		XboxTypes::ULONG *val = (XboxTypes::ULONG *)(&pListHead->Depth);
		val += 0xffff;
	}
}

XboxTypes::PSINGLE_LIST_ENTRY XboxKernel::InterlockedPushEntrySList(XboxTypes::PSLIST_HEADER ListHead, XboxTypes::PSINGLE_LIST_ENTRY ListEntry) {
	XboxTypes::SLIST_HEADER *pListHead = ToPointer<XboxTypes::SLIST_HEADER>(ListHead);
	XboxTypes::SINGLE_LIST_ENTRY *pListEntry = ToPointer<XboxTypes::SINGLE_LIST_ENTRY>(ListEntry);
	XboxTypes::PSINGLE_LIST_ENTRY currNext = pListHead->Next.Next;

	pListEntry->Next = currNext;
	pListHead->Next = *pListEntry;
	pListHead->Depth++;
	pListHead->Sequence++;

	return currNext;
}
