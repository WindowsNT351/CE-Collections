/*CEVCHECK.EXE for CECollections P2.0 2.01*/
/*Copyright 351Workshop 2022-2024*/
/*Build on Tubro C++*/
/*Run on x86 DOS*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char **argv)
{
	if (argc == 1)
	{
		printf("\nWindows CE video support checking program\n\n");
		printf("Usage: cevcheck [CEVER]\n");
		printf("CEVER sample: 211STD\n\n");

		printf("Return: VESASupport*10+MobileSupport\n\n");
		printf("Last build:ver.201B101 Copyright 351Workshop 2022-2024\n");
		return -1;
	}

	if(!(argv[1][0] >= '0' && argv[1][0] <= '9'))
		return 100;
	char CEID[10] = {0};
	strcpy(CEID, argv[1]);

	char CEVer[10] = { 0 };
	strncpy(CEVer, CEID, 3);
	if (CEVer[2] == 'x')
		CEVer[2] = '0';
	char CEEdion[10] = { 0 };
	strncpy(CEEdion, CEID + 3, 3);

	int currentVESASupport = 1;
	int currentHalfSupport = 0;

	int iCEVer = atoi(CEVer);
	if (iCEVer >= 400)
		currentVESASupport = 1;
	else
		currentVESASupport = 0;

	if ((CEEdion[0] == 'S' && CEEdion[1] == 'P' && CEEdion[2] == 0) || (CEEdion[0] == 'P' && CEEdion[1] == 'P' && CEEdion[2] == 'C' && CEEdion[3] == 0))
		currentHalfSupport = 1;
	else
		currentHalfSupport = 0;

	return currentVESASupport * 10 + currentHalfSupport;
}