#pragma once
#include "Types.h"
#include "Defines.h"


#pragma pack(push, 1)

struct BASE_PACKET
{
	uint8	_size;
	uint8	_packet_id;

	BASE_PACKET(uint8 packet_size, uint8 packet_id)
		: _size{ packet_size }
		, _packet_id{ packet_id } { }
	BASE_PACKET()
		: _size{ sizeof(BASE_PACKET) }
		, _packet_id{ 0 } { }
};

struct SC_LOGIN_ALLOW_PACKET	: public BASE_PACKET
{
	int64	_id;
	int16	_x, _y;
	uint16	_max_hp;
	uint16	_hp;
	uint8	_level;
	uint32	_exp;

	SC_LOGIN_ALLOW_PACKET(int64 id, int16 x, int16 y, uint16 max_hp, uint16 hp, uint8 level, uint32 exp)
		: BASE_PACKET{ sizeof(SC_LOGIN_ALLOW_PACKET), S2C_LOGIN_ALLOW }
		, _id{ id }
		, _x{ x }, _y{ y }
		, _max_hp{ max_hp }
		, _hp{ hp }
		, _level{ level }
		, _exp{ exp } { }
};

struct SC_MOVE_PACKET			: public BASE_PACKET
{
	int64	_id;
	int16	_x, _y;
	uint32	_move_time;

	SC_MOVE_PACKET(int64 id, int16 x, int16 y, uint32 move_time)
		: BASE_PACKET{ sizeof(SC_MOVE_PACKET), S2C_MOVE }
		, _id{ id }
		, _x{ x }, _y{ y }
		, _move_time{ move_time } { }
};

struct SC_ENTER_PACKET			: public BASE_PACKET
{
	int64	_id;
	int16	_x, _y;
	char	_name[MAX_NAME_LEN];
	uint8	_visual_info;
	uint8	_level;

	SC_ENTER_PACKET(int64 id, int16 x, int16 y, char* name, uint8 visual_info, uint8 level)
		: BASE_PACKET{ sizeof(SC_ENTER_PACKET), S2C_ENTER }
		, _id{ id }
		, _x{ x }, _y{ y }
		, _visual_info{ visual_info }
		, _level{ level } { strcpy_s(_name, name); }
};

struct SC_LEAVE_PACKET			: public BASE_PACKET
{
	int64	_id;

	SC_LEAVE_PACKET(int64 id)
		: BASE_PACKET{ sizeof(SC_LEAVE_PACKET), S2C_LEAVE }
		, _id{ id } { }
};

struct SC_CHAT_PACKET			: public BASE_PACKET
{
	int64	_id;
	char	_message[CHAT_SIZE];

	SC_CHAT_PACKET(int64 id, char* message)
		: BASE_PACKET{ sizeof(SC_CHAT_PACKET), S2C_CHAT }
		, _id{ id } { strcpy_s(_message, message); }
};

struct CS_LOGIN_PACKET			: public BASE_PACKET
{
	char	_name[MAX_NAME_LEN];

	CS_LOGIN_PACKET(char* name)
		: BASE_PACKET{ sizeof(CS_LOGIN_PACKET), C2S_LOGIN }
		{ strcpy_s(_name, name); }
};

struct CS_MOVE_PACKET			: public BASE_PACKET
{
	uint8	_direction;
	uint32	_move_time;

	CS_MOVE_PACKET(uint8 direction, uint32 move_time)
		: BASE_PACKET{ sizeof(CS_MOVE_PACKET), C2S_MOVE }
		, _direction{ direction }
		, _move_time{ move_time } { }
};

struct CS_CHAT_PACKET			: public BASE_PACKET
{
	char	_message[CHAT_SIZE];

	CS_CHAT_PACKET(char* message)
		: BASE_PACKET{ sizeof(CS_CHAT_PACKET), C2S_CHAT }
		{ strcpy_s(_message, message); }
};

#pragma pack(pop)
