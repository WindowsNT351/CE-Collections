// Win9x:
//
// The CRT now uses several "W" versions of functions which is more practial to require
// the use of the Microsoft Layer for Unicode (MSLU) for Windows 9x to implement it.  The 
// unicows.dll (for 9x) should be placed in the program folder with the .exe if using it.
// unicows.dll is only loaded on 9x platforms. The way Win9x works without the MSLU is 
// that several "W" version of functions are located in kernel32.dll but they are just a
// stub that returns failure code. To implement the unicode layer (unicows) the unicode.lib
// must be linked prior to the other libs that should then linked in after unicode.lib.  
// The libraries are:
//
//    kernel32.lib advapi32.lib user32.lib gdi32.lib shell32.lib comdlg32.lib 
//    version.lib mpr.lib rasapi32.lib winmm.lib winspool.lib vfw32.lib 
//    secur32.lib oleacc.lib oledlg.lib sensapi.lib
//


#include <windows.h>

// pull items from ntdef.h
#define NT_SUCCESS(Status) (((NTSTATUS)(Status)) >= 0)
typedef _Return_type_success_(return >= 0) LONG NTSTATUS;

//
// implementation of replacement function for GetModuleHandleExW
//
extern "C" BOOL WINAPI ImplementGetModuleHandleExW(DWORD dwFlags, LPCWSTR lpModuleName, HMODULE* phModule)
{
	// check flag combinations
	if (phModule == NULL ||
		dwFlags & ~(GET_MODULE_HANDLE_EX_FLAG_PIN | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT | GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS) ||
		((dwFlags & GET_MODULE_HANDLE_EX_FLAG_PIN) && (dwFlags & GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT)) ||
		(lpModuleName == NULL && (dwFlags & GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS))) {
		SetLastError(ERROR_INVALID_PARAMETER);
		return FALSE;
	}


	// check if to get by address
	if (dwFlags & GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS) {
		*phModule = NULL;

		typedef PVOID(NTAPI *LPFN_RtlPcToFileHeader)(PVOID PcValue, PVOID * BaseOfImage);

		const HMODULE hmodkernel32 = GetModuleHandleW(L"kernel32");
		const LPFN_RtlPcToFileHeader pfnRtlPcToFileHeader = reinterpret_cast<LPFN_RtlPcToFileHeader>(GetProcAddress(hmodkernel32, "RtlPcToFileHeader"));

		if (pfnRtlPcToFileHeader) {
			// use RTL function (nt4+)
			pfnRtlPcToFileHeader((PVOID)lpModuleName, (PVOID*)phModule);
		}
		else {
			// query memory directly (win9x)
			MEMORY_BASIC_INFORMATION mbi;
			if (VirtualQuery((PVOID)lpModuleName, &mbi, sizeof(mbi)) >= offsetof(MEMORY_BASIC_INFORMATION, AllocationProtect)) {
				*phModule = reinterpret_cast<HMODULE>(mbi.AllocationBase);
			}
		}
	}
	else {
		// standard getmodulehandle - to see if loaded
		*phModule = GetModuleHandleW(lpModuleName);
	}


	// check if module found
	if (*phModule == NULL) {
		SetLastError(ERROR_DLL_NOT_FOUND);
		return FALSE;
	}

	// check if reference needs updating
	if ((dwFlags & GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT) == 0) {

		typedef NTSTATUS(NTAPI *LPFN_LdrAddRefDll)(ULONG Flags, PVOID BaseAddress);
#define LDR_ADDREF_DLL_PIN   0x00000001

		const HMODULE hmodntdll = GetModuleHandleW(L"ntdll");
		const LPFN_LdrAddRefDll pfnLdrAddRefDll = reinterpret_cast<LPFN_LdrAddRefDll>(GetProcAddress(hmodntdll, "LdrAddRefDll"));

		if (pfnLdrAddRefDll) {
			// update dll reference
			if (!NT_SUCCESS(pfnLdrAddRefDll((dwFlags & GET_MODULE_HANDLE_EX_FLAG_PIN) ? LDR_ADDREF_DLL_PIN : 0, *phModule))) {
				SetLastError(ERROR_GEN_FAILURE);
				return FALSE;
			}
		}
		else if (dwFlags & GET_MODULE_HANDLE_EX_FLAG_PIN) {
			SetLastError(ERROR_NOT_SUPPORTED);
			return FALSE;
		}
		else {
			WCHAR *filename;
			if ((filename = reinterpret_cast<WCHAR*>(VirtualAlloc(NULL, MAX_PATH * sizeof(WCHAR), MEM_COMMIT, PAGE_READWRITE))) != NULL) {
				DWORD ret = GetModuleFileNameW(*phModule, filename, MAX_PATH);
				if (ret < MAX_PATH) {
					*phModule = LoadLibraryW(filename);
				}
				else *phModule = NULL;
				VirtualFree(filename, 0, MEM_RELEASE);
				// ensure load library success
				if (*phModule == NULL) {
					return FALSE;
				}
			}
			else {
				SetLastError(ERROR_NOT_ENOUGH_MEMORY);
				return FALSE;
			}
		}
	}
	return TRUE;
}

//
// implementation of replacement function for SetFilePointerEx
//
extern "C" BOOL WINAPI ImplementSetFilePointerEx(HANDLE hFile, LARGE_INTEGER liDistanceToMove, PLARGE_INTEGER lpNewFilePointer, DWORD dwMoveMethod)
{
	DWORD ret = SetFilePointer(hFile, liDistanceToMove.LowPart, &liDistanceToMove.HighPart, dwMoveMethod);
	if (ret == INVALID_SET_FILE_POINTER) {
		if (GetLastError() != NO_ERROR) {
			return FALSE;
		}
	}
	// check if provide file location
	if (lpNewFilePointer) {
		lpNewFilePointer->LowPart = ret;
		lpNewFilePointer->HighPart = liDistanceToMove.HighPart;
	}
	// success
	return TRUE;
}
