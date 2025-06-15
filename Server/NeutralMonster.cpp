#include "pch.h"
#include "NeutralMonster.h"


NeutralMonster::NeutralMonster()
	: NeutralMonster{ INVALID_ID }
{

}

NeutralMonster::NeutralMonster(uint64 id)
	: Monster{ _id, true, false, false }
{

}

NeutralMonster::~NeutralMonster()
{

}

void NeutralMonster::Update()
{

}

void NeutralMonster::DropItem(uint64 id)
{

}
