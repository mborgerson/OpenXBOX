#include "kernel/impl/kernel.h"

XboxKernel::XboxKernel(char *ram, size_t ramSize)
	: m_ram(ram)
	, m_ramSize(ramSize)
{
}
