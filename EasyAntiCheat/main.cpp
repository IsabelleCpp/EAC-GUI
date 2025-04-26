
#include <Windows.h>
#include <string>

#define INJ_86 TEXT("EasyAntiCheat_x86.exe")
#define INJ_64 TEXT("EasyAntiCheat_x64.exe")

int main()
{
	BOOL WoW64 = FALSE;
	IsWow64Process(GetCurrentProcess(), &WoW64);

	const TCHAR * szExe = WoW64 ? INJ_64 : INJ_86;
	
	if (GetFileAttributes(szExe) == INVALID_FILE_ATTRIBUTES && WoW64)
	{
		szExe = INJ_86;
	}

	if (GetFileAttributes(szExe) == INVALID_FILE_ATTRIBUTES)
	{
		MessageBoxA(NULL, "EasyAntiCheat executables are missing. Make sure all files are in the correct directory.", "Error", MB_ICONERROR);

		return ERROR_FILE_NOT_FOUND;
	}

	PROCESS_INFORMATION pi{ 0 };
	STARTUPINFO			si{ 0 };
	si.cb = sizeof(si);

	auto bRet = CreateProcess(szExe, nullptr, nullptr, nullptr, FALSE, NULL, nullptr, nullptr, &si, &pi);
	if (!bRet)
	{
		DWORD dwRet = GetLastError();

		char error_code[9]{ 0 };
		_ultoa_s(dwRet, error_code, 0x10);

		std::string msg;
		msg += "Failed to launch EasyAntiCheat. CreateProcess returned error code 0x";
		msg += error_code;

		MessageBoxA(NULL, msg.c_str(), "Error", MB_ICONERROR);

		return dwRet;
	}

	CloseHandle(pi.hProcess);
	CloseHandle(pi.hThread);

	return ERROR_SUCCESS;
}