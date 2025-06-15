#include "pch.h"
#include "PeaceMonster.h"
#include "DeadState.h"
#include "PM_IdleState.h"
#include "PM_RunState.h"
#include "PM_ReturnState.h"


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

bool PeaceMonster::TakeDamage(uint64 id, uint16 damage)
{
	bool my_kill = Bot::TakeDamage(id, damage);

	_target = server.GetClients()[id];

	return my_kill;
}

void PeaceMonster::Update()
{
	if (_state == GameState::ST_DEAD) {
		_fsm.ChangeState(this, &DeadState::Instance());
		return;
	}

	// TODO: 보다 올바르게
	// target이 시야에 없으면 target = nullptr
	// target이 있으면 run
	// target이 없으면 return
	// TakeDamage에서 target 설정

	_fsm.Update(this);
}

void PeaceMonster::DropItem(uint64 id)
{

}
