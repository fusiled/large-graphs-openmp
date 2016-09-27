#include "BoolArray.h"

#include <stdlib.h>
#include <stdio.h>

#define CHAR_SIZE 8


int get_n_chunks(int);


int get_n_chunks(int size)
{
	return size%CHAR_SIZE==0 ? size/CHAR_SIZE : size/CHAR_SIZE + 1;
}

BoolArray * newBoolArray(int size)
{
	BoolArray * ret_ref = malloc(sizeof(BoolArray));
	int array_dim=get_n_chunks(size);
	ret_ref -> array = malloc(sizeof(unsigned char)*array_dim);
	ret_ref -> size = size;
	return ret_ref;
}

void destroyBoolArray(BoolArray * bArr)
{
	free(bArr->array);
	free(bArr);
}


unsigned char getValue(BoolArray * bArr,int index)
{
	int chunk_index = index/CHAR_SIZE;
	unsigned char chunk = *( (bArr->array)+chunk_index );
	int little_index = index- chunk_index * CHAR_SIZE;
	#ifdef DEBUG
		if(little_index>=CHAR_SIZE || little_index<0)
		{
			printf("Something is wrong with BoolArray little indexing. It is %d\n", little_index);
			return -1;
		}
	#endif
	chunk = chunk << (CHAR_SIZE - little_index);
	return chunk >> CHAR_SIZE;
}


void setValue(BoolArray * bArr, int index, unsigned char value)
{
	if(value!=UNS_TRUE && value !=UNS_FALSE)
	{
		printf("Not valid value (it is not UNS_TRUE or UNS_FALSE). Past %d. Skipping setValue invokation\n", value);
		return;
	}
	//nothing to do
	if(getValue(bArr,index)==value)
	{
		#ifdef DEBUG
			printf("index out of bounds. ignoring set invokation\n");
		#endif
		return;
	}
	if(index >= bArr->size)
	{
		#ifdef DEBUG
			printf("index out of bounds. ignoring set invokation\n");
		#endif
		return;
	}
	int chunk_index = index /CHAR_SIZE;
	char * chunk_ref = (bArr->array)+chunk_index;
	int little_index = index - chunk_index * CHAR_SIZE;
	#ifdef DEBUG
		if(little_index>=CHAR_SIZE || little_index<0)
		{
			printf("Something is wrong with BoolArray little indexing. It is %d", little_index);
			return;
		}
	#endif
	if(value==UNS_TRUE)
	{
		*chunk_ref |= 1U << little_index;	
	}
	else
	{
		*chunk_ref &= ~(1U<<little_index);
	}
}

void printBoolArray(BoolArray * bArr)
{
	for(int i=0; i < get_n_chunks(bArr->size); i++)
	{
		printf("%x ", (bArr->array)[i] );
	}
	printf("\n");
}