
#ifndef BOOL_ARRAY_H
#define BOOL_ARRAY_H

#define UNS_TRUE 1U
#define UNS_FALSE 0U

typedef struct BoolArray
{
	unsigned char * array;
	int size;
} BoolArray;



BoolArray * newBoolArray(int);

void destroyBoolArray(BoolArray *);

unsigned char getValue(BoolArray *,int);

void setValue(BoolArray *,int, unsigned char);

void printBoolArray(BoolArray *);

int getSize(BoolArray *);

unsigned char isEmpty(BoolArray *);

#endif