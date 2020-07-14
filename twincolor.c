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
	"Hiv2ChargerError",
	"SNBC_OutSide_Error",
	"REG_RED",
	"T2_Detect",
	"PaperDetect1",
	"PaperDetect2",
	"MainMotRdy",
	"Main2MotRdy"
};

char *gTempTextTable[] = {
	"A3_TEMP",
	"A4_TEMP",
    "A4_Out_TEMP",
    "Fuser_Env_TEMP",
    "Target_TEMP"    
};

char *gIoTextTable[] = {
    "IR_SENSOR_POWER_EN",
	"SNBC_PICKUP",
	"SENSOR_POWER_EN",
	"HV_DEV_REV",
	"HV_TRANSREV",
    /* PIOB */
    "V24_EN",
    "FUSER_AC_POWER_SW",
    "HV_CHG",
    "HV_DEV",
    "HV_GRID",
	"HV2_CHG",
	"HV_TRANS",
	"HV2_GRID",
	"HV2_TRANS",
	"T2_DOWN_FW",
	"FW_RESET",
	/* PIOC */
	"T2_FW_RESET",
	"FW_RESET2",
	"LPH_SELECT",
	"PAPER_SHIFT_MOTOR_EN",
	"MAIN2_MOTOR_EN",
	"MAIN2_MOTOR_BRAKE",
	"MAIN_MOTOR_BRAKE",
	"MAIN_MOTOR_EN",
	"LAMP_SW_SIDE",
	"MAIN2_MOTOR_DIR",
	"MANUAL_PICKUP_CLUTCH",
	/* PIOD */
	"DESKEW_CLUTCH",
	"PICKUP_CLUTCH",
	"PAPER_SHIFT_MOTOR_DIR",
	"LAMP_SW_CENTER",
	"SNBC_JAM_ERROR",
	/* PIOE */
	"HV2_TRANSREV",
	"HV2_DEV",
	"HV2_DEV_REV",
	"V24_CONTROL"
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
			else if(count ==4 || count ==5)
			{
				if(count ==4)
					len = sprintf(outBuff, "%40s %6u %8u %12u %12u\r", gSensorTextTable[getVal[0]], getVal[0], getVal[1],(unsigned int)(getVal[3] * 1000), lineCount);	
				else	
					len = sprintf(outBuff, "%40s %6u %8u %12u %12u\r", gSensorTextTable[getVal[0]], getVal[0], getVal[1],(unsigned int)(getVal[4]), lineCount);	
			}
			if(len)
				fwrite(outBuff, 1, len, fpWrite);
		}
		
		if(len == 0)
		{
			ret = ret2 = NULL;
			ret = strstr(buff, "_Fuser_Action_ISR_ADC_Temp");
			//if(ret != NULL)
			//	ret2 = strstr(buff, "gFuserTargetTemp");

			//if(ret != NULL && ret2 != NULL)
			if(ret != NULL)
				count = GetRightNumber(ret, getVal);

			if(count == 10)
			{
				unsigned int range;
				if(curTime == 0)
				{
					curTime = getVal[9];
				}
				else
				{
					range = GetMsTimeFromStart(curTime, getVal[9]);
					if(range > maxTime)
					{
						if(range > 110)
							printf("line_%08d: last=%u ms now=%u ms range=%u ms\n", lineCount, curTime, getVal[12], range);
						maxTime = range;						
					}
					curTime = getVal[9];
				}
				len = sprintf(outBuff, "%40s %6u %8u %12u %12u\r", gTempTextTable[1], 1, getVal[1],(unsigned int)(getVal[9] * 1000), lineCount);
				fwrite(outBuff, 1, len, fpWrite);
				len = sprintf(outBuff, "%40s %6u %8u %12u %12u\r", gTempTextTable[2], 2, getVal[3],(unsigned int)(getVal[9] * 1000), lineCount);
				fwrite(outBuff, 1, len, fpWrite);
				len = sprintf(outBuff, "%40s %6u %8u %12u %12u\r", gTempTextTable[3], 3, getVal[4],(unsigned int)(getVal[9] * 1000), lineCount);
				fwrite(outBuff, 1, len, fpWrite);
				len = sprintf(outBuff, "%40s %6u %8u %12u %12u\r", gTempTextTable[4], 4, getVal[6],(unsigned int)(getVal[9] * 1000), lineCount);
				fwrite(outBuff, 1, len, fpWrite);
			}				
		}
		#if 1
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
		#endif
	}

exit_read_log:
	if(fp)
		fclose(fp);

	if(fpWrite)
		fclose(fpWrite);

    return retAp;
}

