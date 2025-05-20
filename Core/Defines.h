#pragma once
#include "Types.h"


constexpr int16 PORT_NUM = 8252;
constexpr int32 BUF_SIZE = 1024;

constexpr int32 MAX_NAME_LEN = 20;

constexpr uint16 MAX_WIDTH = 400;
constexpr uint16 MAX_HEIGHT = 400;


enum MOVE : uint8
{
	MOVE_UP = 1,
	MOVE_DOWN,
	MOVE_LEFT,
	MOVE_RIGHT
};
