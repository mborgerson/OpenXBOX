#include "kernel/impl/kernel.h"
#include "log.h"

XboxTypes::ULONG XboxKernel::HalGetInterruptVector(XboxTypes::ULONG BusInterruptLevel, XboxTypes::PKIRQL Irql) {
	XboxTypes::ULONG vec = BusInterruptLevel + 0x30;
	if (vec < 0x30 || vec > 0x4a) {
		return 0;
	}

	XboxTypes::KIRQL irql = (XboxTypes::KIRQL)(0x4a - vec);
	m_cpu->MemWrite(Irql, sizeof(XboxTypes::KIRQL), &irql);

	return vec;
}

XboxTypes::VOID XboxKernel::HalReturnToFirmware(XboxTypes::FIRMWARE_REENTRY Routine) {
	switch (Routine) {
	case XboxTypes::HalHaltRoutine:
		log_info("HalReturnToFirmware: Halt system\n");
		break;
	case XboxTypes::HalRebootRoutine:
		log_info("HalReturnToFirmware: Reboot\n");
		break;
	case XboxTypes::HalQuickRebootRoutine:
		log_info("HalReturnToFirmware: Quick reboot\n");
		break;
	case XboxTypes::HalKdRebootRoutine:
		log_info("HalReturnToFirmware: Reboot to kernel debugger\n");
		break;
	case XboxTypes::HalFatalErrorRebootRoutine:
		log_info("HalReturnToFirmware: Reboot due to fatal error\n");
		break;
	default:
		log_info("HalReturnToFirmware: Unknown routine: %d\n", Routine);
		break;
	}
}
