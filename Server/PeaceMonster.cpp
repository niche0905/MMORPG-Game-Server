#include "pch.h"
#include "PeaceMonster.h"


PeaceMonster::PeaceMonster()
	: PeaceMonster{ INVALID_ID }
{

}

PeaceMonster::PeaceMonster(uint64 id)
	: Monster{ _id, true, true, false }
{

}

PeaceMonster::~PeaceMonster()
{

}

void PeaceMonster::Update()
{

}

void PeaceMonster::DropItem(uint64 id)
{

}
