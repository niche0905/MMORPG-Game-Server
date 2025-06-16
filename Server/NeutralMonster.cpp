#include "pch.h"
#include "NeutralMonster.h"
#include "IdleState.h"
#include "ChaseState.h"
#include "AttackState.h"
#include "ReturnState.h"
#include "DeadState.h"


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
	if (_state == GameState::ST_DEAD) {
		_fsm.ChangeState(this, &DeadState::Instance());
		return;
	}

	if (_target != nullptr) {
		uint8 state = _target->GetState();
		if (state == GameState::ST_CLOSE or state == GameState::ST_DEAD or not _target->CanSee(_position, VIEW_RANGE)) {
			_target = nullptr;
		}
	}

	if (_target == nullptr) {
		// 거리가 멀다면 Return
		if (not CanSee(_base_pos, RETURN_RANGE)) {		// 스폰 지점에서 너무 멀다!
			_fsm.ChangeState(this, &ReturnState::Instance());
		}
		// 가깝다면 Idle
		else if (_path.empty()) {
			_fsm.ChangeState(this, &IdleState::Instance());
		}
	}
	else {
		Position client_pos = _target->GetPosition();

		if (not _path.empty()) {
			// 때릴 수 있으면 Attack
			if (Attackalbe(client_pos)) {
				_fsm.ChangeState(this, &AttackState::Instance());
			}
			// 떄릴 수 없다면 Chase
			else {
				_fsm.ChangeState(this, &ChaseState::Instance());
			}
		}
		// 경로를 도달 했지만 할 수 있는게 없다 (다시 찾아라)
		else {
			_fsm.ForceChangeState(this, &ChaseState::Instance());
		}
	}

	_fsm.Update(this);
}

void NeutralMonster::DropItem(uint64 id)
{

}
