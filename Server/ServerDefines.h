#pragma once


constexpr int16 VIEW_RANGE = 5;
constexpr int32 SECTOR_SIZE = 20;	// ������ �⺻ ũ��

constexpr std::array<int16, 4> vx = { -1, 1, -1, 1 };	// ������ x
constexpr std::array<int16, 4> vy = { -1, -1, 1, 1 };	// ������ y


enum IoOperation
{
	IO_START = 0,

	IO_RECV,
	IO_SEND,
	IO_ACCEPT,

	IO_NONE,

	IO_END
};
