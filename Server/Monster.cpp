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
	: Bot{ id, ClassType::BOT_NONE, false, neut, peac, false, fix }
{

}

Monster::~Monster()
{

}

void Monster::DropItem(uint64 id)
{
	// TODO: 해당 객체에 돈 줘 (나중엔 아이템도 추가 가능하면 하고)
}
