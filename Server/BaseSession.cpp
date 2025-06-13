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

void Creature::SetPosition(Position pos)
{
	_position = pos;
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

void Creature::TakeDamage(uint16 damage)	// 만약 실제 들어간 데미지가 필요하면 이를 수정하거나 메서드를 추가하여야 함
{
	uint16 expected = _hp.load();
	uint16 desired;
	do {
		if (_state != GameState::ST_DEAD and expected <= 0) {
			SetDead();
			return;
		}

		int32 calc = static_cast<int32>(expected) - static_cast<int32>(damage);
		desired = static_cast<uint16>(std::max<uint16>(calc, 0));
	} while (not _hp.compare_exchange_strong(expected, desired));

	if (_state != GameState::ST_DEAD and _hp <= 0) {
		SetDead();
		return;
	}

	// TODO: 자식 클래스에서 Override하여 각자 알맞은 것을 수행해야 함
	//		 Session은 피격 당함 Send
	//		 Bot은 딱히...? <- Broadcast 해주어야 함
}

bool Creature::SetDead()
{
	if (_state == GameState::ST_DEAD) return false;

	uint8 now_state = _state;
	return _state.compare_exchange_strong(now_state, GameState::ST_DEAD);

	// TODO: 이도 상속 받아 Bot에서는 State Machine에서 Dead 처리를 해야 할 것으로 보임
}
