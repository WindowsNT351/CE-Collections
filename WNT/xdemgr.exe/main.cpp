#include <stdlib.h>
#include <stdio.h>
#include <afxwin.h>
#include <tlhelp32.h>
#include <conio.h>
#include <iostream>
using namespace std;

int fileCopy(char *in, char *out)
{
	if (!CopyFile(in, out, FALSE))
		return 0;
	SetFileAttributes(out, FILE_ATTRIBUTE_NORMAL); //去掉文件属性
	return 1;
}

typedef BOOL(WINAPI *LPFN_ISWOW64PROCESS) (HANDLE, PBOOL);
LPFN_ISWOW64PROCESS fnIsWow64Process;
BOOL IsWow64()
{
	BOOL bIsWow64 = FALSE;
	//IsWow64Process is not available on all supported versions of Windows.
	//Use GetModuleHandle to get a handle to the DLL that contains the function
	//and GetProcAddress to get a pointer to the function if available.
	fnIsWow64Process = (LPFN_ISWOW64PROCESS)GetProcAddress(
		GetModuleHandle(TEXT("kernel32")), "IsWow64Process");
	if (NULL != fnIsWow64Process)
	{
		if (!fnIsWow64Process(GetCurrentProcess(), &bIsWow64))
		{
			return TRUE;
		}
	}
	return bIsWow64;
}

BOOL LoadNTDriver(char* lpszDriverName, char* lpszDriverPath) {
	char szDriverImagePath[256];
	GetFullPathName(lpszDriverPath, 256, szDriverImagePath, NULL);
	BOOL bRet = FALSE;
	SC_HANDLE hServiceMgr = NULL;
	SC_HANDLE hServiceDDK = NULL;

	hServiceMgr = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
	if (hServiceMgr == NULL) {
		printf("OpenSCManager() Failed %d!\n", GetLastError());
		return FALSE;
	}

	hServiceDDK = CreateService(hServiceMgr, lpszDriverName, lpszDriverName, SERVICE_ALL_ACCESS, SERVICE_KERNEL_DRIVER, SERVICE_DEMAND_START, SERVICE_ERROR_NORMAL, szDriverImagePath, NULL, NULL, NULL, NULL, NULL);
	if (hServiceDDK == NULL) {
		DWORD dwRtn = GetLastError();
		if (dwRtn != ERROR_IO_PENDING && dwRtn != ERROR_SERVICE_EXISTS) {
			printf("CreateService() Failed %d!\n", dwRtn);
			CloseServiceHandle(hServiceMgr);
			return FALSE;
		}
		hServiceDDK = OpenService(hServiceMgr, lpszDriverName, SERVICE_ALL_ACCESS);
		if (hServiceDDK == NULL) {
			printf("OpenService() Failed %d!\n", GetLastError());
			CloseServiceHandle(hServiceMgr);
			return FALSE;
		}
	}

	bRet = StartService(hServiceDDK, NULL, NULL);
	if (!bRet) {
		DWORD dwRtn = GetLastError();
		if (dwRtn != ERROR_IO_PENDING && dwRtn != ERROR_SERVICE_ALREADY_RUNNING) {
			printf("StartService() Failed %d!\n", dwRtn);
			CloseServiceHandle(hServiceDDK);
			CloseServiceHandle(hServiceMgr);
			return FALSE;
		}
	}

	CloseServiceHandle(hServiceDDK);
	CloseServiceHandle(hServiceMgr);
	return TRUE;
}

// 卸载驱动程序
BOOL UnloadNTDriver(char* szSvrName) {
	BOOL bRet = FALSE;
	SC_HANDLE hServiceMgr = NULL;
	SC_HANDLE hServiceDDK = NULL;
	SERVICE_STATUS SvrSta;

	hServiceMgr = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
	if (hServiceMgr == NULL) {
		printf("OpenSCManager() Failed %d!\n", GetLastError());
		return FALSE;
	}

	hServiceDDK = OpenService(hServiceMgr, szSvrName, SERVICE_ALL_ACCESS);
	if (hServiceDDK == NULL) {
		printf("OpenService() Failed %d!\n", GetLastError());
		CloseServiceHandle(hServiceMgr);
		return FALSE;
	}

	if (!ControlService(hServiceDDK, SERVICE_CONTROL_STOP, &SvrSta)) {
		printf("ControlService() Failed %d!\n", GetLastError());
	}

	if (!DeleteService(hServiceDDK)) {
		printf("DeleteService() Failed %d!\n", GetLastError());
	}

	CloseServiceHandle(hServiceDDK);
	CloseServiceHandle(hServiceMgr);
	return TRUE;
}

int IsWindows10OrGreater()
{
	std::string vname;
	typedef void(__stdcall*NTPROC)(DWORD*, DWORD*, DWORD*);
	HINSTANCE hinst = LoadLibrary("ntdll.dll");
	DWORD dwMajor, dwMinor, dwBuildNumber;
	NTPROC proc = (NTPROC)GetProcAddress(hinst, "RtlGetNtVersionNumbers");
	proc(&dwMajor, &dwMinor, &dwBuildNumber);
	if (dwMajor >= 10)
		return 1;
	return 0;
}

int findProcessByName(char *processName) {
	// 创建进程快照
	HANDLE hSnapShot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	PROCESSENTRY32 pe;
	int flag = 0;

	if (hSnapShot == INVALID_HANDLE_VALUE) {
		return false;
	}

	pe.dwSize = sizeof(PROCESSENTRY32);

	// 遍历进程列表
	if (Process32First(hSnapShot, &pe)) {
		do {
			// 检查进程名称
			if (_stricmp(pe.szExeFile, processName) == 0) {
				HANDLE hProcess = OpenProcess(PROCESS_TERMINATE, FALSE, pe.th32ProcessID);

				if (hProcess != NULL) {
					flag++;
				}
			}
		} while (Process32Next(hSnapShot, &pe));
	}

	CloseHandle(hSnapShot);
	return flag;
}

DWORD WINAPI dispWind_cantStart(LPVOID lpParam)
{
	MessageBox(NULL, "You can't start emulator due to:\r\n    - You are running another XDE Emulator.\r\n    - Your OS is not the required version for the emulator.", "CE-Collections", MB_OK | MB_ICONHAND);
	return 0;
}

int main(int argc, char **argv)
{
	if (argc == 1)
	{
		puts("XDE 1.x Emulator Starter for CE-Collections 2.02 - 351Workshop 2025");
		puts("Usage: Xdemgr <Emulator version> <Path to NK.bin>");
		puts("    <Emulator version>:");
		puts("      0: XDE 1.0");
		puts("      2: XDE \"1.2\"");
		puts("Sample: Xdemgr 0 700WP.bin");
		puts("Copyright 351Workshop 2025 - Last built: with CECv202B105");
		return 1;
	}
	else if (argc != 3)
	{
		puts("Error - Wrong command");
		return 1;
	}

	if ((IsWindows10OrGreater() == 1) || (findProcessByName("Xdemgr.exe") > 1))
	{
		printf("Info - Can't start emulator due to OS version too high OR you are starting more than one emulator at same time.Press Backspace to skip.\n");
		HANDLE hDispWindCantStart = CreateThread(
			NULL, // 默认安全属性
			0, // 默认堆栈大小
			dispWind_cantStart, // 线程函数
			NULL, // 无参数传递
			0, // 立即执行线程
			NULL // 不接收线程ID
		);
		DWORD flag = 0;
		while (1)
		{
			if (_kbhit() && _getche() == '\b')
				break;
			GetExitCodeThread(hDispWindCantStart, &flag);
			if (flag != STILL_ACTIVE)
			{
				return 1;
			}
		}
	}

	int emulVer = atoi(argv[1]);
	char imgPath[255] = { 0 };
	char emulDir[255] = { 0 };
	char emulDirBase[255] = { 0 };
	char vmmPath[255] = { 0 };
	char vmmOutPath[255] = { 0 };
	char cfgPath[255] = { 0 };
	char cfgOutPath[255] = { 0 };
	char cfgName[255] = { 0 };
	char xdePath[255] = { 0 };
	char xdeCmd[255] = { 0 };
	strcpy_s(imgPath, argv[2]);
	GetPrivateProfileString("xdemgr", "emulDirBase", ".\\xde1", emulDirBase, 255, ".\\xdemgr.ini");
	sprintf_s(emulDir, "%s%d\\", emulDirBase, emulVer);
	GetPrivateProfileString("xdemgr", "cfgName", "config_board0.decfg", cfgName, 255, ".\\xdemgr.ini");
	sprintf_s(cfgPath, "%s\\%s", emulDir, cfgName);
	if (GetTempPath(255, cfgOutPath) == 0)
	{
		puts("Error - Can't get TEMP path");
		return 1;
	}
	strcat_s(cfgOutPath, "\\config_board.decfg");
	GetCurrentDirectory(255, xdePath);
	sprintf_s(xdePath, "%s\\%s\\XDE_.exe", xdePath, emulDir);
	if (GetTempPath(255, vmmOutPath) == 0)
	{
		puts("Error - Can't get TEMP path");
		return 1;
	}
	strcat_s(vmmOutPath, "\\VMM.sys");//sprintf_s(vmmOutPath, "%s\\VMM.sys", vmmOutPath);
	sprintf_s(xdeCmd, "/Vmid {5B40E2C9-24EA-447A-9B7D-9B9688325384} /language 1033 /decfg %s %s", cfgOutPath, imgPath);
	if (IsWow64())
		sprintf_s(vmmPath, "%s\\VMM64.sys", emulDir);
	else
		sprintf_s(vmmPath, "%s\\VMM.sys", emulDir);

	printf("================Starting XDE Emulator===================\n");
	printf("emulVer......................:%d\n", emulVer);
	printf("imgPath......................:%s\n", imgPath);
	printf("emulDir......................:%s\n", emulDir);
	printf("emulDirBase..................:%s\n", emulDirBase);
	printf("vmmPath......................:%s\n", vmmPath);
	printf("vmmOutPath...................:%s\n", vmmOutPath);
	printf("cfgName......................:%s\n", cfgName);
	printf("cfgPath......................:%s\n", cfgPath);
	printf("cfgOutPath...................:%s\n", cfgOutPath);
	printf("xdePath......................:%s\n", xdePath);
	printf("xdeCmd.......................:%s\n", xdeCmd);

	if (!fileCopy(vmmPath, vmmOutPath))
	{
		puts("ERROR - Can't copy VMM.sys");
		return 1;
	}
	if (!fileCopy(cfgPath, cfgOutPath))
	{
		puts("Waring - Can't copy %%CFG%%");
	}
	Sleep(100);
	int isLoadedDriver = 1;
	if (!LoadNTDriver("VMM", vmmOutPath))
	{
		puts("Warning - Can't load VMM driver");
		isLoadedDriver = 0;
	}

	SHELLEXECUTEINFO ShExecInfo = { 0 };
	ShExecInfo.cbSize = sizeof(SHELLEXECUTEINFO);
	ShExecInfo.fMask = SEE_MASK_NOCLOSEPROCESS;
	ShExecInfo.hwnd = NULL;
	ShExecInfo.lpVerb = NULL;
	ShExecInfo.lpFile = xdePath;
	ShExecInfo.lpParameters = xdeCmd;
	ShExecInfo.lpDirectory = emulDir;
	ShExecInfo.nShow = SW_SHOWNORMAL;
	ShExecInfo.hInstApp = NULL;
	ShellExecuteEx(&ShExecInfo);
	WaitForSingleObject(ShExecInfo.hProcess, INFINITE);

	Sleep(100);
	printf("\n");
	if (isLoadedDriver == 1)
	{
		if (!UnloadNTDriver("VMM"))
		{
			puts("ERROR - Can't unload VMM driver");
			return 1;
		}
	}
	if (!DeleteFile(vmmOutPath))
		printf("Warning - Can't delete VMM.sys\n");
	if (!DeleteFile(cfgOutPath))
		printf("Warning - Can't delete %%CFG%%\n");
	printf("All TEMP removed\n");
	return 0;
}