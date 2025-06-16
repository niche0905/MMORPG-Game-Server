#include "pch.h"
#include "FixedMonster.h"
#include "DeadState.h"
#include "AttackState.h"
#include "FM_IdleState.h"


FixedMonster::FixedMonster()
	: FixedMonster{ INVALID_ID }
{

}

FixedMonster::FixedMonster(uint64 id)
	: Monster{ id, true, false, true }
{
	_class_type = ClassType::NEPENTHES_MONSTER;
	_visual_type = VisualInfo::VI_NEPENTHES;
	_fsm.ChangeState(this, &FM_IdleState::Instance());
}

FixedMonster::~FixedMonster()
{

}

bool FixedMonster::TakeDamage(uint64 id, uint16 damage)
{
	bool my_kill = Bot::TakeDamage(id, damage);

	_target = server.GetClients()[id];

	return my_kill;
}

void FixedMonster::Update()
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
		_fsm.ChangeState(this, &FM_IdleState::Instance());
	}
	else {
		_fsm.ChangeState(this, &AttackState::Instance());
	}

	_fsm.Update(this);
}

void FixedMonster::Attack()
{
	const uint16 damage = 10;

	if (_target) {

		if (_target->CanSee(_position, FIX_MONSTER_ATK_RANGE)) {

			SC_ATTACK_PACKET attack_packet{ _id, KeyType::KEY_A, AttackDirection::NO_DIRECTION };

			std::unordered_set<uint64> closed_clients = server.GetClientList(_position);

			for (uint64 client_id : closed_clients) {

				if (client_id == _id) continue;

				Creature* client = server.GetClients()[client_id];
				if (client == nullptr) continue;

				if (::IsNPC(client_id)) {
					Bot* npc = static_cast<Bot*>(client);
					if (npc->IsInvincibility() or not npc->IsFriendly()) continue;
				}

				uint8 state = client->GetState();
				if (state == GameState::ST_ALLOC or state == GameState::ST_CLOSE) continue;

				if (not client->CanSee(_position, VIEW_RANGE)) continue;

				if (::IsPlayer(client_id)) {

					Session* session = static_cast<Session*>(client);
					session->Send(&attack_packet);
				}

				if (state == GameState::ST_DEAD) continue;

				// 공격 범위 판정!
				Position client_pos = client->GetPosition();
				int16 dx = std::abs(client_pos.x - _position.x);
				int16 dy = std::abs(client_pos.y - _position.y);
				if (dx <= 2 and dy <= 2) {
					// 맞은 것임

					client->TakeDamage(_id, damage);
				}
				
			}
		}

		if (not _target->CanSee(_position, VIEW_RANGE) or _target->GetState() == GameState::ST_DEAD) {	// TODO: 어그로 풀리는 범위를 적용하고 싶다면 상수 정의 ㄱㄱ
			_target = nullptr;
		}
	}
}

void FixedMonster::ReviveChangeState()
{
	_fsm.ChangeState(this, &FM_IdleState::Instance());
}
