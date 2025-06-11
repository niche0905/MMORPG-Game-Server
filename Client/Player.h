#pragma once
#include "Object.h"
#include "Chat.h"


class World;
class Chat;


class Creature : public Object
{
private:
	// TODO : 멤버 변수 추가
	uint64 id;

	std::list<Chat> _chattings;

public:
	Creature();
	Creature(std::shared_ptr<World> world);
	Creature(std::shared_ptr<World> world, uint64 id);

	void Init();

	void Update(const int64 delta_time);

	void Draw(sf::RenderWindow& window);

	uint64 GetID() const;

	void SetDummy();
	void SetMonster();

	void AddChat(std::string_view chat);

};

