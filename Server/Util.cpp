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
	std::wcout << L"에러 : " << lpMsgBuf << std::endl;
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
	std::wcout << pos << L" 에러 : " << lpMsgBuf << std::endl;
	LocalFree(lpMsgBuf);
}

// TODO: bit 연산으로 바꾸면 더 빠를 수도?
bool IsNPC(uint64 id)
{
	return id < NUM_MONSTER;
}

bool IsPlayer(uint64 id)
{
	return id >= NUM_MONSTER;
}
