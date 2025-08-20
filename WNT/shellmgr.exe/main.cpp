//Build on VS 2005
//For NT4.0-Win11

#include <stdlib.h>
#include <stdio.h>
#include <afxwin.h>
#include <psapi.h>
#include <tlhelp32.h>
#include <conio.h>
#include <vector>
#include <iostream>
#include <io.h>
#include <string>
#pragma comment(lib, "psapi.lib")
using namespace std;

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

BOOL KillNTDriver(char* szSvrName) {
	BOOL bRet = FALSE;
	SC_HANDLE hServiceMgr = NULL;
	SC_HANDLE hServiceDDK = NULL;
	SERVICE_STATUS SvrSta;

	hServiceMgr = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
	if (hServiceMgr == NULL) {
		//printf("OpenSCManager() Failed %d!\n", GetLastError());
		return FALSE;
	}

	hServiceDDK = OpenService(hServiceMgr, szSvrName, SERVICE_ALL_ACCESS);
	if (hServiceDDK == NULL) {
		//printf("OpenService() Failed %d!\n", GetLastError());
		CloseServiceHandle(hServiceMgr);
		return FALSE;
	}

	if (!ControlService(hServiceDDK, SERVICE_CONTROL_STOP, &SvrSta)) {
		//printf("ControlService() Failed %d!\n", GetLastError());
	}


	CloseServiceHandle(hServiceDDK);
	CloseServiceHandle(hServiceMgr);
	return TRUE;
}

bool DeleteDirectory(CString DirName)
{
	CString PUBPATH;
	PUBPATH = DirName;
	CFileFind tempFind;
	DirName += "\\*.*";
	BOOL IsFinded = (BOOL)tempFind.FindFile(DirName);
	//cout << IsFinded <<endl;
	while (IsFinded)
	{
		IsFinded = (BOOL)tempFind.FindNextFile();
		if (!tempFind.IsDots())
		{
			CString strDirName;
			strDirName += PUBPATH;
			strDirName += "\\";
			strDirName += tempFind.GetFileName();
			if (tempFind.IsDirectory())
			{
				DeleteDirectory(strDirName);
			}
			else
			{
				SetFileAttributes(strDirName, FILE_ATTRIBUTE_NORMAL); //去掉文件的系统和隐藏属性
				DeleteFile(strDirName);
			}
		}
	}
	tempFind.Close();
	if (!RemoveDirectory(PUBPATH))
	{
		return false;
	}

	return true;
}

bool NormalDirectory(CString DirName)
{
	CString PUBPATH;
	PUBPATH = DirName;
	CFileFind tempFind;
	DirName += "\\*.*";
	BOOL IsFinded = (BOOL)tempFind.FindFile(DirName);
	//cout << IsFinded <<endl;
	while (IsFinded)
	{
		IsFinded = (BOOL)tempFind.FindNextFile();
		if (!tempFind.IsDots())
		{
			CString strDirName;
			strDirName += PUBPATH;
			strDirName += "\\";
			strDirName += tempFind.GetFileName();
			if (tempFind.IsDirectory())
			{
				NormalDirectory(strDirName);
			}
			else
			{
				SetFileAttributes(strDirName, FILE_ATTRIBUTE_NORMAL); //去掉文件的系统和隐藏属性
			}
		}
		if (IsFinded == 0)
			break;
	}
	tempFind.Close();
	/*if (!NormalDirectory(PUBPATH))
	{
		return false;
	}*/
	return true;
}

BOOL CopyDir(const char* srcDir, const char* destDir) {
	char cmd[255]={0};
	sprintf_s(cmd,"xcopy %s %s /E /I >> nul",srcDir,destDir);
	system(cmd);
	/*sprintf_s(cmd,"xcopy %s %s /Y /E /I >> nul",srcDir,destDir);
	system(cmd);*/
    return TRUE;
}

inline char* stristr(char *str1, char *str2)
{
	char str1A[255] = { 0 };
	char str2A[255] = { 0 };
	for (int i = 0; i < strlen(str1); i++) 
		str1A[i] = tolower(str1[i]);
	for (int i = 0; i < strlen(str2); i++)
		str2A[i] = tolower(str2[i]);
	
	return strstr(str1A, str2A);
}

int KillProcessByName(char *processName) {
	DWORD dwProcessID[0x500] = { 0 };  //开始的预先分配较大的缓冲区，用来存放进程ID
	DWORD dwNeeded = 0;
	BOOL bEnumRes = EnumProcesses(dwProcessID, sizeof(dwProcessID), &dwNeeded);
	UINT uCount = dwNeeded / sizeof(DWORD);//获得枚举到进程的数量
	for (UINT i = 0; i < uCount; i++)
	{
		//只对进程进程枚举，所以申请QUERY权限，具体还得根据应用申请权限
		HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwProcessID[i]);
		if (NULL != hProcess)
		{
			CHAR szProcessName[255] = { 0 };
			DWORD dwNameLen = 255;
			//int bRet = GetProcessImageFileName(hProcess, szProcessName, dwNameLen);
			int bRet = GetModuleFileNameEx(hProcess, NULL, szProcessName, dwNameLen);
			if (bRet != 0)
			{
				if (stristr(szProcessName, processName) != 0)
				{
					TerminateProcess(hProcess, 0);
					CloseHandle(hProcess);
					return true;
				}
			}
		}
	}
	return false;
}

int findProcessByName(char *processName) {
	int flag = 0;
	DWORD dwProcessID[0x500] = { 0 };  //开始的预先分配较大的缓冲区，用来存放进程ID
	DWORD dwNeeded = 0;
	BOOL bEnumRes = EnumProcesses(dwProcessID, sizeof(dwProcessID), &dwNeeded);
	UINT uCount = dwNeeded / sizeof(DWORD);//获得枚举到进程的数量
	for (UINT i = 0; i < uCount; i++)
	{
		//只对进程进程枚举，所以申请QUERY权限，具体还得根据应用申请权限
		HANDLE hProcess = OpenProcess(/*PROCESS_QUERY_INFORMATION*/PROCESS_ALL_ACCESS, FALSE, dwProcessID[i]);
		if (NULL != hProcess)
		{
			CHAR szProcessName[255] = { 0 };
			DWORD dwNameLen = 255;
			//int bRet = GetProcessImageFileName(hProcess, szProcessName, dwNameLen);
			int bRet = GetModuleFileNameEx(hProcess, NULL, szProcessName, dwNameLen);
			if (bRet != 0)
			{
				if (stristr(szProcessName, processName) != 0)
				{
					flag++;
				}
				//printf("ID:%4d\tprocessName(%s)\n", dwProcessID[i], szProcessName);
			}
			//printf("ID:%4d\tprocessName(%s)\n", dwProcessID[i], szProcessName);
		}
	}
	return flag;
}

/*int delReg(char *path)
{
	char fatKey[255] = { 0 };
	char subKey2[255] = { 0 };
	char subKey[255] = { 0 };
	int j = 0;
	for (int i = strlen(path) - 1; i >= 0; i--)
	{
		if (path[i] == '\\')
		{
			path[i] = '\0';
			break;
		}
		else
		{
			subKey2[j] = path[i];
			j++;
		}
	}
	for (int i = 0; i < strlen(subKey2); i++)
	{
		j--;
		subKey[i] = subKey2[j];
	}
	j = 0;
	for (int i = 0; i < strlen(path); i++)
	{
		if (path[i] == '\\')
		{
			i++;
			for (int k = i; k < strlen(path); k++)
			{
				fatKey[j] = path[k];
				j++;
			}
			break;
		}
	}
	printf("fat:%s    sub:%s\n", fatKey, subKey);
	HKEY hKey;
	if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, fatKey, 0, KEY_WRITE, &hKey) != ERROR_SUCCESS)
	{
		return 0;
	}
	if (RegDeleteKey(hKey, subKey) != ERROR_SUCCESS)
	{
		return 0;
	}
	return 1;
}*/

DWORD WINAPI dispWind_cantStart(LPVOID lpParam)
{
	MessageBox(NULL, "You can't start emulator due to:\r\n    - You are running another Shell Emulator.\r\n    - Your OS is not the required version for the emulator.", "CE-Collections", MB_OK | MB_ICONHAND);
	return 0;
}

DWORD WINAPI dispWind_starting(LPVOID lpParam)
{
	while (1)
	{
		MessageBox(NULL, "Please wait while emulator is starting.", "CE-Collections", MB_OK);
	}
	return 0;
}

DWORD WINAPI APC10_killApchook(LPVOID lpParam)
{
	while (1)
	{
		Sleep(100);
		if (KillNTDriver("apchook"))
		{
			//printf("Kill apchook SUCCESSFULY!\n");
		}
	}
	return 0;
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

void afterworks(char *outDir)
{
	printf("\n");
	Sleep(300);
	if (!KillProcessByName("FILESYS.EXE"))
		puts("Warning - Can't stop FILESYS.exe");
	Sleep(100);
	if (!KillProcessByName("DEVICE.EXE"))
		puts("Warning - Can't stop DEVICE.exe");
	Sleep(100);
	if (!KillProcessByName("AADESK.EXE"))
		puts("Warning - Can't stop AADESK.exe");
	Sleep(100);
	if (!DeleteDirectory(outDir))
		puts("Warning - Can't delete CESHLTMP");
	Sleep(100);
	printf("All TEMP files removed.\n");
	exit(0);
	return;
}

int IsWindowsVistaOrGreater()
{
	SYSTEM_INFO info;                                   //用SYSTEM_INFO结构判断64位AMD处理器
    GetSystemInfo(&info);                               //调用GetSystemInfo函数填充结构
    OSVERSIONINFOEX os;
    os.dwOSVersionInfoSize=sizeof(OSVERSIONINFOEX); 
    if(GetVersionEx((OSVERSIONINFO *)&os))                  
    {
		if (os.dwMajorVersion >= 6)
			return 1;
	}
	return 0;
}

int main(int argc, char **argv)
{
	if (argc == 1)
	{
		puts("Windows CE Shell Emulator Starter for CE-Collections 2.02 - 351Workshop 2025");
		puts("Usage: Shellmgr <Start type> <Path to EMUL> <shell32.exe name>");
		puts("    <Start type>:");
		puts("      1: Standard 1.x series");
		puts("      2: Standard 2.x series");
		puts("      3: PocketPC 2000");
		puts("      4: Auto PC 1.0");
		puts("Sample: Shellmgr 1 .\\10xSTD\\ SHELL32.exe");
		puts("        Shellmgr 2 .\\200HPC\\ explorem.exe");
		puts("Copyright 351Workshop 2025 - Last built: with CECv202B105");
		return 1;
	}
	else if (argc != 4)
	{
		puts("Error - Wrong command");
		return 1;
	}

	if (findProcessByName("Shellmgr.exe") > 1)
	{
		printf("Info - Can't start emulator due to another Emulator is running.Press Backspace to skip.\n");
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
			Sleep(10);
		}
		TerminateThread(hDispWindCantStart, 0);
	}

	HANDLE hDispWindStarting = CreateThread(
		NULL, // 默认安全属性
		0, // 默认堆栈大小
		dispWind_starting, // 线程函数
		NULL, // 无参数传递
		0, // 立即执行线程
		NULL // 不接收线程ID
	);


	char srcDir[255] = { 0 };
	char outDir[255] = { 0 };
	char execName[255] = { 0 };
	char execPath[255] = { 0 };
	strcpy_s(srcDir, argv[2]);
	strcpy_s(execName, argv[3]);
	if (GetTempPath(255, outDir) == 0)
	{
		puts("Error - Can't get TEMP path");
		return 1;
	}
	//sprintf_s(outDir, "%sCESHLTMP", outDir);
	strcat_s(outDir, "CESHLTMP");

	printf("====================Prep TEMP files=====================\n");
	printf("srcDir......................:%s\n", srcDir);
	printf("outDir......................:%s\n", outDir);

	if (!DeleteDirectory(outDir))
		puts("Warning - Can't delete CESHLTMP");
	Sleep(50);
	if (CopyDir(srcDir, outDir) == 0)
	{
		puts("ERROR - Can't copy files into CESHLTMP");
		return 1;
	}

	int emulType = atoi(argv[1]);
	if (emulType == 1)
	{
		char shellEnv[255] = { 0 };
		sprintf_s(shellEnv, "PegEmulDir=%s", outDir);
		sprintf_s(execPath, "%s\\Bin\\%s", outDir, execName);
		printf("==============WCE1.0 Mode - Starting Emul===============\n");
		printf("shellEnv....................:%s\n", shellEnv);
		printf("outDir......................:%s\n", outDir);
		printf("execName....................:%s\n", execName);
		printf("execPath....................:%s\n", execPath);
		STARTUPINFO si;
		PROCESS_INFORMATION pi;
		ZeroMemory(&si, sizeof(si));
		si.cb = sizeof(si);
		ZeroMemory(&pi, sizeof(pi));
		if (!CreateProcess(
			NULL, // 不在此指定可执行文件的文件名
			execPath, // 命令行参数
			NULL, // 默认进程安全性
			NULL, // 默认线程安全性
			FALSE, // 指定当前进程内的句柄不可以被子进程继承
			0, // 创建标志
			shellEnv, // 使用本进程的环境变量
			outDir, // 使用本进程的驱动器和目录
			&si, // 指向 STARTUPINFO 结构的指针
			&pi // 指向 PROCESS_INFORMATION 结构的指针
		))
		{
			puts("ERROR - Can't start the emulator");
			afterworks(outDir);
		}
		Sleep(1000);
		TerminateThread(hDispWindStarting, 0);
		WaitForSingleObject(pi.hProcess, INFINITE);
	}
	else if (emulType == 2)
	{
		char regPath[255] = { 0 };
		char regKeyPath[255] = { 0 };
		char regKeyPath64[255] = { 0 };
		char regKeyPathE[255] = { 0 };
		char regKeyPathE64[255] = { 0 };
		char regCmd[255] = { 0 };
		sprintf_s(execPath, "%s\\Windows\\%s", outDir, execName);
		sprintf_s(regPath, "%s\\cei.reg\0", outDir);
		sprintf_s(regCmd, "regedit.exe /s %s\\cei.reg", outDir);
		GetPrivateProfileString("shellmgr", "regKeyPath", "HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows CE Platform SDK\\Emulation\\CESHLTMP", regKeyPath, 255, regPath);
		GetPrivateProfileString("shellmgr", "regKeyPath64", "HKEY_LOCAL_MACHINE\\SOFTWARE\\WOW6432Node\\Microsoft\\Windows CE Platform SDK\\Emulation\\CESHLTMP", regKeyPath64, 255, regPath);
		sprintf_s(regKeyPathE, "%s\\Environment", regKeyPath);
		sprintf_s(regKeyPathE64, "%s\\Environment", regKeyPath64);

		string outDirReg;
		outDirReg += "\"";
		outDirReg += outDir;
		for (int i = 0; i < outDirReg.size(); i++)
		{
			if (outDirReg[i] == '\\')
			{
				outDirReg.replace(i, 1, "\\\\");
				i++;
			}
		}
		outDirReg += "\"";

		printf("==============WCE2.0 Mode - Starting Emul===============\n");
		printf("regPath.....................:%s\n", regPath);
		printf("outDir......................:%s\n", outDir);
		printf("execName....................:%s\n", execName);
		printf("execPath....................:%s\n", execPath);
		printf("regKeyPath..................:%s\n", regKeyPath);
		printf("regKeyPath64................:%s\n", regKeyPath64);
		printf("regKeyPathE.................:%s\n", regKeyPathE);
		printf("regKeyPathE64...............:%s\n", regKeyPathE64);
		printf("regCmd......................:%s\n", regCmd);
		printf("outDirReg...................:%s\n", outDirReg.c_str());
		Sleep(50);
		WritePrivateProfileString(regKeyPathE, "\"Root Directory\"", outDirReg.c_str(), regPath);
		WritePrivateProfileString(regKeyPathE64, "\"Root Directory\"", outDirReg.c_str(), regPath);

		system(regCmd);

		STARTUPINFO si;
		PROCESS_INFORMATION pi;
		ZeroMemory(&si, sizeof(si));
		si.cb = sizeof(si);
		ZeroMemory(&pi, sizeof(pi));
		if (!CreateProcess(
			NULL, // 不在此指定可执行文件的文件名
			execPath, // 命令行参数
			NULL, // 默认进程安全性
			NULL, // 默认线程安全性
			FALSE, // 指定当前进程内的句柄不可以被子进程继承
			0, // 创建标志
			NULL, // 使用本进程的环境变量
			outDir, // 使用本进程的驱动器和目录
			&si, // 指向 STARTUPINFO 结构的指针
			&pi // 指向 PROCESS_INFORMATION 结构的指针
		))
		{
			puts("ERROR - Can't start the emulator");
			afterworks(outDir);
		}
		Sleep(1000);
		TerminateThread(hDispWindStarting, 0);
		WaitForSingleObject(pi.hProcess, INFINITE);
		/*if (!delReg(regKeyPath))
			puts("Warning - Can't remove EMULATION reg key.");
		if(!delReg(regKeyPath64))
			puts("Warning - Can't remove EMULATION64 reg key.");*/
	}
	else if (emulType == 3)
	{
		char regPath[255] = { 0 };
		char regKeyPath[255] = { 0 };
		char regKeyPath64[255] = { 0 };
		char regKeyPathE[255] = { 0 };
		char regKeyPathE64[255] = { 0 };
		char regCmd[255] = { 0 };
		char emulDir[255] = { 0 };
		sprintf_s(execPath, "%s\\palm300\\Windows\\%s", outDir, execName);
		sprintf_s(regPath, "%s\\cei.reg", outDir);
		sprintf_s(regCmd, "%%SystemRoot%%\\regedit.exe /s %s\\cei.reg", outDir);
		GetPrivateProfileString("shellmgr", "regKeyPath", NULL, regKeyPath, 255, regPath);
		GetPrivateProfileString("shellmgr", "regKeyPath64", NULL, regKeyPath64, 255, regPath);
		sprintf_s(regKeyPathE, "%s\\Environment", regKeyPath);
		sprintf_s(regKeyPathE64, "%s\\Environment", regKeyPath64);
		sprintf_s(emulDir, "%s\\palm300\\", outDir);

		string outDirReg;
		outDirReg += "\"";
		outDirReg += outDir;
		outDirReg += "\\palm300\\";
		for (int i = 0; i < outDirReg.size(); i++)
		{
			if (outDirReg[i] == '\\')
			{
				outDirReg.replace(i, 1, "\\\\");
				i++;
			}
		}
		outDirReg += "\"";

		if (IsWindowsVistaOrGreater())
		{
			TerminateThread(hDispWindStarting, 0);
			printf("Info - Can't start emulator due to OS version too high.Press Backspace to skip.\n");
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
					afterworks(outDir);
				}
				Sleep(10);
			}
			TerminateThread(hDispWindCantStart, 0);
		}

		printf("==============Auto10 Mode - Starting Emul===============\n");
		printf("regPath.....................:%s\n", regPath);
		printf("outDir......................:%s\n", outDir);
		printf("emulDir.....................:%s\n", emulDir);
		printf("execName....................:%s\n", execName);
		printf("execPath....................:%s\n", execPath);
		printf("regKeyPath..................:%s\n", regKeyPath);
		printf("regKeyPath64................:%s\n", regKeyPath64);
		printf("regKeyPathE.................:%s\n", regKeyPathE);
		printf("regKeyPathE64...............:%s\n", regKeyPathE64);
		printf("regCmd......................:%s\n", regCmd);
		printf("outDirReg...................:%s\n", outDirReg.c_str());
		Sleep(50);
		WritePrivateProfileString(regKeyPathE, "\"Root Directory\"", outDirReg.c_str(), regPath);
		WritePrivateProfileString(regKeyPathE64, "\"Root Directory\"", outDirReg.c_str(), regPath);

		system(regCmd);

		STARTUPINFO si;
		PROCESS_INFORMATION pi;
		ZeroMemory(&si, sizeof(si));
		si.cb = sizeof(si);
		ZeroMemory(&pi, sizeof(pi));
		if (!CreateProcess(
			NULL, // 不在此指定可执行文件的文件名
			execPath, // 命令行参数
			NULL, // 默认进程安全性
			NULL, // 默认线程安全性
			FALSE, // 指定当前进程内的句柄不可以被子进程继承
			0, // 创建标志
			NULL, // 使用本进程的环境变量
			emulDir, // 使用本进程的驱动器和目录
			&si, // 指向 STARTUPINFO 结构的指针
			&pi // 指向 PROCESS_INFORMATION 结构的指针
		))
		{
			puts("ERROR - Can't start the emulator");
			afterworks(outDir);
		}
		Sleep(1000);
		TerminateThread(hDispWindStarting, 0);
		WaitForSingleObject(pi.hProcess, INFINITE);
		/*if (!delReg(regKeyPath))
			puts("Warning - Can't remove EMULATION reg key.");
		if(!delReg(regKeyPath64))
			puts("Warning - Can't remove EMULATION64 reg key.");*/
	}
	else if (emulType == 4)
	{
		char wcefsPath[255] = { 0 };
		char regPath[255] = { 0 };
		char regKeyPath[255] = { 0 };
		char regKeyPathE[255] = { 0 };
		char regCmd[255] = { 0 };
		sprintf_s(execPath, "%s\\Bin\\%s", outDir, execName);
		sprintf_s(regPath, "%s\\cei.reg", outDir);
		sprintf_s(regCmd, "regedit.exe /s %s\\cei.reg", outDir);
		GetPrivateProfileString("shellmgr", "regKeyPath", NULL, regKeyPath, 255, regPath);
		sprintf_s(regKeyPathE, "%s\\Environment", regKeyPath);
		sprintf_s(wcefsPath, "%s\\bin\\wcefs.sys", outDir);

		string outDirReg;
		outDirReg += "\"";
		outDirReg += outDir;
		for (int i = 0; i < outDirReg.size(); i++)
		{
			if (outDirReg[i] == '\\')
			{
				outDirReg.replace(i, 1, "\\\\");
				i++;
			}
		}
		outDirReg += "\"";

		if (IsWow64())
		{
			TerminateThread(hDispWindStarting, 0);
			printf("Info - Can't start emulator due to the OS isn't I386.Press Backspace to skip.\n");
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
					afterworks(outDir);
				}
				Sleep(10);
			}
			TerminateThread(hDispWindCantStart, 0);
		}

		printf("====================Auto PC 1.0 Mode====================\n");
		printf("wcefsPath...................:%s\n", wcefsPath);
		printf("regPath.....................:%s\n", regPath);
		printf("outDir......................:%s\n", outDir);
		printf("execName....................:%s\n", execName);
		printf("execPath....................:%s\n", execPath);
		printf("regKeyPath..................:%s\n", regKeyPath);
		printf("regKeyPathE.................:%s\n", regKeyPathE);
		printf("regCmd......................:%s\n", regCmd);
		printf("outDirReg...................:%s\n", outDirReg.c_str());

		Sleep(50);
		WritePrivateProfileString(regKeyPathE, "\"Root Directory\"", outDirReg.c_str(), regPath);
		system(regCmd);

		HANDLE hKillApchook = NULL;
		if (IsWindowsVistaOrGreater())
		{
			hKillApchook = CreateThread(
				NULL, // 默认安全属性
				0, // 默认堆栈大小
				APC10_killApchook, // 线程函数
				NULL, // 无参数传递
				0, // 立即执行线程
				NULL // 不接收线程ID
			);
			printf("Info - NT6.x,Started killing APCHOOK driver.\n");
		}

		STARTUPINFO si;
		PROCESS_INFORMATION pi;
		ZeroMemory(&si, sizeof(si));
		si.cb = sizeof(si);
		ZeroMemory(&pi, sizeof(pi));
		if (!CreateProcess(
			NULL, // 不在此指定可执行文件的文件名
			execPath, // 命令行参数
			NULL, // 默认进程安全性
			NULL, // 默认线程安全性
			FALSE, // 指定当前进程内的句柄不可以被子进程继承
			0, // 创建标志
			NULL, // 使用本进程的环境变量
			outDir, // 使用本进程的驱动器和目录
			&si, // 指向 STARTUPINFO 结构的指针
			&pi // 指向 PROCESS_INFORMATION 结构的指针
		))
		{
			puts("ERROR - Can't start the emulator");
			afterworks(outDir);
		}

		Sleep(200);
		if (!LoadNTDriver("wcefs", wcefsPath))
		{
			printf("ERROR - Can't load WCEFS driver.");
			afterworks(outDir);
		}
		Sleep(1000);
		TerminateThread(hDispWindStarting, 0);

		WaitForSingleObject(pi.hProcess, INFINITE);
		Sleep(100);
		if (!UnloadNTDriver("wcefs"))
		{
			printf("ERROR - Can't unload WCEFS driver.");
			afterworks(outDir);
		}
		if (IsWindowsVistaOrGreater())
			TerminateThread(hKillApchook, 0);
	}

	afterworks(outDir);

	return 0;
}
