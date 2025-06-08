#pragma once


bool CanSee(int16 p1_x, int16 p1_y, int16 p2_x, int16 p2_y);	// 이제 안쓸듯?

void PrintErrorMessage(DWORD err_code);
void PrintErrorMessage(const std::wstring msg, DWORD err_code);

bool IsNPC(int64 id);
bool IsPlayer(int64 id);
