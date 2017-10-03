#include <stdio.h>

char *global[50000];

#define MOVE(ptr, size) ((char*)ptr + size)

int alpha = 1;
unsigned short *checker = NULL;


void *memory_alloc(unsigned short size) {

	if (size <= 0) 
		return -1;
	unsigned int moved = 0;
	unsigned int *result, *prev, *next, *flag;
	printf("GLOBAL = %d   ", global);
	int *curr = global; // set current to point to the start of the main memory
	 int main_size = *((int*)curr); // save the main memory size
	curr = MOVE(curr, 4); // move by 4B to the pointer of the first free block
	int free_block = *((int*)curr); // save the location of the free_block
	moved += free_block + 4;
	curr = MOVE(curr, free_block + 4); // go to the first free block
	unsigned short block_size = *((unsigned short*)curr); // save the free blocks size
	unsigned char isFree = *((unsigned char*)MOVE(curr, 2));
	if (isFree == 1) {
		printf("The block is already allocated. Sry\n");
		return NULL;
	}
	
	unsigned int nextFree = 0; // save the position of the next free block

	// loop until you find a suitable free block
	while (block_size < size || nextFree != 0) {
		(void*)curr = nextFree; // get to the position of the next free block
		block_size = *curr; // get the next free blocks size
		nextFree = MOVE(curr, 2); // save the position of the next free block
		moved += block_size + 10;
		printf("\n One block checked\n");
	}
	
	result = curr;

	if (block_size == size) {

		
		printf("Exact fitting block allocated.");
	}

	else if (block_size > size) {
		// split(curr, size);
		// result = (void*)(curr)+curr->size + sizeof(HEADER);
		// curr->prev->next = curr->next;


		printf("found a free block with size: %hu and pointer %p %d\n", block_size, curr, curr);
	}

	else if (block_size < size) {
		curr = NULL;
		printf("Sorry. No sufficient memory to allocate\n");

	}

	// set the new size of block at header
	*curr = size;
	printf("\ncurr - header %d ", curr);
	prev = (char*)global + 4;  // save the location of the previous free block

	// set the flag of the block at header
	curr = MOVE(curr, 2);
	*curr = 1;
	if (alpha == 4)
		checker = curr;

	// get prev
	// set the size of the block at the footer
	curr = MOVE(curr, size + 1);

	printf("curr - footer %d ", curr);

	*curr = size;
	//set the flag at the footer
	curr = MOVE(curr, 2);
	unsigned char flagSet = 1;
	*((unsigned char*)curr) = flagSet;

	// unlink the block from free blocks
	moved += size + 3 + 3 - 4 + 1 -1;
	printf("\nprevious position %d\n", curr);

	//prev = *((int*)MOVE(curr, 6));
	
	// setting new free block if none else exists, serves as a split of needed size vs main size
	//if (nextFree == 0) {
		curr = MOVE(curr, 3);
		isFree = *((unsigned short*)curr);

		if (isFree == 0) {
			curr = MOVE(curr, -2);
			printf("new - header %d\n", curr);

			(unsigned short*)*curr = (unsigned short*)(main_size - moved);
			curr = MOVE(curr, 2);
			*curr = 0;
			curr = MOVE(curr, 4);
			*curr = (char*)global + 4;
		//}
		*prev = moved;

		/*curr = MOVE(curr, size + 10);
		*prev = moved;

		*curr = main_size - moved;
		curr = MOVE(curr, 2);
		*curr = 0;
		curr = MOVE(curr, 4);
		*curr = (char*)global + 4;*/


		//prev = MOVE(prev, 2);
	}
	//prev = MOVE(prev, 2);
	//*prev = moved;
	/*
	// nastavime next free block and its size
	curr = ((char*)curr) + 2; // get the position of the next free block

	*curr = ((char*)curr) + block_size;	// set the position of the next free block to point to
	(void*)curr = (char*)curr + block_size; // set the next blocks size 
	*curr = 50000 + (int)(global) - (int)curr;*/
		printf("%d", checker);

		alpha++;
	return result;
}

int memory_free(void* valid_ptr) {
	int *curr = (char*)valid_ptr; // set current to point to the start of the main memory
	int *prev;
	printf("\ntest%d %d %d\n", ((char *)curr) - ((char *)global), (char*)curr, (char*)global);
	unsigned short moved = ((char *)curr) - ((char *)global) - 8;

	// scenario 1
	/*curr -= 1;
	unsigned char prev_flag = *((int*)curr);
	printf("\nPREV FLAG = %d", prev_flag);*/
	// scenario 2

	// scenario 3

	curr = MOVE(curr, 2);
	unsigned char flag = *((int*)curr);
	if (flag == 1) {
		printf("yup it's allocated");
		*curr = 0; // set the flag to 0
	}
	curr = (char*)global + 4;
	*curr = moved;

}

int main() {
	void *ptr = malloc(50);
	void *next = (char*)ptr + 2;
	// printf("%p %c, %d, %p\n", ptr, (char*)ptr, sizeof(ptr), next);

	int i = 50;
	*global = 50000;
	int *curr = (char*)global + 4; // adress to the first free block 
	*curr = i;
	printf("curr = %p a global = %p", curr, (char*)global + 4);
	printf("%d", *((int*)curr));

	unsigned short j = 50;
	curr = (char*)global + 8 + 50; // adress to the first free block 
	*curr = j;

	curr = ((char*)curr) + 2;
	*curr = 0;

	curr = (char*)global + 8 + 50 + 7; // adress to the first free block 
	*curr = (char*)global + 4;

	printf("umm%hu", *((int*)curr));

	/*int i = 50;
	void *pointer;
	pointer = &i;
	printf("%d", *((int*)pointer));*/
	memory_alloc(4);
	memory_alloc(4);
	curr = memory_alloc(4);
	printf("CURR = %d    ", curr);
	memory_alloc(20);
	memory_alloc(20);
	memory_free(curr);
	memory_alloc(4);
	memory_alloc(4);

	getchar();
	return 0;
}