#pragma once

#include "kernel/impl/kernel.h"

#define KiLockDispatcherDatabase(oldIRQL) \
	*(oldIRQL) = KeRaiseIrqlToDpcLevel();


const XboxTypes::UCHAR KiFindLeftNibbleBitTable[] = { 0, 0, 1, 1, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 3, 3 };

#define ClearMember(Member, Set) \
    Set = Set & (~(1 << (Member)))

#define SetMember(Member, Set) \
    Set = Set | (1 << (Member))

#define FindFirstSetLeftMember(Set, Member) { \
    XboxTypes::ULONG _Bit; \
    XboxTypes::ULONG _Mask; \
    XboxTypes::ULONG _Offset = 16; \
    if ((_Mask = Set >> 16) == 0) { \
        _Offset = 0; \
        _Mask = Set; \
    } \
    if (_Mask >> 8) { \
        _Offset += 8; \
    } \
    if ((_Bit = Set >> _Offset) & 0xf0) { \
        _Bit >>= 4; \
        _Offset += 4; \
    } \
    *(Member) = KiFindLeftNibbleBitTable[_Bit] + _Offset; \
}
