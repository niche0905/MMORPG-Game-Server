#pragma once


constexpr int16 VIEW_RANGE = 5;
constexpr int32 SECTOR_SIZE = 20;	// 섹터의 기본 크기

constexpr std::array<int16, 4> vx = { -1, 1, -1, 1 };	// 꼭지점 x
constexpr std::array<int16, 4> vy = { -1, -1, 1, 1 };	// 꼭지점 y


enum IoOperation
{
	IO_START = 0,

	IO_RECV,
	IO_SEND,
	IO_ACCEPT,

	IO_NONE,

	IO_END
};

enum State : uint8
{
	STATE_START = 0,

	ST_ALLOC,
	ST_INGAME,
	ST_CLOSE,

	ST_ALIVE,
	ST_DEAD,

	STATE_END
};
