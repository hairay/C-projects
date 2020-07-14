#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#define MAX_LINE_LENGTH 4096

char *gLevelTextTable[] = {
    "HALT_MSG_LEVEL",            
    "ERROR_MSG_LEVEL",       
    "IMPORTANT_MSG_LEVEL",
    "NOTE_MSG_LEVEL",
    "SCANNER_MSG_LEVEL",     
    "PRINTDOC_MSG_LEVEL",    
    "PARSER_MSG_LEVEL",      
    "IO_MSG_LEVEL",          
    "IO_USB_MSG_LEVEL",      
    "PRINTENG_MSG_LEVEL",    
    "SCANDOC_MSG_LEVEL",     
    "FUNCTION_MSG_LEVEL",    
    "IP_MSG_LEVEL",          
    "COPY_MSG_LEVEL",        
    "UI_MSG_LEVEL",          
    "MGR_MSG_LEVEL",         
    "DEBUG_TOOL_MSG_LEVEL",  
    "PRT_DVT_MSG_LEVEL",     
    "FLASH_MSG_LEVEL",       
    "BACKUP_MSG_LEVEL",      
    "FAX_MSG_LEVEL",         
    "NET_MSG_LEVEL",         
    "ADF_MSG_LEVEL",         
    "SHELL_MSG_LEVEL",       
    "JOB_MGR_LEVEL",         
    "SYS_MGR_LEVEL",         
    "JOBPRINT_MSG_LEVEL",    
    "IPS_MSG_LEVEL",         
    "PB_MSG1_LEVEL",         
    "PB_MSG2_LEVEL",         
    "M3S_MSG_LEVEL",         
    "M3P_MSG_LEVEL",         
};



int main(int argc, char **argv)
{
	FILE *fp = NULL;
	FILE *fpWrite = NULL;
	int retAp = 0, len, i;
	unsigned int lineCount = 0, levelCount = 0;
	char buff[MAX_LINE_LENGTH];
	char outBuff[MAX_LINE_LENGTH];
	char *testName = "test.txt";
	unsigned long getVal[32], level;
    double ans;
    unsigned long newAns;

	if (argc != 2)	
		argv[1] = testName;		
	

	fp = fopen(argv[1],"r");	
    if(fp == NULL)
	{
		printf("open %s fail\n", argv[1]);
		retAp = -2;
		goto exit_read_log;
	}

	sprintf(buff, "output_%s",  argv[1]);
	fpWrite = fopen(buff, "w");
	if(fpWrite == NULL)
	{
		printf("open %s fail\n", buff);
		retAp = -2;
		goto exit_read_log;
	}
    for (i = 0; i < 32; i++)
        getVal[i] = 0;

    while (fgets(buff, MAX_LINE_LENGTH, fp) != NULL)
    {
        lineCount++;
        if (buff[0] == 0x5B && buff[9] == 0x5D)
        {            
            buff[9] = 0;
            level = strtoul(&buff[1], NULL, 16);
            ans = log2((double)level);            
            newAns = (unsigned long)ans;
            if((double)newAns == ans)
            {
                getVal[newAns]++;    
                levelCount++;
            }            
        }
    }
    for (i = 0; i < 32; i++)
    {
        len = sprintf(outBuff, "%40s [%08lX] count=%08lu ratio:%lf\r",  gLevelTextTable[i], (unsigned long)(1 << i), getVal[i], (double)getVal[i]/levelCount);
        fwrite(outBuff, 1, len, fpWrite);
    }

    len = sprintf(outBuff, "log lines = %u level lines = %u\r",  lineCount, levelCount);        
    fwrite(outBuff, 1, len, fpWrite);

exit_read_log:
	if(fp)
		fclose(fp);

	if(fpWrite)
		fclose(fpWrite);

    return retAp;
}

