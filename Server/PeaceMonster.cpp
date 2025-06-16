#include "pch.h"
#include "PeaceMonster.h"
#include "DeadState.h"
#include "PM_IdleState.h"
#include "PM_RunState.h"
#include "ReturnState.h"


PeaceMonster::PeaceMonster()
	: PeaceMonster{ INVALID_ID }
{

}

PeaceMonster::PeaceMonster(uint64 id)
	: Monster{ id, true, true, false }
{
	_fsm.ChangeState(this, &PM_IdleState::Instance());
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

	if (_target != nullptr) {
		uint8 state = _target->GetState();
		if (state == GameState::ST_CLOSE or state == GameState::ST_DEAD or not _target->CanSee(_position, VIEW_RANGE)) {
			_target = nullptr;
			return;
		}
	}

	if (_target == nullptr) {
		if (not CanSee(_base_pos, RETURN_RANGE)) {		// 스폰 지점에서 너무 멀다!
			_fsm.ChangeState(this, &ReturnState::Instance());
		}
		else if (_path.empty()) {
			_fsm.ChangeState(this, &PM_IdleState::Instance());
		}
	}
	else {
		if (not _path.empty()) {
			_fsm.ChangeState(this, &PM_RunState::Instance());
		}
		else {
			_fsm.ForceChangeState(this, &PM_RunState::Instance());
		}
	}

	_fsm.Update(this);
}

void PeaceMonster::DropItem(uint64 id)
{

}

void PeaceMonster::ReviveChangeState()
{
	_fsm.ChangeState(this, &PM_IdleState::Instance());
}
