#pragma once

#include <stdio.h>
#include <xbox.h>

// Kernel function execution results
#define KF_OK                     0  // The function is fully implemented
#define KF_ERROR_NOT_IMPLEMENTED -1  // The function is not implemented
#define KF_WARN_PARTIAL_IMPL     -2  // The function has a partial implementation
#define KF_WARN_FAKE_IMPL        -3  // The function has a fake implementation

#define K_ENTER_CDECL() \
    CdeclHelper cc(m_cpu);

#define K_ENTER_FASTCALL() \
    FastcallHelper cc(m_cpu);

#define K_ENTER_STDCALL() \
    StdcallHelper cc(m_cpu);

// Get the argument from the stack frame according to the calling convention
#define K_INIT_ARG(TYPE, NAME) \
	XboxTypes::TYPE NAME; \
	cc.GetArgument(&NAME, sizeof(XboxTypes::TYPE));

// Return from the kernel function
#define K_EXIT() \
    do { \
        m_cpu->RegWrite(REG_EIP, cc.GetReturnAddress()); \
        cc.Cleanup(); \
    } while (0)

// Return from the kernel function with a value
#define K_EXIT_WITH_VALUE(VAL) \
    do { \
        cc.SetReturnValue(&VAL, sizeof(VAL)); \
        K_EXIT(); \
    } while (0)
