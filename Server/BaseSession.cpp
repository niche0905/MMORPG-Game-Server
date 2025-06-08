#include "pch.h"
#include "BaseSession.h"


Creature::Creature()
	: Creature{ -1, 0, 0 }
{

}

Creature::Creature(int64 id)
	: Creature{ id, 0, 0, false }
{

}

Creature::Creature(int64 id, bool is_pc)
	: Creature{ id, 0, 0, is_pc }
{
}

Creature::Creature(int64 id, int16 x, int16 y)
	: Creature{ id, x, y, false }
{

}

Creature::Creature(int64 id, int16 x, int16 y, bool is_pc)
	: _id{ id }
	, _position{ x, y }
	, _is_pc{ is_pc }
{

}

Creature::Creature(int64 id, Position pos)
	: Creature{ id, pos, false }
{

}

Creature::Creature(int64 id, Position pos, bool is_pc)
	: _id{ id }
	, _position{ pos }
	, _is_pc{ is_pc }
{

}

int64 Creature::GetID() const
{
	return _id;
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

uint8 Creature::GetState() const
{
	return _state.load();
}
