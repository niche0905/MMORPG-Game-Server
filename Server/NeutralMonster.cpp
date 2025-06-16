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
	_class_type = ClassType::BEAR_MONSTER;
	_visual_type = VisualInfo::VI_BEAR;
	_fsm.ChangeState(this, &IdleState::Instance());
}

NeutralMonster::~NeutralMonster()
{

}

bool NeutralMonster::TakeDamage(uint64 id, uint16 damage)
{
	bool my_kill = Bot::TakeDamage(id, damage);

	_target = server.GetClients()[id];

	return my_kill;
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
			_path.clear();
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

		// 때릴 수 있으면 Attack
		if (Attackalbe(client_pos)) {
			_fsm.ChangeState(this, &AttackState::Instance());
		}
		// 떄릴 수 없다면 Chase
		else {
			if (not _path.empty()) {
				_fsm.ChangeState(this, &ChaseState::Instance());
			}
			else {
				_fsm.ForceChangeState(this, &ChaseState::Instance());
			}
		}
	}

	_fsm.Update(this);
}

void NeutralMonster::Attack()
{
	const uint16 damage = 13;

	if (_target) {

		if (Attackalbe(_target->GetPosition())) {

			SC_ATTACK_PACKET attack_packet{ _id, KeyType::KEY_A, AttackDirection::NO_DIRECTION };

			std::unordered_set<uint64> closed_clients = server.GetClientList(_position);

			for (uint64 client_id : closed_clients) {

				if (client_id == _id) continue;

				if (::IsNPC(client_id)) continue;

				Creature* client = server.GetClients()[client_id];
				if (client == nullptr) continue;

				uint8 state = client->GetState();
				if (state == GameState::ST_ALLOC or state == GameState::ST_CLOSE) continue;

				if (not client->CanSee(_position, VIEW_RANGE)) continue;

				Session* session = static_cast<Session*>(client);
				session->Send(&attack_packet);

				if (state == GameState::ST_DEAD) continue;

				if (Attackalbe(client->GetPosition())) {
					// 맞은 것임

					client->TakeDamage(_id, damage);
				}

			}

		}

		if (not _target->CanSee(_position, VIEW_RANGE) or _target->GetState() == GameState::ST_DEAD) {	// TODO: 어그로 풀리는 범위를 적용하고 싶다면 상수 정의 ㄱㄱ
			_target = nullptr;
			_path.clear();
		}
	}
}

bool NeutralMonster::Attackalbe(Position pos)
{
	int16 dx = std::abs(pos.x - _position.x);
	int16 dy = std::abs(pos.y - _position.y);
	if ((dx + dy) <= 1) {
		// 맞은 것임

		return true;
	}

	return false;
}
