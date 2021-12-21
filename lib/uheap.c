#include <inc/lib.h>

// malloc()
//	This function use FIRST FIT strategy to allocate space in heap
//  with the given size and return void pointer to the start of the allocated space

//	To do this, we need to switch to the kernel, allocate the required space
//	in Page File then switch back to the user again.
//
//	We can use sys_allocateMem(uint32 virtual_address, uint32 size); which
//		switches to the kernel mode, calls allocateMem(struct Env* e, uint32 virtual_address, uint32 size) in
//		"memory_manager.c", then switch back to the user mode here
//	the allocateMem function is empty, make sure to implement it.

//==================================================================================//
//============================ REQUIRED FUNCTIONS ==================================//
//==================================================================================//
struct Block
{
	uint32 startAddress, endAddress;
	struct Block *ptrNext, *ptrPrev;
};

struct Block head = {USER_HEAP_START, USER_HEAP_MAX, NULL, NULL};
struct Block *freeBlockListHead = &head;

void* malloc(uint32 size)
{
	//TODO: [PROJECT 2021 - [2] User Heap] malloc() [User Side]
	// Write your code here, remove the panic and write your code
	// panic("malloc() is not implemented yet...!!");

	//This function should find the space of the required range
	//using the BEST FIT strategy

	//refer to the project presentation and documentation for details

	uint32 minSize = USER_HEAP_MAX - USER_HEAP_START + 5;
	struct Block *bestFitBlock = NULL;
	struct Block *block = freeBlockListHead;

	while (block != NULL)
	{
		uint32 blockSize = block->endAddress - block->startAddress + 1;
		if (blockSize >= size && blockSize < minSize)
		{
			minSize = blockSize;
			bestFitBlock = block;
		}
		block = block->ptrNext;
	}

	if (bestFitBlock == NULL)
		return NULL;

	sys_allocateMem(bestFitBlock->startAddress, size);
	uint32 bestFitBlockSize = bestFitBlock->endAddress - bestFitBlock->startAddress + 1;
	uint32 retAddress = bestFitBlock->startAddress;
	uint32 numOfPages = (size + 4095) / 4096;
	uint32 pagesSize = numOfPages * PAGE_SIZE;

	if (bestFitBlockSize == size)
	{
		if (bestFitBlock->ptrNext != NULL)
		{
			bestFitBlock->ptrNext->ptrPrev = bestFitBlock->ptrPrev;
		}

		if (bestFitBlock->ptrPrev != NULL)
		{
			bestFitBlock->ptrPrev->ptrNext = bestFitBlock->ptrNext;
		}
		else
		{
			freeBlockListHead = bestFitBlock->ptrNext;
		}
	}
	else
	{
		bestFitBlock->startAddress += pagesSize;
	}

	return (void*) retAddress;
}

// free():
//	This function frees the allocation of the given virtual_address
//	To do this, we need to switch to the kernel, free the pages AND "EMPTY" PAGE TABLES
//	from page file and main memory then switch back to the user again.
//
//	We can use sys_freeMem(uint32 virtual_address, uint32 size); which
//		switches to the kernel mode, calls freeMem(struct Env* e, uint32 virtual_address, uint32 size) in
//		"memory_manager.c", then switch back to the user mode here
//	the freeMem function is empty, make sure to implement it.

void free(void* virtual_address)
{
	//TODO: [PROJECT 2021 - [2] User Heap] free() [User Side]
	// Write your code here, remove the panic and write your code
	panic("free() is not implemented yet...!!");

	//you should get the size of the given allocation using its address

	//refer to the project presentation and documentation for details
}

//==================================================================================//
//================================ OTHER FUNCTIONS =================================//
//==================================================================================//

void* smalloc(char *sharedVarName, uint32 size, uint8 isWritable)
{
	panic("this function is not required...!!");
	return 0;
}

void* sget(int32 ownerEnvID, char *sharedVarName)
{
	panic("this function is not required...!!");
	return 0;
}

void sfree(void* virtual_address)
{
	panic("this function is not required...!!");
}

void *realloc(void *virtual_address, uint32 new_size)
{
	panic("this function is not required...!!");
	return 0;
}

void expand(uint32 newSize)
{
	panic("this function is not required...!!");
}
void shrink(uint32 newSize)
{
	panic("this function is not required...!!");
}

void freeHeap(void* virtual_address)
{
	panic("this function is not required...!!");
}
