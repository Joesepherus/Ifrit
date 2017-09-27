#include <stdio.h>

void *global = 51651;

void *memory_alloc(unsigned short size) {
	if (size <= 0) 
		return -1;

	void *next;
	void *curr = (char*)global + 2; // adress to the first free block 
	unsigned short free_block = curr; 
	(void*)curr = (char*)global + free_block;

	unsigned short block_size = (unsigned short)curr;

	while (block_size < size) {
		(void*)curr = (char*)curr + (unsigned short)curr; // pamatam si velkost bloku
		block_size = (unsigned short)curr;
		(void*)next = (char*)curr + (unsigned short)curr; // pa
		printf("One block checked\n");
	}
	
	if (block_size == size) {
		// unlink the block from free blocks
		(void*)curr = (char*)curr + (unsigned short)curr; 
		(void*)next = curr;
		(void*)curr += (unsigned short)curr;
		(unsigned short)curr = NULL; //set next as NULL
		curr->prev->next = curr->next;

		printf("curr = %d   ++curr = %d \n", (void*)(curr), (void*)(curr)+curr->size + sizeof(HEADER));
		printf("Exact fitting block allocated.");
	}

	else if (block_size > size) {
		split(curr, size);
		result = (void*)(curr)+curr->size + sizeof(HEADER);
		curr->prev->next = curr->next;

		printf("found a free block with size: %d\n", block_size);
	}

	else (block_size < size) {
		curr = NULL;
		print("Sorry. No sufficient memory to allocate\n");

	}

}*/

int main() {
	void *ptr = 51614;
	void *next = (char*)ptr + 2;
	printf("%p %c, %d, %p\n", ptr, (char*)ptr, sizeof(ptr), next);

	memory_alloc(4);
	memory_alloc(50000);
	getchar();
}