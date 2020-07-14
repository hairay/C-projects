#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define MAX_LINE_LENGTH    		4096


char *gSensorTextTable[] = {
	"REG",
	"EXIT",
    "FrontCover",
    "MainTray_PaperFeed",
    "MainTray_PaperIn",
    "ManualTray_PaperIn",
    "Deskew",
    "FuserOut",
    "BackCover",
    "PaperShift",
    "Duplex",
    "HivChargerError",
};

char *gTempTextTable[] = {
	"A3_TEMP",
	"A4_TEMP",
    "A4_Out_TEMP",
    "Fuser_Env_TEMP",
    "Target_TEMP"    
};

char *gIoTextTable[] = {
	"LAMP_SW",
	"HV_CHG",
	"HV_DEV",
	"HV_DEV_REV",
	"HV_GRID",
	"HV_TRANS",
	"HV_TRANSREV",    
	"MAIN_MOTOR_DIR",
	"MAIN_MOTOR_EN",        	
	"FW_RESET",	
	//10
	"EXIT_MOTOR_EN",
	"PAPER_SHIFT_MOTOR_MS1",
	"PAPER_SHIFT_MOTOR_MS2",	
	"PICKUP_CLUTCH",
	"DESKEW_CLUTCH",
	"V24_CONTROL",
	"MOTOR_CURRENT_LOW",
	"IR_SENSOR_POWER_EN",
	"EXIT_MOTOR_DIR",
	"PAPER_SHIFT_MOTOR_DIR",
	//20
	"SENSOR_POWER_EN",		
	"V24_EN",
	"FAN_LOW_VOLTAGE",
	"MANUAL_PICKUP_CLUTCH",
	"PAPER_SHIFT_MOTOR_EN",	
	"FUSER_AC_POWER_SW",
	"Vsync"
};

char *gSearchPattern[] = {
	"Sensor_GetSensorStatus",
	"Sensor_PrintStatas"
};

char *gSearchFixPattern[] = {
	"[Sensor]"	
};

int CheckStringIfNum(char *inStr)
{
	int numCount = 0, i;
	int len = strlen(inStr);

	for (i = 0; i< len; i++)
		if(inStr[i] >= '0' && inStr[i] <= '9')
			numCount++;
		else
			break;

	if(numCount == len)
		return 1;
	else	
		return 0;
}
int GetRightNumber(char *inStr, unsigned int getVal[])
{
	int count = 0, len;
	char	seps[]  = " =(,.)";	
	char	*token;

	len = strlen(inStr);
	if(inStr[len-1] == '\n' || inStr[len-1] == '\r')
		inStr[len - 1] = 0;

	token = strtok(inStr, seps);
	while( token != NULL )
	{
		if(CheckStringIfNum(token))
		{
			getVal[count] = atoi(token);
			count++;
		}
		token = strtok( NULL, seps );		
	}
	return count;
}

unsigned int GetMsTimeFromStart(unsigned int startTime, unsigned int curTime)
{				
	if(curTime >= startTime)
		return (curTime - startTime);	
	else	
		return ( 0xFFFFFFFFUL - startTime + curTime+1);					
}

int main(int argc, char **argv)
{
	FILE *fp = NULL;
	FILE *fpWrite = NULL;
	int retAp = 0, len, i, count;
	unsigned int lineCount = 0, curTime = 0, maxTime = 0;
	char buff[MAX_LINE_LENGTH];
	char outBuff[MAX_LINE_LENGTH];
	char *testName = "test.txt";
	unsigned int getVal[16];
	char *p;

	if (argc != 2)	
		argv[1] = testName;		
	

	fp = fopen(argv[1],"r");	
    if(fp == NULL)
	{
		printf("open %s fail\n", argv[1]);
		retAp = -2;
		goto exit_read_log;
	}

	strcpy(outBuff, argv[1]);
	p =  strrchr(outBuff, '\\');
	if(p != NULL)
	{
		*p = '\0'; 	
		sprintf(buff, "%s\\MCU_%s.log",  outBuff, p+1);
	}
	else
	{
		sprintf(buff, "MCU_%s.log",  argv[1]);
	}
	fpWrite = fopen(buff, "w");
	if(fpWrite == NULL)
	{
		printf("open %s fail\n", buff);
		retAp = -2;
		goto exit_read_log;
	}

	len = sprintf(buff, "%40s %6s %8s %12s %12s\r", "Name", "ID", "state","time(us)","line");
	fwrite(buff, 1, len, fpWrite);

	while(fgets(buff, MAX_LINE_LENGTH, fp) != NULL)
	{
		char *ret , *ret2 ;

		lineCount++;
		for (i = 0;  i <2; i++)
		{
			count = len = 0;
			ret = ret2 = NULL;
			ret = strstr(buff, gSearchPattern[i]);
			if(ret != NULL)
				ret2 = strstr(buff, gSearchFixPattern[0]);
			if(ret2 != NULL)
				count = GetRightNumber(ret2, getVal);

			if(count ==3)
			{
				len = sprintf(outBuff, "%40s %6u %8u %12u %12u\r", gSensorTextTable[getVal[0]], getVal[0], getVal[1],(unsigned int)(getVal[2] * 1000), lineCount);
			}	
			else if(count ==4)
			{
				len = sprintf(outBuff, "%40s %6u %8u %12u %12u\r", gSensorTextTable[getVal[0]], getVal[0], getVal[1],(unsigned int)(getVal[3] * 1000), lineCount);	
			}
			if(len)
				fwrite(outBuff, 1, len, fpWrite);
		}
		if(len == 0)
		{
			ret = ret2 = NULL;
			ret = strstr(buff, "_Fuser_Action_ISR_ADC_Temp");
			if(ret != NULL)
				ret2 = strstr(buff, "gFuserTargetTemp");

			if(ret != NULL && ret2 != NULL)
				count = GetRightNumber(ret, getVal);

			if(count == 13)
			{				
				unsigned int range;
				if(curTime == 0)
				{
					curTime = getVal[12];
				}
				else
				{
					range = GetMsTimeFromStart(curTime, getVal[12]);
					if(range > maxTime)
					{
						if(range > 110)
							printf("line_%08d: last=%u ms now=%u ms range=%u ms\n", lineCount, curTime, getVal[12], range);
						maxTime = range;						
					}
					curTime = getVal[12];
				}
				len = sprintf(outBuff, "%40s %6u %8u %12u %12u\r", gTempTextTable[1], 1, getVal[1], (unsigned int)(getVal[12] * 1000), lineCount);
				fwrite(outBuff, 1, len, fpWrite);
				len = sprintf(outBuff, "%40s %6u %8u %12u %12u\r", gTempTextTable[2], 2, getVal[3],(unsigned int)(getVal[12] * 1000), lineCount);
				fwrite(outBuff, 1, len, fpWrite);
				len = sprintf(outBuff, "%40s %6u %8u %12u %12u\r", gTempTextTable[3], 3, getVal[5],(unsigned int)(getVal[12] * 1000), lineCount);
				fwrite(outBuff, 1, len, fpWrite);
				len = sprintf(outBuff, "%40s %6u %8u %12u %12u\r", gTempTextTable[4], 4, getVal[11],(unsigned int)(getVal[12] * 1000), lineCount);
				fwrite(outBuff, 1, len, fpWrite);
			}	
		}

		if(len == 0)
		{
			ret = NULL;
			ret = strstr(buff, "IO_CtrlTable");
			if(ret != NULL)			
				count = GetRightNumber(ret, getVal);
			if(count == 3)
			{
				len = sprintf(outBuff, "%40s %6u %8u %12u %12u\r", gIoTextTable[getVal[1]], getVal[1], getVal[2],(unsigned int)(getVal[0] * 1000), lineCount);
				fwrite(outBuff, 1, len, fpWrite);				
			}
		}
	}

exit_read_log:
	if(fp)
		fclose(fp);

	if(fpWrite)
		fclose(fpWrite);

    return retAp;
}

