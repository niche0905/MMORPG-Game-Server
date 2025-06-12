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


enum LoginFailReason : int8
{
	NO_IDEA,		// 알 수 없는 이유
	USED_ID,		// 해당 아이디 현재 접속중
	INAPPOSITE_ID,	// 부적절한 ID (특수문자, 20자 이상의 이유)
	TO_MANY			// 서버 부하로 인해 (너무 많은 접속자)
};

enum MOVE : uint8
{
	MOVE_UP = 1,
	MOVE_DOWN,
	MOVE_LEFT,
	MOVE_RIGHT
};

enum ClassType : uint8
{
	CLASS_START = 0,

	WARRIOR,		// 전사
	ROGUE,			// 도적
	SORCERER,		// 마법사
	
	CLASS_NONE,

	CLASS_END
};

enum KeyType : uint8
{
	KEY_START = 0,

	KEY_A,
	KEY_S,
	KEY_D,
	KEY_SB,		// Space bar

	KEY_END
};

enum AttackType : uint8
{
	ATTACK_NONE = 0,

	STANDARD_ATK,	// 기본 +자 공격

	// TODO: 추가할 필요가 있는 공격 타입 추가하기
};
