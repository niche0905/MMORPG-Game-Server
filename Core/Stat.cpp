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
        static_cast<uint16>(HP + other.HP),
        static_cast<uint16>(ATK + other.ATK),
        static_cast<uint16>(DEF + other.DEF),
        static_cast<uint16>(STR + other.STR),
        static_cast<uint16>(INT + other.INT),
        static_cast<uint16>(DEX + other.DEX),
        static_cast<uint16>(CRT + other.CRT),
        static_cast<uint16>(MOV + other.MOV)
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

Stats BasicStats(uint32 level)
{
    // 기본 스텟은 아래와 같다
    return Stats{ (uint16)(100 * level), (uint16)(50 * level), (uint16)level, (uint16)(20 * level), (uint16)(20 * level), (uint16)(20 * level), (uint16)level, (uint16)(2 * level) };
}

uint64 NeedExpToLevelUp(uint32 level)
{
    if (level >= MAX_LEVEL) return ULLONG_MAX;
    return 100 * (1ULL << level);   
}
