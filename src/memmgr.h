#ifndef MEMMGR_H
#define MEMMGR_H

#include <set>
#include <map>

#include "util.h"

#define MIN_ADDRESS       MiB(16)
#define XBOX_PAGE_SIZE    KiB(4)
#define PAGE_ALIGN_SHIFT  12

#define BYTES_TO_PAGES(Size)  \
	(((Size) >> PAGE_ALIGN_SHIFT) + \
	(((Size) & (XBOX_PAGE_SIZE - 1)) != 0))

class ContiguousMemoryBlock {
protected:
	uint32_t m_basePage;
	uint32_t m_numPages;
public:
	ContiguousMemoryBlock(uint32_t baseAddress, uint32_t size);
	
	uint32_t BasePage() const { return m_basePage; }
	uint32_t NumPages() const { return m_numPages; }
	uint32_t LastPage() const { return m_basePage + m_numPages - 1; }

	uint32_t BaseAddress() const { return m_basePage << PAGE_ALIGN_SHIFT; }
	uint32_t Size() const { return m_numPages << PAGE_ALIGN_SHIFT; }
};


class MemoryManager {
protected:
	uint32_t m_mainMemorySize;

	// Allocated memory blocks
	std::set<ContiguousMemoryBlock *> m_blocks;

	// Maps base pages to allocated blocks
	std::map<uint32_t, ContiguousMemoryBlock *> m_pageToBlock;

	// Page allocation bitmap
	// TODO: Use a bit set
	// TODO: Keep track of protection masks and persistence state per page
	// - Protection masks can be set by MmAllocateContiguousMemoryEx.
	// - Persistence is used by MmPersistContiguousMemory. Pages marked as
	//   persistent survive a system reboot.
	bool *m_allocatedPages;

	/*!
	 * Registers a memory block on the list.
	 */
	void RegisterBlock(ContiguousMemoryBlock *block) {
		m_blocks.emplace(block);
		m_pageToBlock[block->BasePage()] = block;
		for (uint32_t page = block->BasePage(); page <= block->LastPage(); page++) {
			m_allocatedPages[page] = true;
		}
	}

	/*!
	 * Determines if the given region is unallocated.
	 */
	bool IsRegionUnallocated(uint32_t basePage, uint32_t numPages);
public:
	MemoryManager(uint32_t mainMemorySize);
	~MemoryManager();

	/*!
	 * Allocates a contiguous block of memory of the requested size and
	 * alignment within the specified range.
	 * If the memory cannot be allocated, the function returns nullptr.
	 */
	// TODO: The Protect parameter from the MmAllocateContiguousMemory
	// functions is currently being ignored.
	ContiguousMemoryBlock *AllocateContiguous(uint32_t size, uint32_t minAcceptableAddress = 0, uint32_t maxAcceptableAddress = UINT32_MAX, uint32_t align = XBOX_PAGE_SIZE);

	/*!
	 * Frees the memory allocated at the given address.
	 * Returns true if the deallocation was successful, which only happens if
	 * there was a block of memory at the specified base address.
	 */
	bool Free(uint32_t baseAddress);
};

#endif
