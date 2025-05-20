#pragma once
#include "Types.h"


constexpr int16 PORT_NUM = 8252;
constexpr int32 BUF_SIZE = 1024;

constexpr int32 MAX_NAME_LEN = 20;
constexpr int32 CHAT_SIZE = 118;

constexpr uint16 MAX_WIDTH = 400;
constexpr uint16 MAX_HEIGHT = 400;


enum PacketID : uint8
{
	PACKET_START,

	S2C_LOGIN_ALLOW = 1,
	S2C_MOVE,	// Pressed Key board
	S2C_ENTER,
	S2C_LEAVE,
	S2C_CHAT,

	C2S_LOGIN,
	C2S_MOVE,
	C2S_CHAT,

	PACKET_END
};

enum MOVE : uint8
{
	MOVE_UP = 1,
	MOVE_DOWN,
	MOVE_LEFT,
	MOVE_RIGHT
};
