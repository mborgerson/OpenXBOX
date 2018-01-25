#include "memmgr.h"

ContiguousMemoryBlock::ContiguousMemoryBlock(uint32_t basePage, uint32_t numPages)
	: m_basePage(basePage)
	, m_numPages(numPages)
{
}

// TODO: paged vs. non-paged pools
// TODO: thread safety
// take some pointers from Cxbx-Reloaded's VMManager
// https://github.com/Cxbx-Reloaded/Cxbx-Reloaded/blob/master/src/CxbxKrnl/VMManager.cpp


// NOTE: 0x20000 bytes removed from the main memory because the topmost region
// is reserved for something. Thread stacks perhaps?
MemoryManager::MemoryManager(uint32_t mainMemorySize)
	: m_mainMemorySize(mainMemorySize - 0x20000)
{
	uint32_t size = m_mainMemorySize >> PAGE_ALIGN_SHIFT;
	m_allocatedPages = new bool[size];
	memset(m_allocatedPages, 0, size);
}

MemoryManager::~MemoryManager()
{
	for (auto it = m_blocks.begin(); it != m_blocks.end(); it++) {
		delete (*it);
	}
	delete[] m_allocatedPages;
}

ContiguousMemoryBlock *MemoryManager::AllocateContiguous(uint32_t size, uint32_t minAcceptableAddress, uint32_t maxAcceptableAddress, uint32_t align, uint32_t protect)
{
	// Cannot allocate zero length
	if (size == 0) {
		return nullptr;
	}

	// Alignment must be a power of two
	if ((align & (align - 1)) != 0) {
		return nullptr;
	}

	// Cannot allocate less than the minimum size of an Xbox memory page
	if (align < XBOX_PAGE_SIZE) {
		align = XBOX_PAGE_SIZE;
	}
	
	// Calculate number of pages to allocate
	uint32_t numPages = BYTES_TO_PAGES(size);

	// Convert addresses to pages
	uint32_t pageAlign = align >> PAGE_ALIGN_SHIFT;
	if (pageAlign == 0) {
		pageAlign = 1;
	}
	uint32_t pageAlignMask = ~(pageAlign - 1);

	if (minAcceptableAddress < MIN_ADDRESS) {
		minAcceptableAddress = MIN_ADDRESS;
	}

	uint32_t minAcceptablePageNumber = minAcceptableAddress >> PAGE_ALIGN_SHIFT;
	uint32_t maxAcceptablePageNumber = maxAcceptableAddress >> PAGE_ALIGN_SHIFT;

	// Make sure the requested range is valid
	// NOTE: It seems that MmAllocateContiguousMemoryEx does not validate these
	// parameters but instead readjusts them to fit in the main memory range.
	uint32_t mainMemoryPages = (m_mainMemorySize >> PAGE_ALIGN_SHIFT);
	/*if (minAcceptablePageNumber > maxAcceptablePageNumber) {
		return nullptr;
	}
	if (minAcceptablePageNumber >= mainMemoryPages || maxAcceptablePageNumber >= mainMemoryPages) {
		return nullptr;
	}*/
	if (maxAcceptablePageNumber > mainMemoryPages) {
		maxAcceptablePageNumber = mainMemoryPages;
	}
	if (minAcceptablePageNumber > maxAcceptablePageNumber) {
		minAcceptablePageNumber = maxAcceptablePageNumber;
	}

	// Locate a free contiguous region of memory within the specified range
	// (in reverse, because of page alignment restrictions)
	for (uint32_t page = maxAcceptablePageNumber & pageAlignMask; page >= minAcceptablePageNumber; page -= pageAlign) {
		if (IsRegionUnallocated(page, numPages)) {
			ContiguousMemoryBlock *block = new ContiguousMemoryBlock(page, numPages);
			RegisterBlock(block, protect);
			return block;
		}
	}

	// Cannot fit
	return nullptr;
}

bool MemoryManager::FreeContiguous(uint32_t baseAddress)
{
	// TODO: what if the address is unaligned?
	uint32_t page = BYTES_TO_PAGES(baseAddress);
	if (m_pageToBlock.count(page)) {
		ContiguousMemoryBlock *block = m_pageToBlock[page];
		m_pageToBlock.erase(page);
		m_blocks.erase(block);
		for (uint32_t page = block->BasePage(); page <= block->LastPage(); page++) {
			m_allocatedPages[page] = false;
		}
		delete block;
		return true;
	}
	return false;
}

uint32_t MemoryManager::QueryAllocationSize(uint32_t baseAddress) {
	// TODO: what if the address is unaligned?
	uint32_t page = BYTES_TO_PAGES(baseAddress);
	if (m_pageToBlock.count(page)) {
		return m_pageToBlock[page]->Size();
	}
	return 0;
}

void MemoryManager::SetProtect(uint32_t baseAddress, uint32_t size, uint32_t protect) {
	// TODO: set protection mask and update CPU
}

uint32_t MemoryManager::QueryProtect(uint32_t address) {
	// TODO: implement
	return 0;
}

void MemoryManager::SetPersist(uint32_t baseAddress, uint32_t size, bool persist) {
	// TODO: implement
}

bool MemoryManager::IsRegionUnallocated(uint32_t basePage, uint32_t numPages) {
	for (uint32_t page = basePage; page < basePage + numPages; page++) {
		if (m_allocatedPages[page]) {
			return false;
		}
	}
	return true;
}
