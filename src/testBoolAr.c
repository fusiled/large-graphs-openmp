

#include "BoolArray.h"

int main(int argc, char const *argv[])
{
	BoolArray * ba = newBoolArray(20);
	printBoolArray(ba);
	setValue(ba, 4, UNS_TRUE);
	setValue(ba, 21, UNS_TRUE);
	setValue(ba, 19, UNS_TRUE);
	printBoolArray(ba);
	setValue(ba, 4, UNS_FALSE);
	printBoolArray(ba);
	destroyBoolArray(ba);
	return 0;
}