#include "kernel/impl/kernel.h"
#include "log.h"

XboxTypes::PVOID XboxKernel::AvGetSavedDataAddress() {
	return m_AvSavedDataAddress;
}

XboxTypes::VOID XboxKernel::AvSendTVEncoderOption(XboxTypes::PVOID RegisterBase, XboxTypes::ULONG Option, XboxTypes::ULONG Param, XboxTypes::PULONG Result) {
	log_debug("%s(%08x, %x, %x, %08x)\n",
		__func__,
		RegisterBase,
		Option,
		Param,
		Result);

	// Defines for video encoder options
#define VIDEO_ENC_GET_SETTINGS      6
#define VIDEO_ENC_VIDEOENABLE       9
#define VIDEO_ENC_FLICKERFILTER     11
#define VIDEO_ENC_SOFTEN_FILTER     14

	if (Option == VIDEO_ENC_GET_SETTINGS) {
		// 640,480,60,VIDEO_REGION_NTSCM,AV_PACK_STANDARD}, //640x480 NTSCM 60Hz
		log_debug("-- VIDEO_ENC_GET_SETTINGS\n");
		XboxTypes::DWORD value = 0x00400101;
		m_cpu->MemWrite(Result, sizeof(XboxTypes::DWORD), &value);
	}
	else {
		// Do nothing
	}
}

XboxTypes::ULONG XboxKernel::AvSetDisplayMode(XboxTypes::PVOID RegisterBase, XboxTypes::ULONG Step, XboxTypes::ULONG DisplayMode, XboxTypes::ULONG SourceColorFormat, XboxTypes::ULONG Pitch, XboxTypes::ULONG FrameBuffer) {
	log_debug("%s(%08x, %x, %x, %x, %x, %x)\n",
		__func__,
		RegisterBase,
		Step,
		DisplayMode,
		SourceColorFormat,
		Pitch,
		FrameBuffer);

	return 0;
}

XboxTypes::VOID XboxKernel::AvSetSavedDataAddress(XboxTypes::PVOID Address) {
	m_AvSavedDataAddress = Address;
}
