#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <omp.h>

int main(int argc, char **argv)
{
	FILE *fp = NULL;	
	unsigned int checkSum = 0;
    int retAp = 0, numRead = 1, i;
    unsigned char buffer[65536];

    fp = fopen(argv[1], "rb");
    if(fp == NULL)
	{
		printf("open %s fail\n", argv[1]);
		retAp = -2;
		goto exit_read_log;
	}

    while(numRead)
    {
        numRead = fread(buffer, 1, sizeof(buffer), fp);
        #if 1
        #pragma omp parallel
        {
            #pragma omp for reduction( +:checkSum)
            for (i = 0; i < numRead; i++)
                checkSum += buffer[i];
        }	
        #else
        for (i = 0; i < numRead; i++)
            checkSum += buffer[i];
        #endif    
    }
    printf("%u", checkSum);

exit_read_log:
	if(fp)
		fclose(fp);



    return retAp;
}

