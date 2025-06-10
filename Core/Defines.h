#pragma once
#include "Types.h"


constexpr int16 PORT_NUM = 8252;
constexpr int32 BUF_SIZE = 1024;

constexpr int32 MAX_USER = 10000;
constexpr int32 NUM_MONSTER = 200000;

constexpr int32 CACHE_LINE_SIZE = 64;

constexpr int8 MAX_NAME_LEN = 20;
constexpr int16 MAX_CHAT_LENGTH = 245;
// size 1byte를 고수하기 위해... 255 -> 245로 수정

constexpr uint16 MAX_WIDTH = 2000;
constexpr uint16 MAX_HEIGHT = 2000;

constexpr uint64 INVALID_ID = UINT64_MAX;


enum PacketID : uint8
{
	PACKET_START,

	S2C_LOGIN_ALLOW = 1,
	S2C_LOGIN_FAIL,
	S2C_MOVE,
	S2C_ENTER,
	S2C_LEAVE,
	S2C_CHAT,
	S2C_STAT_CHANGE,

	C2S_LOGIN,
	C2S_MOVE,
	C2S_CHAT,
	C2S_ATTACK,
	C2S_TELEPORT,

	PACKET_END
};

enum MOVE : uint8
{
	MOVE_UP = 1,
	MOVE_DOWN,
	MOVE_LEFT,
	MOVE_RIGHT
};
