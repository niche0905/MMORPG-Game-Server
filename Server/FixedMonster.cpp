#include "pch.h"
#include "FixedMonster.h"


FixedMonster::FixedMonster()
	: FixedMonster{ INVALID_ID }
{

}

FixedMonster::FixedMonster(uint64 id)
	: Monster{ _id, true, false, true }
{

}

FixedMonster::~FixedMonster()
{

}

void FixedMonster::Update()
{

}

void FixedMonster::DropItem(uint64 id)
{

}
