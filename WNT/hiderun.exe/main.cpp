#include <stdlib.h>
#include <stdio.h>
#include <afxwin.h>
#include <iostream>
using namespace std;
#pragma comment( linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"" )

int main(int argc, char **argv)
{
	if(argc<=1)
		return 1;
	char batBuffer[120] = { 0 };
	for (int i = 0; i < argc - 2; i++)
	{
		strcat_s(batBuffer, argv[i + 2]);
		strcat_s(batBuffer, " ");
	}
	cout << batBuffer << endl;
	ShellExecute(NULL, "open", argv[1], batBuffer, NULL, SW_HIDE);
	return 0;
}