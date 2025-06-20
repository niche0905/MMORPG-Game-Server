#pragma once


constexpr int16 VIEW_RANGE = 7;
constexpr int16 REAL_VIEW_RANGE = (2 * VIEW_RANGE) + 1;
constexpr int16 RETURN_RANGE = 10;
constexpr int32 SECTOR_SIZE = 20;	// 섹터의 기본 크기

constexpr int16 FIX_MONSTER_ATK_RANGE = 2;
constexpr int16 AGRO_MONSTER_RANGE = 5;

constexpr std::array<int16, 4> vx = { -VIEW_RANGE, VIEW_RANGE, -VIEW_RANGE, VIEW_RANGE };	// 꼭지점 x
constexpr std::array<int16, 4> vy = { -VIEW_RANGE, -VIEW_RANGE, VIEW_RANGE, VIEW_RANGE };	// 꼭지점 y


enum OverOperation
{
	OVER_START = 0,

	IO_RECV,
	IO_SEND,
	IO_ACCEPT,

	DO_HEAL,

	DO_UPDATE,
	DO_REVIVE,

	DB_LOGIN,
	DB_LOGIN_FAIL,
	DB_REGISTER_FAIL,
	DB_LOGOUT,

	OVER_NONE,

	OVER_END
};

enum GameState : uint8
{
	STATE_START = 0,

	ST_ALLOC,
	ST_INGAME,
	ST_CLOSE,

	ST_ALIVE,
	ST_DEAD,

	STATE_END
};
