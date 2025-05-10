#pragma once


constexpr short PORT_NUM = 8252;
constexpr int BUF_SIZE = 1024;

constexpr int MAX_NAME_LEN = 20;

constexpr unsigned short MAX_WIDTH = 400;
constexpr unsigned short MAX_HEIGHT = 400;


enum MOVE : unsigned char
{
	MOVE_UP = 1,
	MOVE_DOWN,
	MOVE_LEFT,
	MOVE_RIGHT
};
