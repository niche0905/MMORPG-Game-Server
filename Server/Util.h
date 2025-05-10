#pragma once


bool CanSee(short p1_x, short p1_y, short p2_x, short p2_y);

void PrintErrorMessage(DWORD err_code);
void PrintErrorMessage(const std::string msg, DWORD err_code);
