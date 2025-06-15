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

	if (_target != nullptr and not _target->CanSee(_position, VIEW_RANGE)) {
		_target == nullptr;
	}

	if (_target == nullptr) {
		if (not CanSee(_base_pos, RETURN_RANGE)) {		// 스폰 지점에서 너무 멀다!
			_fsm.ChangeState(this, &PM_ReturnState::Instance());
		}
		else if (_path.empty()) {
			_fsm.ChangeState(this, &PM_IdleState::Instance());
		}
	}
	else {
		_fsm.ChangeState(this, &PM_RunState::Instance());
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

void PeaceMonster::DoReturnSequence()
{
	if (_current_index == -1 || _current_index >= _path.size()) return;

	// 목표 좌표
	Position target = _path[_current_index];

	if (DoMove(target)) // 이동에 성공하면
	{
		_current_index++;
		if (_current_index >= _path.size())
		{
			// 도착 완료
			_path.clear();
			_current_index = -1;
		}
	}
}
