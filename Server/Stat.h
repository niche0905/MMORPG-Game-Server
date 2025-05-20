#pragma once


struct BaseStats			// 모든 캐릭터/장비가 가지는 스텟
{
	uint16 HP	= 0;		// 체력
	uint16 ATK	= 0;		// 공격력
	uint16 DEF	= 0;		// 방어력
	uint16 STR	= 0;		// 힘
	uint16 INT	= 0;		// 지능
	uint16 DEX	= 0;		// 솜씨
	uint16 CRT	= 0;		// 치명타
	uint16 MOV	= 0;		// 이동력

	BaseStats();
	BaseStats(uint16 hp, uint16 atk, uint16 def, uint16 str, uint16 intelli, uint16 dex, uint16 crt, uint16 mov);

	BaseStats operator+(const BaseStats& other) const;
	BaseStats operator+=(const BaseStats& other);
};

struct CharacterStats	
{
	uint8	level = 1;	// 캐릭터가 가지는 레벨 (적 도 있어야 함)
};

struct UserStats : CharacterStats
{
	uint32	exp = 0;	// 플레이어만 가지는 경험치
};

using Stats = BaseStats;
