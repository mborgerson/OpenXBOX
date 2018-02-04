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
