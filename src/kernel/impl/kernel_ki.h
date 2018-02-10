#pragma once

#include "kernel/impl/kernel.h"

#define KiLockDispatcherDatabase(oldIRQL) \
	*(oldIRQL) = KeRaiseIrqlToDpcLevel();
