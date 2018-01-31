#include "kernel/impl/kernel.h"

XboxTypes::NTSTATUS XboxKernel::ExQueryNonVolatileSetting(XboxTypes::ULONG ValueIndex, XboxTypes::PULONG Type, XboxTypes::PVOID Value, XboxTypes::ULONG ValueLength, XboxTypes::PULONG ResultLength) {
	// TODO: read from EEPROM
	return STATUS_SUCCESS;
}
