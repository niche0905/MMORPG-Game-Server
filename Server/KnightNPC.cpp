#include "pch.h"
#include "KnightNPC.h"
#include "DeadState.h"
#include "ReturnState.h"
#include "IdleState.h"
#include "ChaseState.h"
#include "AttackState.h"


KnightNPC::KnightNPC()
	: KnightNPC{ INVALID_ID }
{

}

KnightNPC::KnightNPC(uint64 id)
	: Bot{ id, ClassType::KNIGHT_NPC, false, true, false, true, false }
{

}

KnightNPC::~KnightNPC()
{

}

void KnightNPC::Update()
{
	if (_state == GameState::ST_DEAD) {
		_fsm.ChangeState(this, &DeadState::Instance());
		return;
	}

	if (_target != nullptr) {
		uint8 state = _target->GetState();
		if (state == GameState::ST_DEAD or not _target->CanSee(_position, VIEW_RANGE)) {
			_target = nullptr;
			_path.clear();
		}
	}

	// target 물색
	if (_target == nullptr) {

		std::unordered_set<uint64> view_list = server.GetClientList(_position);

		for (uint64 client_id : view_list) {

			if (::IsPlayer(client_id)) continue;

			Creature* client = server.GetClients()[client_id];
			if (client == nullptr) continue;

			uint8 state = client->GetState();
			if (state == GameState::ST_DEAD) continue;

			if (not client->CanSee(_position, VIEW_RANGE)) continue;

			Bot* npc = static_cast<Bot*>(client);
			if (npc->IsFriendly()) continue;

			for (int16 gap = 0; gap <= AGRO_MONSTER_RANGE; ++gap) {
				if (client->CanSee(_position, gap)) {
					_target = client;
					break;
				}
			}

			if (_target != nullptr) break;
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

void KnightNPC::Attack()
{
	const uint16 damage = 15;

	if (_target) {

		if (Attackalbe(_target->GetPosition())) {

			SC_ATTACK_PACKET attack_packet{ _id, KeyType::KEY_A, AttackDirection::NO_DIRECTION };

			std::unordered_set<uint64> closed_clients = server.GetClientList(_position);

			for (uint64 client_id : closed_clients) {

				if (client_id == _id) continue;

				if (::IsPlayer(client_id)) continue;

				Creature* client = server.GetClients()[client_id];
				if (client == nullptr) continue;

				uint8 state = client->GetState();
				if (state == GameState::ST_ALLOC or state == GameState::ST_CLOSE) continue;

				if (not client->CanSee(_position, VIEW_RANGE)) continue;

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

bool KnightNPC::Attackalbe(Position pos)
{
	int16 dx = std::abs(pos.x - _position.x);
	int16 dy = std::abs(pos.y - _position.y);
	return (dx <= 1 and dy <= 1);
}
