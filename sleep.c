#include "stdio.h"
#include "stdlib.h"

int main(int argc, char* argv[])
{
	//printf("Hello World!\n");
	if(argc == 2)
		_sleep(atoi(argv[1]));
	return 0;
}

