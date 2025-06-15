#pragma once
#include "Types.h"
#include "Defines.h"


constexpr int8 MAX_NAME_LEN = 20;
constexpr int16 MAX_CHAT_LENGTH = 245;
// size 1byte를 고수하기 위해... 255 -> 245로 수정

enum PacketID : uint8
{
	PACKET_START,

//---------------------------------------------------------------------------//

	S2C_LOGIN_ALLOW = 1,		// 스스로 보내는 로그인 허락 및 정보
	S2C_LOGIN_FAIL,				// 스스로 보내는 로그린 실패와 이유
	S2C_MOVE_SELF,				// 스스로 위치 Update (스트레스 테스트용)
	S2C_STAT_CHANGE,			// 스스로 보내는 스텟 변경
	S2C_DAMAGE,					// 스스로 공격에 대한 Damage 출력용
	S2C_LEVEL_UP,				// 스스로 레벨이 올랐음을 알려주는 패킷
	S2C_EXP_UP,					// 스스로 경험치가 올랐음을 알려주는 패킷

	S2C_MOVE,					// 누군가 움직였다
	S2C_ENTER,					// 누군가 들어왔다
	S2C_LEAVE,					// 누군가 떠났다
	S2C_CHAT,					// 누군가 채팅했다
	S2C_ATTACK,					// 누군가 공격했다
	S2C_HP_CHANGE,				// 누군가 체력이 바뀌었다
	S2C_LEVEL_CHANGE,			// 누군가 레벨이 바뀌었다


	C2S_LOGIN,					// 로그인 요청
	C2S_REGISTER,				// 신규 가입 요청
	C2S_MOVE,					// 이동 요청
	C2S_TELEPORT,				// TP 요청
	C2S_CHAT,					// 채팅 송신
	C2S_ATTACK,					// 공격

//---------------------------------------------------------------------------//

	PACKET_END
};

#pragma pack(push, 1)

struct DamageInfo
{
	uint64 _id;
	uint16 _damage;
};

//---------------------------------------------------------------------------//
//---------------------------------------------------------------------------//
//---------------------------------------------------------------------------//

// TODO: 가변길이 데이터 마지막 압축하기


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
	uint8	_visual_info;
	uint8	_class_type;
	uint32	_level;
	uint64	_exp;

	SC_LOGIN_ALLOW_PACKET(uint64 id, int16 x, int16 y, uint16 max_hp, uint16 hp, uint8 visual_info, uint8 class_type, uint32 level, uint64 exp)
		: BASE_PACKET{ sizeof(SC_LOGIN_ALLOW_PACKET), S2C_LOGIN_ALLOW }
		, _id{ id }
		, _x{ x }, _y{ y }
		, _max_hp{ max_hp }
		, _hp{ hp }
		, _visual_info{ visual_info }
		, _class_type{ class_type }
		, _level{ level }
		, _exp{ exp } { }
};

struct SC_LOGIN_FAIL_PACKET		: public BASE_PACKET
{
	int8	_reason;

	SC_LOGIN_FAIL_PACKET(int8 reason)
		: BASE_PACKET{ sizeof(SC_LOGIN_FAIL_PACKET), S2C_LOGIN_FAIL }
		, _reason{ reason } { }
};

struct SC_MOVE_SELF_PACKET : public BASE_PACKET
{
	int16	_x, _y;
	uint64	_move_time;

	SC_MOVE_SELF_PACKET(int16 x, int16 y, uint64 move_time)
		: BASE_PACKET{ sizeof(SC_MOVE_SELF_PACKET), S2C_MOVE_SELF }
		, _x{ x }, _y{ y }
		, _move_time{ move_time } { }
};

struct SC_STAT_CHANGE_PACKET : public BASE_PACKET
{
	uint16	_max_hp;
	uint16	_hp;

	SC_STAT_CHANGE_PACKET(uint16 max_hp, uint16 hp)
		: BASE_PACKET{ sizeof(SC_STAT_CHANGE_PACKET), S2C_STAT_CHANGE }
		, _max_hp{ max_hp }
		, _hp{ hp } { }
};

struct SC_DAMAGE_PACKET : public BASE_PACKET
{
	static constexpr int32 MAX_DAMAGE_INFO_NUM = 20;

	uint32		_num;
	DamageInfo	_damage_infos[MAX_DAMAGE_INFO_NUM];

	SC_DAMAGE_PACKET()
		: BASE_PACKET{ sizeof(BASE_PACKET) + sizeof(uint32), S2C_DAMAGE}
		, _num{ 0 }
		, _damage_infos{} { }

	void AddDamageInfo(uint64 id, uint16 damage)
	{
		_damage_infos[_num] = { id, damage };
		_size += sizeof(DamageInfo);
		_num += 1;
	}
};

struct SC_LEVEL_UP_PACKET : public BASE_PACKET
{
	uint32	_level;
	uint64	_exp;

	SC_LEVEL_UP_PACKET(uint32 level, uint64 exp)
		: BASE_PACKET{ sizeof(SC_LEVEL_UP_PACKET), S2C_LEVEL_UP }
		, _level{ level }
		, _exp{ exp } { }
};

struct SC_EXP_UP_PACKET : public BASE_PACKET
{
	uint64	_exp;

	SC_EXP_UP_PACKET(uint64 exp)
		: BASE_PACKET{ sizeof(SC_EXP_UP_PACKET), S2C_EXP_UP }
		, _exp{ exp } { }
};

struct SC_MOVE_PACKET			: public BASE_PACKET
{
	uint64	_id;
	int16	_x, _y;

	SC_MOVE_PACKET(uint64 id, int16 x, int16 y)
		: BASE_PACKET{ sizeof(SC_MOVE_PACKET), S2C_MOVE }
		, _id{ id }
		, _x{ x }, _y{ y } { }
};

struct SC_ENTER_PACKET			: public BASE_PACKET
{
	uint64	_id;
	int16	_x, _y;
	char	_name[MAX_NAME_LEN];
	uint8	_visual_info;
	uint8	_class_type;
	uint8	_level;

	SC_ENTER_PACKET(uint64 id, int16 x, int16 y, const char* name, uint8 visual_info, uint8 class_type, uint8 level)
		: BASE_PACKET{ sizeof(SC_ENTER_PACKET), S2C_ENTER }
		, _id{ id }
		, _x{ x }, _y{ y }
		, _visual_info{ visual_info }
		, _class_type{ class_type }
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

struct SC_ATTACK_PACKET : public BASE_PACKET
{
	uint64	_id;
	uint8	_atk_key;
	uint8	_atk_dir;

	SC_ATTACK_PACKET(uint64 id, uint8 atk_key, uint8 atk_dir)
		: BASE_PACKET{ sizeof(SC_ATTACK_PACKET), S2C_ATTACK }
		, _id{ id }
		, _atk_key{ atk_key } 
		, _atk_dir{ atk_dir } { }
};

struct SC_HP_CHANGE_PACKET : public BASE_PACKET
{
	uint64	_id;
	uint16	_hp;

	SC_HP_CHANGE_PACKET(uint64 id, uint16 hp)
		: BASE_PACKET{ sizeof(SC_HP_CHANGE_PACKET), S2C_HP_CHANGE }
		, _id{ id }
		, _hp{ hp } { }
};

struct SC_LEVEL_CHANGE_PACKET : public BASE_PACKET
{
	uint64	_id;
	uint32	_level;

	SC_LEVEL_CHANGE_PACKET(uint64 id, uint32 level)
		: BASE_PACKET{ sizeof(SC_LEVEL_CHANGE_PACKET), S2C_LEVEL_CHANGE }
		, _id{ id }
		, _level{ level } { }
};

struct CS_LOGIN_PACKET			: public BASE_PACKET
{
	char	_name[MAX_NAME_LEN];

	CS_LOGIN_PACKET(const char* name)
		: BASE_PACKET{ sizeof(CS_LOGIN_PACKET), C2S_LOGIN }
		{ strcpy_s(_name, name); }
};

struct CS_REGISTER_PACKET : public BASE_PACKET
{
	char	_name[MAX_NAME_LEN];
	uint8	_class_type;

	CS_REGISTER_PACKET(const char* name, uint8 class_type)
		: BASE_PACKET{ sizeof(CS_REGISTER_PACKET), C2S_REGISTER }
		, _class_type{ class_type }
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

	CS_CHAT_PACKET(const char* message)
		: BASE_PACKET{ sizeof(CS_CHAT_PACKET), C2S_CHAT }
		{ strcpy_s(_message, message); }
};

struct CS_ATTACK_PACKET			: public BASE_PACKET
{
	uint8	_atk_key;
	uint8	_atk_dir;

	CS_ATTACK_PACKET(uint8 atk_key, uint8 atk_dir)
		: BASE_PACKET{ sizeof(CS_ATTACK_PACKET), C2S_ATTACK }
		, _atk_key{ atk_key }
		, _atk_dir{ atk_dir } { }
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


constexpr int32 HEADER_SIZE = sizeof(BASE_PACKET);
