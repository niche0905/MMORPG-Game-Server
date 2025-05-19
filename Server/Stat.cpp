#include "pch.h"
#include "Stat.h"


BaseStats::BaseStats()
{
}

BaseStats::BaseStats(uint16 hp, uint16 atk, uint16 def, uint16 str, uint16 intelli, uint16 dex, uint16 crt, uint16 mov)
	: HP{ hp }
	, ATK{ atk }
	, DEF{ def }
	, STR{ str }
	, INT{ intelli }
	, DEX{ dex }
	, CRT{ crt }
	, MOV{ mov }
{
}

BaseStats BaseStats::operator+(const BaseStats& other) const
{
	return {
		HP + other.HP,
		ATK + other.ATK,
		DEF + other.DEF,
		STR + other.STR,
		INT + other.INT,
		DEX + other.DEX,
		CRT + other.CRT,
		MOV + other.MOV
	};
}

BaseStats BaseStats::operator+=(const BaseStats& other)
{
	HP += other.HP;
	ATK += other.ATK;
	DEF += other.DEF;
	STR += other.STR;
	INT += other.INT;
	DEX += other.DEX;
	CRT += other.CRT;
	MOV += other.MOV;
	return *this;
}
