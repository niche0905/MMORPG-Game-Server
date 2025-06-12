#pragma once
#include "Object.h"
#include "Chat.h"
#include "Attack.h"


class World;
class Chat;
class Attack;


class Creature : public Object
{
private:
	// TODO : 멤버 변수 추가
	uint64 id;

	uint16 hp = 100;

	Attack			_a_atk;

	std::list<Chat> _chattings;
	std::list<Chat> _damages;

public:
	Creature();
	Creature(std::shared_ptr<World> world);
	Creature(std::shared_ptr<World> world, uint64 id);

	void Init();

	void Update(const int64 delta_time);

	void Draw(sf::RenderWindow& window);
	void DrawAttacks(sf::RenderWindow& window);
	void DrawChatting(sf::RenderWindow& window);
	void DrawDamages(sf::RenderWindow& window);

	uint64 GetID() const;

	// TODO: 바꾸어야 함
	void SetDummy();
	void SetMonster();

	void AddChat(std::string_view chat);
	void AddDamage(uint16 damage);
	void ShowAttack(uint8 atk_type);
	void ChangeHP(uint16 hp);

};

