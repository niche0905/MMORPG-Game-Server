#include "pch.h"
#include "Util.h"


bool CanSee(short p1_x, short p1_y, short p2_x, short p2_y)
{
	if (abs(p1_x - p2_x) > VIEW_RANGE) return false;
	return abs(p1_y - p2_y) <= VIEW_RANGE;
}

void PrintErrorMessage(DWORD err_code)
{
	WCHAR* lpMsgBuf;
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, err_code,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf, 0, NULL);
	std::wcout << L"���� : " << lpMsgBuf << std::endl;
	LocalFree(lpMsgBuf);
}

void PrintErrorMessage(const std::wstring pos, DWORD err_code)
{
	WCHAR* lpMsgBuf;
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, err_code,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf, 0, NULL);
	std::wcout << pos << L" ���� : " << lpMsgBuf << std::endl;
	LocalFree(lpMsgBuf);
}
