#include "pch.h"
#include "BaseSession.h"


Creature::Creature()
	: Creature(-1)
{

}

Creature::Creature(int64 id)
	: _id{ id }
	, _basic_stats{}
	, _temp_stats{}
{

}

Creature::~Creature()
{

}

int64 Creature::GetID() const
{
	return _id;
}
