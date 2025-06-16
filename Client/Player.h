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

	uint8 _visual_info = VISUAL_START;

	uint16 hp = 100;
	uint16 max_hp = 100;
	sf::RectangleShape _hp_bg;		// HP 배경
	sf::RectangleShape _hp_fg;		// HP 전면

	uint8			_class_type;	// class type
	Attack			_a_atk;
	Attack			_s_atk;
	Attack			_d_atk;

	std::list<Chat> _chattings;

	sf::Color		_damage_color{ sf::Color::Red };
	std::list<Chat> _damages;

public:
	Creature();
	Creature(std::shared_ptr<World> world);
	Creature(std::shared_ptr<World> world, uint64 id);

	void Init();

	void Update(const int64 delta_time);

	void Draw(sf::RenderWindow& window);
	void DrawHP(sf::RenderWindow& window);
	void DrawAttacks(sf::RenderWindow& window);
	void DrawChatting(sf::RenderWindow& window);
	void DrawDamages(sf::RenderWindow& window);

	uint64 GetID() const;
	uint16 GetHP() const;

	void SetVisualInfo(uint8 visual_info);

	void FixAttack();
	void SetClassType(uint8 class_type);
	uint8 GetClassType() const;

	void AddChat(std::string_view chat);
	void AddDamage(uint16 damage);
	void ShowAttack(uint8 atk_type, uint8 atk_dir);
	void ChangeHP(uint16 hp);

	void SetDamageColor(sf::Color color);

	void SetMaxHP(uint16 max_hp);
	
};

