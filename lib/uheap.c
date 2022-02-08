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
	void* startAddress;
	uint32 size;
	uint32 numberOfPages;
}allocatedBlocks[131072];

struct Block freeBlocks[131072] = {{(void*)USER_HEAP_START, USER_HEAP_MAX - USER_HEAP_START, (USER_HEAP_MAX - USER_HEAP_START) / PAGE_SIZE}};
uint32 allocatedIdx = 0, freeIdx = 1;


void* malloc(uint32 size)
{
	// TODO: [PROJECT 2021 - [2] User Heap] malloc() [User Side]
	uint32 minSize = USER_HEAP_MAX - USER_HEAP_START + 5;
	uint32 tempIdx = -1;

	for (int i = 0; i < freeIdx; i++)
	{
		if (size <= freeBlocks[i].size && freeBlocks[i].size < minSize)
		{
			minSize = freeBlocks[i].size;
			tempIdx = i;
		}
	}

	size = ROUNDUP(size, PAGE_SIZE);
	uint32 numberOfPages = size / PAGE_SIZE;

	if (tempIdx != -1)
	{
		allocatedBlocks[allocatedIdx].startAddress = freeBlocks[tempIdx].startAddress;
		allocatedBlocks[allocatedIdx].size = size;
		allocatedBlocks[allocatedIdx].numberOfPages = numberOfPages;

		freeBlocks[tempIdx].size -= size;
		freeBlocks[tempIdx].numberOfPages -= numberOfPages;
		freeBlocks[tempIdx].startAddress = freeBlocks[tempIdx].startAddress + size;

		sys_allocateMem((uint32)allocatedBlocks[allocatedIdx].startAddress, size);
		allocatedIdx++;
		return allocatedBlocks[allocatedIdx - 1].startAddress;
	}
	return NULL;
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

	for (int i = 0; i < allocatedIdx; i++)
	{
		if (virtual_address == allocatedBlocks[i].startAddress && allocatedBlocks[i].size != 0)
		{
			sys_freeMem((uint32)virtual_address, allocatedBlocks[i].size);
			for (int j = 0; j < freeIdx; j++)
			{
				if (virtual_address < freeBlocks[j].startAddress)
				{
					for (int k = freeIdx; k > j; k--)
					{
						freeBlocks[k] = freeBlocks[k - 1];
					}
					freeBlocks[j] = allocatedBlocks[i];
					break;
				}
			}
			allocatedBlocks[i].size = 0;
			allocatedBlocks[i].numberOfPages = 0;
			freeIdx++;
		}
	}

	for (int i = 0; i < freeIdx - 2; i++)
	{
		if(freeBlocks[i].startAddress + freeBlocks[i].size == freeBlocks[i + 1].startAddress)
		{
			freeBlocks[i].size += freeBlocks[i + 1].size;
			freeBlocks[i].numberOfPages += freeBlocks[i + 1].numberOfPages;
			freeBlocks[i + 1].size = 0;
			freeBlocks[i + 1].numberOfPages = 0;
		}
	}
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
