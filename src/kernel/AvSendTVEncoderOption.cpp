#include "common.h"

/*
 * AvSendTVEncoderOption
 *
 * Import Number:      2
 * Calling Convention: stdcall
 * Parameter 0:        PVOID  RegisterBase
 * Parameter 1:        ULONG  Option
 * Parameter 2:        ULONG  Param
 * Parameter 3:        PULONG Result
 * Return Type:        VOID
 */
int Xbox::AvSendTVEncoderOption()
{
	K_ENTER_STDCALL();
	K_INIT_ARG_PTR(VOID,  RegisterBase);
	K_INIT_ARG_VAL(ULONG, Option);
	K_INIT_ARG_VAL(ULONG, Param);
	K_INIT_ARG_PTR(ULONG, Result);

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
	} else {
		// Do nothing
	}

	K_EXIT();
	return KF_WARN_PARTIAL_IMPL;
}
