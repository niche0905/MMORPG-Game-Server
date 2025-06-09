#pragma once
#include "Types.h"
#include "Defines.h"


#pragma pack(push, 1)

struct BASE_PACKET
{
	uint8	_size;
	uint8	_packet_id;

	BASE_PACKET()
		: BASE_PACKET{ sizeof(BASE_PACKET), 0 } { }
	BASE_PACKET(uint8 packet_size, uint8 packet_id)
		: _size{ packet_size }
		, _packet_id{ packet_id } { }
};

struct SC_LOGIN_ALLOW_PACKET	: public BASE_PACKET
{
	uint64	_id;
	int16	_x, _y;
	uint16	_max_hp;
	uint16	_hp;
	uint8	_level;
	uint32	_exp;

	SC_LOGIN_ALLOW_PACKET(uint64 id, int16 x, int16 y, uint16 max_hp, uint16 hp, uint8 level, uint32 exp)
		: BASE_PACKET{ sizeof(SC_LOGIN_ALLOW_PACKET), S2C_LOGIN_ALLOW }
		, _id{ id }
		, _x{ x }, _y{ y }
		, _max_hp{ max_hp }
		, _hp{ hp }
		, _level{ level }
		, _exp{ exp } { }
};

struct SC_LOGIN_FAIL_PACKET		: public BASE_PACKET
{
	enum LoginFailReason : int8
	{
		NO_IDEA,	// 알 수 없는 이유
		USED_ID,	// 해당 아이디 현재 접속중
		INVALID_ID,	// 부적절한 ID (특수문자, 20자 이상의 이유)
		TO_MANY		// 서버 부하로 인해 (너무 많은 접속자)
	};

	uint64	_id;	// ID가 굳이 필요할까..?
	int8	_reason;

	SC_LOGIN_FAIL_PACKET(uint64 id, int8 reason)
		: BASE_PACKET{ sizeof(SC_LOGIN_FAIL_PACKET), S2C_LOGIN_FAIL }
		, _id{ id }
		, _reason{ reason } { }
};

struct SC_MOVE_PACKET			: public BASE_PACKET
{
	uint64	_id;
	int16	_x, _y;
	uint64	_move_time;

	SC_MOVE_PACKET(uint64 id, int16 x, int16 y, uint64 move_time)
		: BASE_PACKET{ sizeof(SC_MOVE_PACKET), S2C_MOVE }
		, _id{ id }
		, _x{ x }, _y{ y }
		, _move_time{ move_time } { }
};

struct SC_ENTER_PACKET			: public BASE_PACKET
{
	uint64	_id;
	int16	_x, _y;
	char	_name[MAX_NAME_LEN];
	uint8	_visual_info;
	uint8	_level;

	SC_ENTER_PACKET(uint64 id, int16 x, int16 y, char* name, uint8 visual_info, uint8 level)
		: BASE_PACKET{ sizeof(SC_ENTER_PACKET), S2C_ENTER }
		, _id{ id }
		, _x{ x }, _y{ y }
		, _visual_info{ visual_info }
		, _level{ level } { strcpy_s(_name, name); }
};

struct SC_LEAVE_PACKET			: public BASE_PACKET
{
	uint64	_id;

	SC_LEAVE_PACKET(uint64 id)
		: BASE_PACKET{ sizeof(SC_LEAVE_PACKET), S2C_LEAVE }
		, _id{ id } { }
};

struct SC_CHAT_PACKET			: public BASE_PACKET
{
	uint64	_id;
	char	_message[MAX_CHAT_LENGTH];

	SC_CHAT_PACKET(uint64 id, char* message)
		: BASE_PACKET{ sizeof(SC_CHAT_PACKET), S2C_CHAT }
		, _id{ id } { strcpy_s(_message, message); }
};

struct SC_STAT_CHANGE_PACKET	: public BASE_PACKET
{
	uint64	_id;
	int16	_hp;
	uint8	_level;
	uint32	_exp;

	SC_STAT_CHANGE_PACKET(uint64 id, int16 hp, uint8 level, uint32 exp)
		: BASE_PACKET{ sizeof(SC_STAT_CHANGE_PACKET), S2C_STAT_CHANGE }
		, _id{ id }
		, _hp{ hp }
		, _level{ level }
		, _exp{ exp } { }

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
	uint64	_move_time;

	CS_MOVE_PACKET(uint8 direction, uint64 move_time)
		: BASE_PACKET{ sizeof(CS_MOVE_PACKET), C2S_MOVE }
		, _direction{ direction }
		, _move_time{ move_time } { }
};

struct CS_CHAT_PACKET			: public BASE_PACKET
{
	char	_message[MAX_CHAT_LENGTH];

	CS_CHAT_PACKET(char* message)
		: BASE_PACKET{ sizeof(CS_CHAT_PACKET), C2S_CHAT }
		{ strcpy_s(_message, message); }
};

// 해당 공격을 다른 클라이언트에도 알려야 하지 않을까? (비주얼을 동기화 하기 위해)
struct CS_ATTACK_PACKET			: public BASE_PACKET
{
	int8	_atk_type;

	CS_ATTACK_PACKET(int8 atk_type)
		: BASE_PACKET{ sizeof(CS_ATTACK_PACKET), C2S_ATTACK }
		, _atk_type{ atk_type } { }
};

struct CS_TELEPORT_PACKET		: public BASE_PACKET
{
	int16	_x, _y;

	CS_TELEPORT_PACKET(int16 x, int16 y)
		: BASE_PACKET(sizeof(CS_TELEPORT_PACKET), C2S_TELEPORT)
		, _x{ x }
		, _y{ y } { }
};

#pragma pack(pop)
