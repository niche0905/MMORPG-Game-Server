#include "pch.h"
#include "Util.h"


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

bool IsValid(int16 x, int16 y)
{
	return (0 <= x and x < MAX_WIDTH and 0 <= y and y < MAX_HEIGHT);
}

bool IsValid(const Position& pos)
{
	return (0 <= pos.x and pos.x < MAX_WIDTH and 0 <= pos.y and pos.y < MAX_HEIGHT);
}

bool IsBlock(int16 x, int16 y)
{
	return server.IsBlock(x, y);
}

bool IsBlock(const Position& pos)
{
	return server.IsBlock(pos);
}
