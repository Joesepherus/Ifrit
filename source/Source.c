// zadanie1.c -- Jozef Maloch, 2.10.2017 15:26

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void *global;
//char region[100];


#define MOVE(ptr, size) ((char*)ptr + size)

void memory_init(void *ptr, unsigned int size) {
	global = ptr;
	printf("%llu", global);
	long long *curr = ptr;
	*curr = size; // setting of the lenght of the whle memory 
	curr = MOVE(curr, 4); // adress to the first free block 
	*curr = MOVE(curr, 8); // address first free block
	curr = MOVE(curr, 8);
	*curr = size - 12;
	curr = MOVE(curr, 2); // address of the first flag
	*curr = 0; // set the flag to 0 = free
}

void *memory_alloc(unsigned short size) {

	if (size <= 0)
		return NULL;
	unsigned int moved = 0;
	long long *result, *prev, *prev_free = NULL;
	// printf("GLOBAL = %d   ", global);
	long long *curr = global; // set current to point to the start of the main memory
	int main_size = *((int*)curr); // save the main memory size
	curr = MOVE(curr, 4); // move by 4B to the pointer address of the first free block
	long long free_block = *((long long*)curr); // save the location of the free_block
												//moved += free_block + 4;
												//curr = MOVE(curr, free_block + 4); // go to the first free block
	curr = free_block;
	unsigned short block_size = *((unsigned short*)curr); // save the free blocks size
	unsigned char isFree = *((unsigned char*)MOVE(curr, 2));
	if (isFree == 1) {
		//printf("The block is already allocated. Sry\n");
		return NULL;
	}

	unsigned int nextFree = 0; // save the position of the next free block
	int leak = main_size - (((char *)curr) - ((char *)global) - 8);
	// loop until you find a suitable free block
	while (block_size < size || nextFree != NULL || leak <= 7) { // 7 is the minimum an allocated block can be with a header 6 and body 1
		prev_free = curr;
		curr = MOVE(curr, 3); // get to the position of the next block to figure out where it is 
		curr = (*(long long*)curr);
		block_size = *curr; // get the next free blocks size
		nextFree = *((unsigned short*)MOVE(curr, 2)); // save the position of the next free block
		moved += block_size + 10;
		printf("\n One block checked\n");
	}

	unsigned int left = main_size - (((char *)curr) - ((char *)global));
	printf("left = %d\n", left);
	if (left < size + 6)
		return NULL;

	if (block_size == size) {
		printf("Exact fitting block allocated. %hu and pointer %p    %llu\n", size, curr, curr);
	}

	else if (block_size > size) {
		// split(curr, size);
		// result = (void*)(curr)+curr->size + sizeof(HEADER);
		// curr->prev->next = curr->next;
		printf("found a free block with size: %hu and pointer %p     %llu\n", size, curr, curr);
	}

	else if (block_size < size) {
		curr = NULL;
		printf("Sorry. No sufficient memory to allocate\n");
	}

	long long helpCurrNext = 0;
	helpCurrNext = *((long long*)MOVE(curr, 3));

	// set the new size of block at header
	*curr = (unsigned int*)size;
	// printf("\ncurr - header %d ", curr);
	prev = (char*)global + 4;  // save the location of the previous free block

							   // set the flag of the block at header
	result = MOVE(curr, 3);

	curr = MOVE(curr, 2);
	*curr = (unsigned char*)1;



	// get prev
	// set the size of the block at the footer
	curr = MOVE(curr, size + 1);

	// printf("curr - footer %d ", curr);
	/*
	*curr = (unsigned char*)size;
	putchar('\n');
	for (int i = 0; i < 100; i++) {
		if (i % 10 == 0)
			putchar('\n');

		printf("%d ", region[i]);
	}
	putchar('\n');
	//set the flag at the footer
	curr = MOVE(curr, 2);
	*curr = (unsigned char*)1;

	// unlink the block from free blocks
	moved += size + 3 + 3 - 4 + 1 - 1;
	// printf("\nprevious position %d\n", curr);

	//prev = *((int*)MOVE(curr, 6));
	*/
	// setting new free block if none else exists, serves as a split of needed size vs main size
	//if (nextFree == 0) {
	curr = MOVE(curr, 5);
	isFree = *((unsigned char*)curr);
	
	if (isFree == 204 || isFree == 0 && left - 7 > 0) {
		curr = MOVE(curr, -2);
		*prev = (long long*)curr;
		if (prev_free != NULL) {
			prev_free = MOVE(prev_free, 3);
			*prev_free = NULL;
			prev_free = MOVE(prev_free, 4);
			*prev_free = (long long*)curr;
		}
		// printf("new - header %d\n", curr);
		//curr = MOVE(curr, 2);
		*curr = (unsigned short*)(main_size - moved);

		curr = MOVE(curr, 2);
		*curr = 0;
		curr = MOVE(curr, 4);
		*curr = (char*)global + 4;
	}
	else if (helpCurrNext != 0){
		prev_free = (char*)global + 4;
		*prev_free = (long long*)helpCurrNext;
	}
	return result;
}

int memory_free(void* valid_ptr) {
	long long *curr = valid_ptr; // set current to point to the start of the main memory
	long long *next;
	long long saved_next;
	// printf("\ntest%d %d %d\n", ((char *)curr) - ((char *)global), (char*)curr, (char*)global);
	unsigned short moved = ((char *)curr) - ((char *)global) - 8;
	curr = MOVE(curr, -3);
	// scenario 1
	/*curr -= 1;
	unsigned char prev_flag = *((int*)curr);
	printf("\nPREV FLAG = %d", prev_flag);*/
	// scenario 2

	// scenario 3

	// set the first block to point this new free block
	next = (char*)global + 4;
	saved_next = *next;
	*next = (long long*)curr;
	curr = MOVE(curr, 2);
	unsigned char flag = *((unsigned char*)curr);
	if (flag == 1) {
		printf("yup it's allocated\n");
		*curr = (unsigned char*)0; // set the flag to 0
				   // set the next free block 
		curr = MOVE(curr, 1);
		*curr = (long long*)saved_next;
	}
	return 0;
}

int memory_check(void *ptr) {
	int *curr;
	curr = ptr;
	curr = MOVE(curr, 2);
	if (*curr != 1)
		return 0;
	return 1;
}

// Vlastna funkcia main() je pre vase osobne testovanie. Dolezite: pri testovacich scenaroch sa nebude spustat!
int main() {
	char region[100];

	memory_init(region, 100);
	void *pointers[50];
	int counter = 0;

	pointers[counter] = (char*)memory_alloc(8);
	if (pointers[counter])
		memset(pointers[counter++], 0, 8);

	pointers[counter] = (char*)memory_alloc(8);
	if (pointers[counter])
		memset(pointers[counter++], 0, 8);
	pointers[counter] = (char*)memory_alloc(8);
	if (pointers[counter])
	memset(pointers[counter++], 0, 8);
	pointers[counter] = (char*)memory_alloc(8);
	if (pointers[counter])
	memset(pointers[counter++], 0, 8);
	pointers[counter] = (char*)memory_alloc(8);
	if (pointers[counter])
	memset(pointers[counter++], 0, 8);
	pointers[counter] = (char*)memory_alloc(8);
	if (pointers[counter])
	memset(pointers[counter++], 0, 8);
	pointers[counter] = (char*)memory_alloc(8);
	if (pointers[counter])
	memset(pointers[counter++], 0, 8);
	memory_free(pointers[0]);
	memory_free(pointers[1]);
	memory_free(pointers[2]);
	pointers[counter] = (char*)memory_alloc(8);
	if (pointers[counter])
	memset(pointers[counter++], 0, 8);
	putchar('\n');
	pointers[counter] = (char*)memory_alloc(8);
	if (pointers[counter])
	memset(pointers[counter++], 0, 8);
	pointers[counter] = (char*)memory_alloc(8);
	if (pointers[counter])
	memset(pointers[counter++], 0, 8);
	pointers[counter] = (char*)memory_alloc(8);
	if (pointers[counter])
	memset(pointers[counter++], 0, 8);
	pointers[counter] = (char*)memory_alloc(8);
	if (pointers[counter])
		memset(pointers[counter++], 0, 8);

	getchar();
	return 0;
}