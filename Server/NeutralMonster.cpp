#include "pch.h"
#include "NeutralMonster.h"
#include "IdleState.h"


NeutralMonster::NeutralMonster()
	: NeutralMonster{ INVALID_ID }
{

}

NeutralMonster::NeutralMonster(uint64 id)
	: Monster{ id, true, false, false }
{
	_class_type = ClassType::NEUTRAL_MONSTER;
	_fsm.ChangeState(this, &IdleState::Instance());
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
