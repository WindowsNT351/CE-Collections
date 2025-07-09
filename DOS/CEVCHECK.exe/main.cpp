/*CEVCHECK.EXE for CECollections 2.02*/
/*Copyright 351Workshop 2022-2025*/
/*Build on Tubro C++*/
/*Run on x86 DOS*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char **argv)
{
	if (argc == 1)
	{
		printf("\nWindows CE video & CPU support checking program\n\n");
		printf("Usage: cevcheck [CEVER]\n");
		printf("CEVER sample: 211STD\n\n");

		printf("Return: NormolCPUSpt*100 + LegacyDisp*10 + InverDisp\n\n");
		printf("Last build:ver.202B102 Copyright 351Workshop 2022-2025\n");
		return -1;
	}

	if(!(argv[1][0] >= '0' && argv[1][0] <= '9'))
		return 200;
	char CEID[10] = {0};
	strcpy(CEID, argv[1]);

	char CEVer[10] = { 0 };
	strncpy(CEVer, CEID, 3);
	if (CEVer[2] == 'x')
		CEVer[2] = '0';
	char CEEdion[10] = { 0 };
	strncpy(CEEdion, CEID + 3, 3);

	int currentCPUSupport = 1;
	int currentVESASupport = 1;
	int currentHalfSupport = 0;

	int iCEVer = atoi(CEVer);
	if (iCEVer >= 400)
		currentVESASupport = 1;
	else
		currentVESASupport = 0;

	if (iCEVer <= 210)
		currentCPUSupport=0;
	else
		currentCPUSupport=1;

	if ((CEEdion[0] == 'S' && CEEdion[1] == 'P' && CEEdion[2] == 0) || (CEEdion[0] == 'P' && CEEdion[1] == 'P' && CEEdion[2] == 'C' && CEEdion[3] == 0))
		currentHalfSupport = 1;
	else
		currentHalfSupport = 0;

	printf("%d",currentCPUSupport * 100 + currentVESASupport * 10 + currentHalfSupport);
	return currentCPUSupport * 100 + currentVESASupport * 10 + currentHalfSupport;
}