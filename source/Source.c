#include <stdio.h>

typedef struct header
{
	//unsigned short free;
	unsigned int size;
	struct header *prev, *next;
}HEADER;

void *global;

#define HEAD 4

void memory_init(void *ptr, unsigned int size) {
	// pointer first dame na zaciatok ale posunieme ho o int doprava, lebo na prvych 4B bude size pridelenej pamate
	HEADER *first = (HEADER *)((int *)ptr + 1);

	unsigned int *heap_size = (unsigned int*)ptr;
	*heap_size = size - HEAD - sizeof(unsigned int);
	global = ptr;

	//first->free = 1;
	first->size = 8;
}

void memory_alloc(unsigned int size) {
	if (size <= 0) 
		return -1;

	HEADER *curr = (HEADER *)((unsigned int *)global + 1);
	void *result; 

	while (curr->size < size) {
		curr = curr + curr->size + sizeof(HEADER); //jumping to the next free block
		printf("One block checked\n");
	}
	if (curr->size == size) {
		result = (void*)(curr) + curr->size + sizeof(HEADER);

		// unlink the block from free blocks
		curr->prev->next = curr->next;

		printf("curr = %d   ++curr = %d \n", (void*)(curr), (void*)(curr)+curr->size + sizeof(HEADER));
		printf("Exact fitting block allocated.");
	}
	else if (curr->size > size + sizeof(HEADER)) {
		split(curr, size);
		result = (void*)(curr)+curr->size + sizeof(HEADER);
		printf("Fitting block allocated with a split\n");
	}
	else {
		result = NULL;
		print("Sorry. No sufficient memory to allocate\n");
	}
	return result;
}

int memory_free(void *ptr) {

}

int memory_check(void *ptr) {

}