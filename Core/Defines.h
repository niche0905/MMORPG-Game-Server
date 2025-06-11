#pragma once
#include "Types.h"


constexpr int16 PORT_NUM = 8252;
constexpr int32 BUF_SIZE = 1024;

constexpr int32 MAX_USER = 10000;
constexpr int32 NUM_MONSTER = 200000;

constexpr int32 CACHE_LINE_SIZE = 64;

constexpr uint16 MAX_WIDTH = 2000;
constexpr uint16 MAX_HEIGHT = 2000;

constexpr uint64 INVALID_ID = UINT64_MAX;


enum MOVE : uint8
{
	MOVE_UP = 1,
	MOVE_DOWN,
	MOVE_LEFT,
	MOVE_RIGHT
};
