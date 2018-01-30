#pragma once

#include <stdint.h>
#include "kernel/types.h"

/*!
 * Represents an address (pointer) in the emulated Xbox environment.
 * Only the lowest 32 bits are used.
 */
typedef uint64_t XboxMemoryAddress;

/*!
 * Indicates a pointer that does not point to Xbox RAM.
 */
const XboxMemoryAddress XMA_InvalidPointer = -1UL;


/*!
 * A custom implementation of the Xbox kernel that runs on the host machine and
 * manages kernel objects in the virtual Xbox machine.
 *
 * Just like a real kernel, this class handles the low-level mechanisms between
 * applications and the hardware, except that it runs outside of the emulated
 * environment. The goal is to respond to kernel function invocations and
 * manage the Xbox hardware so that the emulated applications believe they're
 * running on a real kernel.
 */
class XboxKernel {
public:
	XboxKernel(char *ram, size_t ramSize);

private:
	char *m_ram;
	size_t m_ramSize;

	/*!
	 * Converts an address into a pointer to data in Xbox memory, allowing
	 * direct manipulation of data. Returns nullptr if the address is outside
	 * the range of the RAM.
	 */
	template<class T> T* ToPointer(XboxMemoryAddress address) {
		return (address < m_ramSize)
			? (T*)m_ram[address]
			: nullptr;
	}

	/*!
	 * Converts a a pointer to data in Xbox memory into an address. If the
	 * pointer does not point to Xbox RAM, returns XMA_InvalidPointer.
	 */
	template<class T> XboxMemoryAddress ToAddress(T* pointer) {
		char *p = (char *)pointer;
		return (p >= m_ram && p < m_ram + m_ramSize)
			? (XboxMemoryAddress)((char *)pointer - m_ram);
			: XMA_InvalidPointer;
	}
};
