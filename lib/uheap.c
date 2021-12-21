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

int isFreeSpace ( unsigned int currentSize);


struct Space {
	void* str_address ;
	void* end_address ;
	int size ;
};


struct Space free_spacess[40] ;
struct Space alloceted_spacess [40];
int index = 0 ;
void* newAddress = (void*) (USER_HEAP_START) ;
int sizeOfUserHeap = USER_HEAP_MAX - USER_HEAP_START ;
int free_index = 0 ;
//==================================================================================//
//============================ REQUIRED FUNCTIONS ==================================//
//==================================================================================//
void* malloc(uint32 size)
{
	//TODO: [PROJECT 2021 - [2] User Heap] malloc() [User Side]
	// Write your code here, remove the panic and write your code

	//This function should find the space of the required range
	//using the BEST FIT strategy

	//refer to the project presentation and documentation for details

	size = ROUNDUP(size,PAGE_SIZE);
	void* returnedAdd ;
	if(free_index == 0){
		if(isFreeSpace(size)){
			returnedAdd = newAddress ;
			newAddress = (void*) ( newAddress + size );
			alloceted_spacess[index].str_address=returnedAdd ;
			alloceted_spacess[index].size=size ;
			alloceted_spacess[index].end_address = newAddress ;
			index++ ;
			//kernal call mem cal
			sys_allocateMem((uint32)returnedAdd,size);
			return returnedAdd ;
		}
		else
		{
			return NULL ;
		}
	}else{

		int min =USER_HEAP_MAX-USER_HEAP_START  ;
		int min_index =0;
		void * vir_address;

		//find min defference of free space [i] and input size
		for(int i =0 ; i<free_index ;i++){
			if(free_spacess[i].str_address==NULL)
			{
				continue;
			}
			if(free_spacess[i].size-size <0)
			{
				continue;
			}
			if(min > free_spacess[i].size-size  )
			{
				min = free_spacess[i].size-size ;
				min_index= i ;
			}
		}

		//check the free spaces in the heap
		int min_s =sizeOfUserHeap-size ;


		if(min_s>0 && min ==USER_HEAP_MAX-USER_HEAP_START ){
			return NULL ;
		}

			if(min_s<min && min_s>0)
		{
			returnedAdd=(void *) USER_HEAP_MAX-sizeOfUserHeap ;

		}else{
			returnedAdd = free_spacess[min_index].str_address;

			if(free_spacess[min_index].size > size )
			{

				free_spacess[min_index].str_address +=size;
				free_spacess[min_index].size -=size;
			}
			else
			{
				free_spacess[min_index].str_address=NULL;
			}

		}

		//pass returned address to kenrl
		sys_allocateMem((uint32)returnedAdd,size);
		alloceted_spacess[index].str_address=returnedAdd ;
		alloceted_spacess[index].size=size ;
	    alloceted_spacess[index].end_address = returnedAdd+alloceted_spacess[index].size;
	    index++ ;
		return 	returnedAdd ;
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
	// Write your code here, remove the panic and write your code

	//you should get the size of the given allocation using its address

	//refer to the project presentation and documentation for details

	int check=0;
	for(int i=0 ; i<index ;i++){
		if(alloceted_spacess[i].str_address==NULL)
		{
			continue;
		}

		if(virtual_address == alloceted_spacess[i].str_address){
			if(free_index !=0){


			for(int j =0 ;j<free_index ;j++){
				if(free_spacess[j].str_address==NULL)
				{
					continue;
				}
				if(alloceted_spacess[i].str_address == free_spacess[j].end_address){
					if (check==0)
					{
					  free_spacess[j].end_address = alloceted_spacess[i].end_address;
					  free_spacess[j].size += alloceted_spacess[i].size ;
					  check ++;
					}
					else
					{
						for(int z=0;z<free_index;z++)
						{
							if(free_spacess[j].str_address == free_spacess[z].end_address){
								free_spacess[j].str_address = free_spacess[z].str_address ;
								free_spacess[j].size += free_spacess[z].size ;
								free_spacess[z].str_address=NULL ;

							}
							else if(free_spacess[j].end_address == free_spacess[z].str_address){
								free_spacess[j].end_address = free_spacess[z].end_address ;
								free_spacess[j].size += free_spacess[z].size ;
								free_spacess[z].str_address=NULL ;
							}
						}
					}
				}
				 if(alloceted_spacess[i].end_address == free_spacess[j].str_address){

					 if(check==0){
						 free_spacess[j].str_address = alloceted_spacess[i].str_address;
						free_spacess[j].size += alloceted_spacess[i].size ;
						check ++ ;
					 }else {
						 for(int z=0;z<free_index;z++)
						 {
							 if(free_spacess[z].str_address==NULL)
							 {
								 continue;
							 }
						 	 if(free_spacess[j].str_address == free_spacess[z].end_address){
						 			free_spacess[j].str_address = free_spacess[z].str_address ;
						 			free_spacess[j].size += free_spacess[z].size ;
						 			free_spacess[z].str_address = NULL ;

						 		}
						 		else if(free_spacess[j].end_address == free_spacess[z].str_address){
						 			free_spacess[j].end_address = free_spacess[z].end_address ;
						 		    free_spacess[j].size += free_spacess[z].size ;
						 			free_spacess[z].str_address=NULL ;
						 		}
						 }

					 }

				}




			 }
			}
			else
			{
				free_spacess[free_index]=alloceted_spacess[i];
			}
			free_index++;
			sys_freeMem((uint32)alloceted_spacess[i].str_address ,(uint32)alloceted_spacess[i].size);
			alloceted_spacess[i].str_address=NULL;
			break;
		}
	}



}

//==================================================================================//
//================================ OTHER FUNCTIONS =================================//
//==================================================================================//


int isFreeSpace ( unsigned int currentSize){
int x ;
	if(sizeOfUserHeap >= currentSize){
		x= 1 ;
		sizeOfUserHeap = sizeOfUserHeap - currentSize ;

	}else{
		x=0 ;
	}


	return x ;
}

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
