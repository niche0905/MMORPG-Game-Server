#pragma once
#include "Object.h"


class World;


class Player : public Object
{
private:
	// TODO : 멤버 변수 추가
	uint64 id;

public:
	Player();
	Player(std::shared_ptr<World> world);
	Player(std::shared_ptr<World> world, uint64 id);

	void Init();

	uint64 GetID() const;

	void SetDummy();
	void SetMonster();

};

