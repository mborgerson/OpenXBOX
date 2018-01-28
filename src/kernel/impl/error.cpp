#include <stdint.h>
#include "kernel/impl/error.h"

uint32_t NTSTATUStoDOSerror(XboxTypes::NTSTATUS status) {
	if (status & 0x20000000) {
		return status;
	}
	else if ((status & 0xffff0000) == 0x80070000) {
		return(status & 0x0000ffff);
	}
	else if ((status & 0xf0000000) == 0xd0000000) {
		status &= 0xcfffffff;
	}

	uint32_t entry = 0;
	uint32_t index = 0;
	do {
		if (status >= RtlpRunTable[entry + 1].BaseCode) {
			index += (RtlpRunTable[entry].RunLength * RtlpRunTable[entry].CodeSize);
		}
		else {
			uint32_t offset = status - RtlpRunTable[entry].BaseCode;
			if (offset >= RtlpRunTable[entry].RunLength) {
				break;
			}

			index += (offset * RtlpRunTable[entry].CodeSize);
			if (RtlpRunTable[entry].CodeSize == 1) {
				return RtlpStatusTable[index];
			}
			else {
				return ((RtlpStatusTable[index + 1] << 16) | RtlpStatusTable[index]);
			}
		}

		entry++;
	} while (entry < (sizeof(RtlpRunTable) / sizeof(RUN_ENTRY)));

	if ((status >> 16) == 0xc001) {
		return status & 0xffff;
	}

	return ERROR_MR_MID_NOT_FOUND;
}