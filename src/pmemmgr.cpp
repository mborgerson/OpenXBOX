#include "pmemmgr.h"

PhysicalMemoryBlock::PhysicalMemoryBlock(uint32_t basePage, uint32_t numPages, uint32_t actualSize, PhysicalMemoryManager *memmgr)
	: m_basePage(basePage)
	, m_numPages(numPages)
	, m_actualSize(actualSize)
	, m_memmgr(memmgr)
{
}

bool PhysicalMemoryBlock::Free() {
	return m_memmgr->FreeContiguous(BaseAddress());
}


PhysicalMemoryManager::PhysicalMemoryManager(uint32_t mainMemorySize)
	: m_mainMemorySize(mainMemorySize)
{
	m_usableMemorySize = mainMemorySize - PFN_DATABASE_SIZE;
	uint32_t size = m_mainMemorySize >> PAGE_SHIFT;
	m_allocatedPages = new bool[size];
	memset(m_allocatedPages, 0, size);
}

PhysicalMemoryManager::~PhysicalMemoryManager()
{
	for (auto it = m_blocks.begin(); it != m_blocks.end(); it++) {
		delete (*it);
	}
	delete[] m_allocatedPages;
}

PhysicalMemoryBlock *PhysicalMemoryManager::AllocateContiguous(uint32_t size, uint32_t minAcceptableAddress, uint32_t maxAcceptableAddress, uint32_t align, uint32_t protect)
{
	// Cannot allocate zero length
	if (size == 0) {
		return nullptr;
	}

	// Alignment must be a power of two
	if ((align & (align - 1)) != 0) {
		return nullptr;
	}

	// Alignment must be at least the size of a memory page
	if (align < PAGE_SIZE) {
		align = PAGE_SIZE;
	}
	
	// Calculate number of pages to allocate
	uint32_t numPages = BYTES_TO_PAGES(size);

	// Convert addresses to pages
	uint32_t pageAlign = align >> PAGE_SHIFT;
	if (pageAlign == 0) {
		pageAlign = 1;
	}
	uint32_t pageAlignMask = ~(pageAlign - 1);

	// Don't allocate memory below the executable image base address
	if (minAcceptableAddress < IMAGE_BASE_ADDRESS) {
		minAcceptableAddress = IMAGE_BASE_ADDRESS;
	}

	uint32_t minAcceptablePageNumber = minAcceptableAddress >> PAGE_SHIFT;
	uint32_t maxAcceptablePageNumber = maxAcceptableAddress >> PAGE_SHIFT;

	// Make sure the requested range is valid
	uint32_t mainMemoryPages = (m_usableMemorySize >> PAGE_SHIFT);
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
			PhysicalMemoryBlock *block = new PhysicalMemoryBlock(page, numPages, size, this);
			RegisterBlock(block, protect);
			return block;
		}
	}

	// Cannot fit
	return nullptr;
}

PhysicalMemoryBlock *PhysicalMemoryManager::Reserve(uint32_t baseAddress, uint32_t size, uint32_t protect) {
	// Convert to pages
	int basePage = BYTES_TO_PAGES(baseAddress);
	int numPages = BYTES_TO_PAGES(size);

	// Reserve block
	if (IsRegionUnallocated(basePage, numPages)) {
		PhysicalMemoryBlock *block = new PhysicalMemoryBlock(basePage, numPages, size, this);
		RegisterBlock(block, protect);
		return block;
	}

	// Region is already reserved
	return nullptr;
}

bool PhysicalMemoryManager::FreeContiguous(uint32_t baseAddress) {
	uint32_t page = BYTES_TO_PAGES(baseAddress);
	if (m_pageToBlock.count(page)) {
		PhysicalMemoryBlock *block = m_pageToBlock[page];
		m_pageToBlock.erase(page);
		m_blocks.erase(block);
		for (uint32_t page = block->BasePage(); page <= block->LastPage(); page++) {
			m_allocatedPages[page] = false;
		}
		return true;
	}
	return false;
}

uint32_t PhysicalMemoryManager::QueryAllocationSize(uint32_t baseAddress) {
	uint32_t page = BYTES_TO_PAGES(baseAddress);
	if (m_pageToBlock.count(page)) {
		return m_pageToBlock[page]->NumPages() << PAGE_SHIFT;
	}
	return 0;
}

void PhysicalMemoryManager::SetProtect(uint32_t baseAddress, uint32_t size, uint32_t protect) {
	// TODO: set protection mask and update CPU
}

uint32_t PhysicalMemoryManager::QueryProtect(uint32_t address) {
	// TODO: implement
	return PAGE_READWRITE;
}

void PhysicalMemoryManager::SetPersist(uint32_t baseAddress, uint32_t size, bool persist) {
	// TODO: implement
}

bool PhysicalMemoryManager::IsRegionUnallocated(uint32_t basePage, uint32_t numPages) {
	for (uint32_t page = basePage; page < basePage + numPages; page++) {
		if (m_allocatedPages[page]) {
			return false;
		}
	}
	return true;
}
