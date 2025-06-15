#include "pch.h"
#include "BaseSession.h"


Creature::Creature()
	: Creature{ INVALID_ID, 0, 0 }
{

}

Creature::Creature(uint64 id)
	: Creature{ id, 0, 0, false }
{

}

Creature::Creature(uint64 id, bool is_pc)
	: Creature{ id, 0, 0, is_pc }
{
}

Creature::Creature(uint64 id, int16 x, int16 y)
	: Creature{ id, x, y, false }
{

}

Creature::Creature(uint64 id, int16 x, int16 y, bool is_pc)
	: _id{ id }
	, _position{ x, y }
	, _is_pc{ is_pc }
{

}

Creature::Creature(uint64 id, Position pos)
	: Creature{ id, pos, false }
{

}

Creature::Creature(uint64 id, Position pos, bool is_pc)
	: _id{ id }
	, _position{ pos }
	, _is_pc{ is_pc }
{

}

uint64 Creature::GetID() const
{
	return _id;
}

void Creature::SetPosition(int16 x, int16 y)
{
	_position = { x, y };
}

void Creature::SetPosition(const Position& pos)
{
	_position = pos;
}

void Creature::SetName(std::string_view name)
{
	_name = name;
}

std::string& Creature::GetName()
{
	return _name;
}

const std::string& Creature::GetName() const
{
	return _name;
}

Position Creature::GetPosition() const
{
	return _position;
}

bool Creature::CanSee(int16 x, int16 y, int16 gap) const
{
	if (std::abs(_position.x - x) > gap) return false;
	return (std::abs(_position.y - y) <= gap);
}

bool Creature::CanSee(Position pos, int16 gap) const
{
	if (std::abs(_position.x - pos.x) > gap) return false;
	return (std::abs(_position.y - pos.y) <= gap);
}

void Creature::SetState(uint8 state)
{
	_state.store(state);
}

uint8 Creature::GetState() const
{
	return _state.load();
}

uint16 Creature::GetHP() const
{
	return _hp;
}

bool Creature::TakeDamage(uint64 id, uint16 damage)	// 만약 실제 들어간 데미지가 필요하면 이를 수정하거나 메서드를 추가하여야 함
{
	uint16 expected = _hp.load();
	uint16 desired;
	do {
		if (_state != GameState::ST_DEAD and expected <= 0) {
			return false;
		}

		int32 calc = static_cast<int32>(expected) - static_cast<int32>(damage);
		desired = static_cast<uint16>(std::max<int32>(calc, 0));
	} while (not _hp.compare_exchange_strong(expected, desired));

	if (_state != GameState::ST_DEAD and _hp <= 0) {
		SetDead();
		return true;
	}
}

bool Creature::SetDead()
{
	if (_state == GameState::ST_DEAD) return false;

	uint8 now_state = _state;
	return _state.compare_exchange_strong(now_state, GameState::ST_DEAD);
}

void Creature::DeadSequence()
{
	// TODO: 죽으면 한번 Broadcast 하기
}

void Creature::SetClassType(uint8 class_type)
{
	_class_type = class_type;
}

uint8 Creature::GetClassType() const
{
	return _class_type;
}

uint8 Creature::GetVisualType() const
{
	return _visual_type;
}

uint8 Creature::GetLevel() const
{
	return _level;
}

bool Creature::ReviveBroadcast()
{
	std::unordered_set<uint64> view_list = server.GetClientList(_position);
	Position pos = _position;
	SC_MOVE_PACKET move_packet{ _id, pos.x, pos.y };
	SC_ENTER_PACKET enter_packet{ _id, pos.x, pos.y, _name.c_str(), GetMaxHP(), _hp, _visual_type, _class_type, _level};

	std::unordered_set<uint64> near_list;
	for (uint64 client_id : view_list) {

		if (::IsNPC(client_id)) continue;

		Creature* client = server.GetClients()[client_id];
		if (client == nullptr) continue;

		uint8 state = client->GetState();
		if (state == GameState::ST_ALLOC or state == GameState::ST_CLOSE) continue;

		if (not client->CanSee(pos, VIEW_RANGE)) continue;
		Session* session = static_cast<Session*>(client);
		
		session->ProcessCloseCreature(_id, &enter_packet, &move_packet);
		near_list.insert(client_id);
	}

	return (near_list.size() != 0);
}

void Creature::DoRevive()
{
	ReviveBroadcast();
	// TODO: Class에 맞게 override 해서 사용하기
	//		 Session 그냥 Revive 주위에 알리기
	//		 Bot 근처에 사람이 있다면 활동하기
}
