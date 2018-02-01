#pragma once

#include <set>
#include <map>

#include "util.h"
#include "kernel/types.h"

#define IMAGE_BASE_ADDRESS  0x10000
#define PAGE_SIZE           KiB(4)
#define PAGE_SHIFT          12
#define PFN_DATABASE_SIZE   0x20000

#define BYTES_TO_PAGES(Size)  \
	(((Size) >> PAGE_SHIFT) + \
	(((Size) & (PAGE_SIZE - 1)) != 0))


class PhysicalMemoryManager;

class PhysicalMemoryBlock {
protected:
	uint32_t m_basePage;
	uint32_t m_numPages;
	uint32_t m_actualSize;
	PhysicalMemoryManager *m_memmgr;
public:
	PhysicalMemoryBlock(uint32_t basePage, uint32_t numPages, uint32_t actualSize, PhysicalMemoryManager *memmgr);
	~PhysicalMemoryBlock();
	
	uint32_t BasePage() const { return m_basePage; }
	uint32_t NumPages() const { return m_numPages; }
	uint32_t LastPage() const { return m_basePage + m_numPages - 1; }
	uint32_t TotalSize() const { return m_numPages * PAGE_SIZE; }

	uint32_t BaseAddress() const { return m_basePage << PAGE_SHIFT; }
	uint32_t Size() const { return m_actualSize; }
};


class PhysicalMemoryManager {
protected:
	uint32_t m_mainMemorySize;
	uint32_t m_usableMemorySize;

	// Allocated memory blocks
	std::set<PhysicalMemoryBlock *> m_blocks;

	// Maps base pages to allocated blocks
	std::map<uint32_t, PhysicalMemoryBlock *> m_pageToBlock;

	// Page allocation bitmap
	// TODO: Use a bit set
	// TODO: Keep track of protection masks and persistence state per page
	// - Protection masks can be set by MmAllocateContiguousMemoryEx.
	// - Persistence is used by MmPersistContiguousMemory. Pages marked as
	//   persistent survive a system reboot.
	bool *m_allocatedPages;

	// TODO: keep track of statistics for MmQueryStatistics

	/*!
	 * Registers a memory block on the list.
	 */
	void RegisterBlock(PhysicalMemoryBlock *block, uint32_t protect) {
		m_blocks.emplace(block);
		m_pageToBlock[block->BasePage()] = block;
		for (uint32_t page = block->BasePage(); page <= block->LastPage(); page++) {
			m_allocatedPages[page] = true;
			// TODO: set page protection mask
		}
		// TODO: update memory protection state on the CPU
	}

	/*!
	 * Determines if the given region is unallocated.
	 */
	bool IsRegionUnallocated(uint32_t basePage, uint32_t numPages);
public:
	PhysicalMemoryManager(uint32_t mainMemorySize);
	~PhysicalMemoryManager();

	/*!
	 * Allocates a contiguous block of memory of the requested size and
	 * alignment within the specified range.
	 * If the memory cannot be allocated, the function returns nullptr.
	 */
	PhysicalMemoryBlock *AllocateContiguous(
		uint32_t size,
		uint32_t minAcceptableAddress = IMAGE_BASE_ADDRESS,
		uint32_t maxAcceptableAddress = UINT32_MAX,
		uint32_t align = PAGE_SIZE,
		uint32_t protect = PAGE_READWRITE
	);

	/*!
	 * Reserves the specified memory range, disallowing automatic allocation of
	 * pages within it.
	 */
	PhysicalMemoryBlock *Reserve(uint32_t baseAddress, uint32_t size, uint32_t protect);
	
	/*!
	 * Frees the memory allocated at the given address.
	 * Returns true if the deallocation was successful, which only happens if
	 * there was a block of memory allocated at the specified base address.
	 */
	bool FreeContiguous(uint32_t baseAddress);

	/*!
	 * Retrieves the size of the allocated contiguous memory at the specified
	 * base address, if available. Otherwise, returns zero.
	 */
	uint32_t QueryAllocationSize(uint32_t baseAddress);

	/*!
	 * Defines the new protection mask of the pages at the specified address
	 * range.
	 */
	void SetProtect(uint32_t baseAddress, uint32_t size, uint32_t protect);

	/*!
	 * Retrieves the protection mask of the page containing the memory address.
	 */
	uint32_t QueryProtect(uint32_t address);

	/*!
	* Enables or disables persistence of a memory region.
	* Persistent memory survives a system reboot.
	*/
	void SetPersist(uint32_t baseAddress, uint32_t size, bool persist);
};
