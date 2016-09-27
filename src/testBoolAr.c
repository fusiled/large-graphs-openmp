#include <stdio.h>

#include "BoolArray.h"

int main(int argc, char const *argv[])
{
	BoolArray * ba = newBoolArray(20);
	printBoolArray(ba);
	printf("isEmpty is %x\n", isEmpty(ba));
	setValue(ba, 4, UNS_TRUE);
	setValue(ba, 21, UNS_TRUE);
	setValue(ba, 19, UNS_TRUE);
	printBoolArray(ba);
	printf("value: %d\n",getValue(ba, 4) );
	printBoolArray(ba);
	setValue(ba, 4, UNS_FALSE);
	printf("value: %d\n",getValue(ba, 4) );
	printf("isEmpty is %x\n", isEmpty(ba));
	printBoolArray(ba);
	destroyBoolArray(ba);
	printf("UNS_TRUE is the same?, %d", UNS_TRUE==1U);
	return 0;
}