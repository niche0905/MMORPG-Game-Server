#include "pch.h"
#include "Monster.h"


Monster::Monster()
	: Monster{ INVALID_ID }
{

}

Monster::Monster(uint64 id)
	: Monster{ id, true, false, false }
{

}

Monster::Monster(uint64 id, bool neut, bool peac, bool fix)
	: Bot{ id, Bot::BotType::BOT_NONE, false, neut, peac, false, fix }
{

}

Monster::~Monster()
{

}
