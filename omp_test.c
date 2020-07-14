#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <sys/time.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <time.h>
#include <omp.h>
#include "SysDef.h"

unsigned char DotLut[256] = {
	0, 1, 1, 2, 1, 2, 2, 3, 1, 2, 2, 3, 2, 3, 3, 4,
	1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5,
	1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5,
	2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6,
	1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5,
	2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6,
	2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6,
	3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7,
	1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5,
	2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6,
	2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6,
	3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7,
	2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6,
	3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7,
	3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7,
	4, 5, 5, 6, 5, 6, 6, 7, 5, 6, 6, 7, 6, 7, 7, 8
};


Uint32 _GetMsCount()
{
    Uint32 now;
    struct timespec fnow;
    
    clock_gettime(CLOCK_MONOTONIC, &fnow);
    now = fnow.tv_sec * 1000 + fnow.tv_nsec / 1000000;
   
    return now;
}

Uint32 _GetMsTimeFromStart(Uint32 startTime)
{
    Uint32 curTime ;

    curTime =   _GetMsCount();

    if(curTime >= startTime)
        return (curTime - startTime);
    else
        return ( 0xFFFFFFFFUL - startTime + 1 + curTime);
}

int IP_DotCount(unsigned char *lpBuf, Uint32 BufSize, Uint32 *dot_count)
{
	Uint32 i, Count;
	Count = 0;
	for (i = 0; i < BufSize; i++)
		Count += DotLut[*(lpBuf++)];
		
	*dot_count = Count;
	return 0;
}

int IP_DotCount_test2(unsigned char *lpBuf, Uint32 BufSize, Uint32 *dot_count)
{
	Uint32 i, Count;
	Count = 0;
	
	#pragma omp parallel
	{
		#pragma omp for reduction( +:Count)
		for (i = 0; i < BufSize; i++)
			Count += DotLut[*(lpBuf++)];
	}	
	*dot_count = Count;
	return 0;
}

int IP_DotCount_test3(unsigned char *lpBuf, Uint32 BufSize, Uint32 *dot_count)
{
	Uint32 Count1, Count2;
			
	#pragma omp parallel num_threads(2)  
	{		
		if(omp_get_thread_num() == 0)
			IP_DotCount(lpBuf, BufSize >> 1, &Count1);
		else if(omp_get_thread_num() == 1)
			IP_DotCount(lpBuf+(BufSize >> 1), BufSize >> 1, &Count2);
	}	
	*dot_count = Count1+ Count2;
	return 0;
}

int main(int argc, char **argv)
{
    int ret=0, i;    
    unsigned char *pBuf, *pTmp;
    Uint32 size = 10*1024*1024, time, count;

    pBuf = malloc(size);
    if(pBuf)
    {		
		for(pTmp = pBuf, i=0 ; i<size; i++)		
			*pTmp++ = (Uint8)rand();			
		
        time = _GetMsCount();
        IP_DotCount(pBuf, size, &count);
        time = _GetMsTimeFromStart(time);
        printf("IP_DotCount=%u time=%u ms\n",count, time);
		
		time = _GetMsCount();
        IP_DotCount_test2(pBuf, size, &count);
        time = _GetMsTimeFromStart(time);
        printf("IP_DotCount_test2=%u time=%u ms\n",count, time);
		
		time = _GetMsCount();
        IP_DotCount_test3(pBuf, size, &count);
        time = _GetMsTimeFromStart(time);
        printf("IP_DotCount_test3=%u time=%u ms\n",count, time);
		
		free(pBuf);
    }

    
    return ret;
}

