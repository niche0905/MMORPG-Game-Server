#pragma once
#include "Object.h"

constexpr double player_size{ 48.0 };

class World;


class Player : public Object
{
private:
	// TODO : ��� ���� �߰�
	uint64_t id;

public:
	Player();
	Player(std::shared_ptr<World> world);
	Player(std::shared_ptr<World> world, uint64_t id);

	void Init();

	uint64_t GetID() const;

	void SetDummy();

};

