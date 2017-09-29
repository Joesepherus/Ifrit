#include <stdio.h>

char *global[50000];

void *memory_alloc(unsigned short size) {

	if (size <= 0) 
		return -1;

	void *next;

	int *curr = (char*)global + 2; // adress to the first free block 
	printf("\nwtff %p %p\n", global, curr);
	unsigned short free_block = *((int*)curr); // save the location of the free_block
	printf("freeblock=%hu", free_block); 
	curr = (char*)global + 2 + free_block; // go to the first free block
	//printf("lol%p", curr);

	unsigned short block_size = *((int*)curr); // save the free blocks size
	int *nextFree = ((char*)curr) + 2; // save the position of the next free block
	printf("nextFree = %p %p", *nextFree, ((char*)curr + 2));

	// loop until you find a suitable free block
	while (block_size < size || *nextFree != 0) {
		(void*)curr = *nextFree; // get to the position of the next free block
		block_size = *curr; // get the next free blocks size
		printf("\nkurvaa %p %d %d\n", curr, *curr, block_size);

		nextFree = ((char*)curr) + 2; // save the position of the next free block
		printf("\nCHECKED\n");
	}
	
	if (block_size == size) {
		// unlink the block from free blocks
		(void*)curr = (char*)curr + (unsigned short)curr; 
		(void*)nextFree = curr;
		(void*)curr = (char*)curr + (unsigned short)curr;
		(unsigned short)curr = NULL; //set next as NULL
		//curr->prev->next = curr->next;

		// printf("curr = %d   ++curr = %d \n", (void*)(curr), (void*)(curr)+curr->size + sizeof(HEADER));
		printf("Exact fitting block allocated.");
	}

	else if (block_size > size) {
		// split(curr, size);
		// result = (void*)(curr)+curr->size + sizeof(HEADER);
		// curr->prev->next = curr->next;

		printf("found a free block with size: %hu\n", block_size);
	}

	/*else (block_size < size) {
		curr = NULL;
		print("Sorry. No sufficient memory to allocate\n");

	}*/

	// nastavime next free block and its size
	curr = ((char*)curr) + 2; // get the position of the next free block

	*curr = ((char*)curr) + block_size;	// set the position of the next free block to point to
	printf("\n%p, %p, %p\n", *curr, (char*)curr + block_size, curr);
	(void*)curr = (char*)curr + block_size; // set the next blocks size 
	printf("%\nOMG %p %p\n", curr, (char*)curr + block_size);

	*curr = 50000 + (int)(global) - (int)curr;
	printf("global - curr %d %d %d", curr, global, (int)curr - (int)global);
	printf("\n%p, %d\n", curr, *curr);

	/*(void*)curr = (char*)curr + (unsigned short)curr;
	*curr = NULL;*/
}

int main() {

	void *ptr = malloc(50);
	void *next = (char*)ptr + 2;
	// printf("%p %c, %d, %p\n", ptr, (char*)ptr, sizeof(ptr), next);

	int i = 50;
	int *curr = (char*)global + 2; // adress to the first free block 
	*curr = i;
	printf("curr = %p a global = %p", curr, (char*)global + 2);
	printf("%d", *((int*)curr));

	unsigned short j = 50;
	curr = (char*)global + 2 + 50; // adress to the first free block 
	*curr = j;

	curr = ((char*)curr) + 2;
	*curr = 0;

	printf("umm%hu", *((int*)curr));

	/*int i = 50;
	void *pointer;
	pointer = &i;
	printf("%d", *((int*)pointer));*/
	memory_alloc(4);
	memory_alloc(4);
	memory_alloc(4);
	memory_alloc(4);


	getchar();
}