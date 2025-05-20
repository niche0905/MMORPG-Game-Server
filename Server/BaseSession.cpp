#include "pch.h"
#include "BaseSession.h"


Creature::Creature()
	: Creature{ -1, 0, 0 }
{

}

Creature::Creature(int64 id)
	: Creature{ id, 0, 0 }
{

}

Creature::Creature(int64 id, int16 x, int16 y)
	: _id{ id }
	, _position{ x, y }
{

}

Creature::Creature(int64 id, Position pos)
	: _id{ id }
	, _position{ pos }
{
}

Creature::~Creature()
{

}

int64 Creature::GetID() const
{
	return _id;
}
