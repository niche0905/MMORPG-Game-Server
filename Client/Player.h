#pragma once
#include "Object.h"


class World;


class Creature : public Object
{
private:
	// TODO : 멤버 변수 추가
	uint64 id;

public:
	Creature();
	Creature(std::shared_ptr<World> world);
	Creature(std::shared_ptr<World> world, uint64 id);

	void Init();

	uint64 GetID() const;

	void SetDummy();
	void SetMonster();

};

