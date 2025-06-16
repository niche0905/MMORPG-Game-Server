#pragma once
#include "Types.h"
#include <chrono>


constexpr int16 PORT_NUM = 8252;
constexpr int32 BUF_SIZE = 1024;

constexpr int32 MAX_USER = 10000;
constexpr int32 NUM_MONSTER = 20000;

constexpr int32 CACHE_LINE_SIZE = 64;

constexpr uint16 MAX_WIDTH = 2000;
constexpr uint16 MAX_HEIGHT = 2000;

constexpr uint64 INVALID_ID = UINT64_MAX;

constexpr std::chrono::milliseconds GRACE_TIME{ 100 };
constexpr std::chrono::milliseconds MOVE_COOLTIME{ 500 };
constexpr std::chrono::milliseconds AATK_COOLTIME{ 1000 };
constexpr std::chrono::milliseconds SATK_COOLTIME{ 5000 };
constexpr std::chrono::milliseconds DATK_COOLTIME{ 20000 };

enum LoginFailReason : int8
{
	NO_IDEA,		// 알 수 없는 이유
	USED_ID,		// 해당 아이디 현재 접속중
	INAPPOSITE_ID,	// 부적절한 ID (특수문자, 20자 이상의 이유)
	TO_MANY,		// 서버 부하로 인해 (너무 많은 접속자)

	GO_REGISTER		// DB에 없어서 새로 생성하거라
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

	// NPC들
	TALK_NPC,
	QUEST_NPC,
	MERCHANT_NPC,
	AGRS_MERCHANT_NPC,
	KNIGHT_NPC,

	// Monster들
	PEACE_MONSTER,
	FIXED_MONSTER,
	AGRO_MONSTER,

	CLASS_NONE,
	BOT_NONE,

	CLASS_END
};

enum VisualInfo : uint8
{
	VISUAL_START = 0,

	VI_WARRIOR,			// 전사
	VI_ROGUE,			// 도적
	VI_SORCERER,		// 마법사
	VI_GRAVE,			// 묘비(플레이어 사망)

	VI_MONSTER,			// 빨간색 테스트용 몬스터

	// TODO: 몬스터, 상인 등등 더 추가해야 한다

	VISUAL_END
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
	WARRIOR_S,		// 전사 S
	ROGUE_S,		// 도적 S
	SORCERER_S,		// 마법사 S

	FIXED_A,
	AGRO_A,

	// TODO: 추가할 필요가 있는 공격 타입 추가하기
};

enum AttackDirection : uint8
{
	NO_DIRECTION = 0,

	DIR_LEFT,
	DIR_RIGHT,
	DIR_UP,
	DIR_DOWN

};
