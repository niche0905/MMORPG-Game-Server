#pragma once


void PrintErrorMessage(DWORD err_code);
void PrintErrorMessage(const std::wstring msg, DWORD err_code);

bool IsNPC(uint64 id);
bool IsPlayer(uint64 id);

bool IsValid(int16 x, int16 y);
bool IsValid(const Position& pos);

bool IsBlock(int16 x, int16 y);
bool IsBlock(const Position& pos);
