#include "pch.h"
#include "AgroMonster.h"


AgroMonster::AgroMonster()
	: AgroMonster{ INVALID_ID }
{

}

AgroMonster::AgroMonster(uint64 id)
	: Monster{ id, false, false, false}
{

}

AgroMonster::~AgroMonster()
{

}

void AgroMonster::Update()
{

}

void AgroMonster::DropItem(uint64 id)
{

}
