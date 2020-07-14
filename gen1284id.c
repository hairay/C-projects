#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef unsigned short	WORD;
typedef unsigned char	BYTE;
typedef unsigned char *	PBYTE;
typedef unsigned int	ULONG;
typedef unsigned int  *	PULONG;


#define MAX_DEVNODE_NAME_ROOT 20
#define _16_BIT_CHECKSUM 5
WORD wCRC16a[16]={
	0000000, 0140301, 0140601, 0000500,
	0141401, 0001700, 0001200, 0141101,
	0143001, 0003300, 0003600, 0143501,
	0002400, 0142701, 0142201, 0002100,
};
WORD wCRC16b[16]={
	0000000, 0146001, 0154001, 0012000,
	0170001, 0036000, 0024000, 0162001,
	0120001, 0066000, 0074000, 0132001,
	0050000, 0116001, 0104001, 0043000,
};

void ChangeSpaceToUnderLine(char *pStr)
{
	int i, len;

	len = strlen(pStr);
	for(i=0; i<len; i++)
		if(pStr[i] == ' ')
			pStr[i] = '_';
}

void ChangeUnderLineToSpace(char *pStr)
{
	int i, len;

	len = strlen(pStr);
	for(i=0; i<len; i++)
		if(pStr[i] == '_')
			pStr[i] = ' ';
}

void BuildP1284PnPID(char *lpMFG, char *lpMDL)
{
	WORD wCheckSum;
	char szCheckSum[_16_BIT_CHECKSUM];
	char szTemp[128], szTemp2[128];
	WORD wTempSize;	
	// The following are used to generate checksum values
	PBYTE pb;
	BYTE bTmp;
	ULONG ulSize, ulSeed;
	PULONG pulSeed;

	
	// N: Concatenate the MFG and MDL fields
	wTempSize = (WORD) strlen(lpMFG) + (WORD) strlen(lpMDL)+1;
	// Concatenate the provided MFG and MDL P1284 fields
	strcpy(szTemp, lpMFG);
	strcat(szTemp, " ");
	strcat(szTemp, lpMDL);
	ulSize=strlen(szTemp);

	printf("CompatibleID\n");
	sprintf(szTemp2, "1284 CID %s", szTemp);
	ChangeSpaceToUnderLine(szTemp2);	
	printf(szTemp2);
    printf("\n");
	// Checksum the string
	// start a seed at 0, and create a pointer to the seed. After the
	// checksum, convert the checksum to a word.
	ulSeed = 0;
	pulSeed = &ulSeed;
	for (pb=(BYTE *)szTemp; ulSize; ulSize--, pb++)
	{
		bTmp=(BYTE)(((WORD)*pb)^((WORD)*pulSeed)); // Xor CRC with new char
		*pulSeed=((*pulSeed)>>8) ^ wCRC16a[bTmp&0x0F] ^ wCRC16b[bTmp>>4];
	}
	wCheckSum=(WORD)(*pulSeed);
	sprintf(szCheckSum, "%04X",wCheckSum);	
	// Form the Dev Node ID.
	if (wTempSize > MAX_DEVNODE_NAME_ROOT)
		szTemp[MAX_DEVNODE_NAME_ROOT] = '\0';
	strcat(szTemp, szCheckSum);
	// Got to remove spaces from lpTemp, Just replace them with Underscores
    printf("HardwareID\n");
	ChangeSpaceToUnderLine(szTemp);	
    printf(szTemp);
    printf("\n");	
}

int main(int argc, char **argv)
{
    if(argc != 3)
    {
        printf("gen1284id vendor_name model_name\n");
        return -1;
    }
    ChangeUnderLineToSpace(argv[1]);
    ChangeUnderLineToSpace(argv[2]);
    BuildP1284PnPID(argv[1], argv[2]);
    return 0;
}

