#include "pch.h"
#include "Player.h"
#include "TextureManager.h"
#include "World.h"
#include "Chat.h"
#include "FontManager.h"


Creature::Creature()
	: Object{}
	, id{ 0 }
{
	Init();
}

Creature::Creature(std::shared_ptr<World> world)
	: Object{ world }
	, id{ 0 }
{
	Init();
}

Creature::Creature(std::shared_ptr<World> world, uint64 id)
	: Object{ world }
	, id{ id }
{
	Init();
}

void Creature::Init()
{
	SetSize(PLAYER_SIZE);
	SetSprite("player");

	_hp_bg.setSize(sf::Vector2f(HP_WIDTH, HP_HEIGHT));
	_hp_bg.setFillColor(sf::Color::Black);
	_hp_bg.setOrigin(0.f, 0.f);
	_hp_fg.setSize(sf::Vector2f(HP_WIDTH, HP_HEIGHT));
	_hp_fg.setFillColor(sf::Color::Red);
	_hp_fg.setOrigin(0.f, 0.f);

	_level_text.setFont(FontManager::Instance().GetFont("neodot"));
	_level_text.setCharacterSize(24);
	_level_text.setFillColor(sf::Color::Yellow);
	_level_text.setStyle(sf::Text::Regular);
	_level_text.setOutlineColor(sf::Color::Black);
	_level_text.setOutlineThickness(0.1f);
}

void Creature::Update(const int64 delta_time)
{
	_a_atk.Update(delta_time);
	_s_atk.Update(delta_time);
	_d_atk.Update(delta_time);

	for (auto it = _chattings.begin(); it != _chattings.end(); ) {
		it->Update(delta_time);
		if (not it->Validate()) {
			it = _chattings.erase(it);
		}
		else {
			++it;
		}
	}

	for (auto it = _damages.begin(); it != _damages.end(); ) {
		it->Update(delta_time);
		if (not it->Validate()) {
			it = _damages.erase(it);
		}
		else {
			++it;
		}
	}
}

void Creature::Draw(sf::RenderWindow& window)
{
	Object::Draw(window);

	DrawLevel(window);
	DrawHP(window);
	DrawAttacks(window);
	DrawChatting(window);
	DrawDamages(window);
}

void Creature::DrawLevel(sf::RenderWindow& window)
{
	sf::Vector2f pos = name_text.getPosition();
	sf::FloatRect bounds = name_text.getLocalBounds();
	float offset = 10.f;
	_level_text.setPosition({ pos.x - (bounds.width / 2.f) - offset, pos.y });

	window.draw(_level_text);
}

void Creature::DrawHP(sf::RenderWindow& window)
{
	sf::Vector2f hp_pos = sf::Vector2f{ static_cast<float>(position.x * TILE_SIZE + (TILE_SIZE / 2)), static_cast<float>(position.y * TILE_SIZE + (TILE_SIZE / 2)) };
	sf::Vector2f hp_offset = sf::Vector2f{ -(HP_WIDTH / 2), (TILE_SIZE - (TILE_SIZE - PLAYER_SIZE)) / 2};
	hp_pos += hp_offset;
	_hp_bg.setPosition(hp_pos);
	window.draw(_hp_bg);
	_hp_fg.setPosition(hp_pos);
	window.draw(_hp_fg);
}

void Creature::DrawAttacks(sf::RenderWindow& window)
{
	_a_atk.Draw(window);
	_s_atk.Draw(window);
	_d_atk.Draw(window);
}

void Creature::DrawChatting(sf::RenderWindow& window)
{
	if (_chattings.size() == 0) return;

	int32 chat_num = _chattings.size();
	float chat_size = 10.f;
	float chat_offset = 5.f;
	sf::Vector2f name_pos = sf::Vector2f{ static_cast<float>(position.x * TILE_SIZE + (TILE_SIZE / 2)), static_cast<float>(position.y * TILE_SIZE + (-TILE_SIZE / 2) - chat_offset) };
	name_pos.y -= chat_num * chat_size;
	for (auto& chat : _chattings) {
		chat.SetPosition(name_pos);
		name_pos.y += chat_size;

		chat.Draw(window);
	}
}

void Creature::DrawDamages(sf::RenderWindow& window)
{
	if (_damages.size() == 0) return;

	int32 damage_num = _damages.size();
	float damage_size = 8.f;
	float damage_offset_y = 5.f;
	sf::Vector2f name_pos = sf::Vector2f{ static_cast<float>(position.x * TILE_SIZE + (TILE_SIZE / 2)), static_cast<float>(position.y * TILE_SIZE + (-TILE_SIZE / 2) - damage_offset_y) };
	name_pos.y -= damage_num * damage_size;
	for (auto& damage : _damages) {
		damage.SetPosition(name_pos);
		name_pos.y += damage_size;

		damage.Draw(window);
	}
}

uint64 Creature::GetID() const
{
	return id;
}

uint16 Creature::GetHP() const
{
	return hp;
}

void Creature::SetVisualInfo(uint8 visual_info)
{
	_visual_info = visual_info;
	switch (_visual_info)
	{
	case VisualInfo::VI_WARRIOR:
		SetSprite("warrior");
		break;
	case VisualInfo::VI_ROGUE:
		SetSprite("rogue");
		break;
	case VisualInfo::VI_SORCERER:
		SetSprite("sorcerer");
		break;
	case VisualInfo::VI_GRAVE:
		SetSprite("grave");
		break;
	case VisualInfo::VI_MONSTER:
		SetSprite("monster");
		break;
	case VisualInfo::VI_SLIME:
		SetSprite("slime");
		break;
	case VisualInfo::VI_NEPENTHES:
		SetSprite("nepenthes");
		break;
	case VisualInfo::VI_DOG:
		SetSprite("dog");
		break;
	case VisualInfo::VI_BEAR:
		SetSprite("bear");
		break;
	case VisualInfo::VI_HELLO:
		SetSprite("hello");
		break;
	case VisualInfo::VI_KNIGHT:
		SetSprite("knight");
		break;
	case VisualInfo::VI_ACTION:
		SetSprite("action");
		break;
		
	}
}

void Creature::FixAttack()
{
	_a_atk.FixOrigin();
	_s_atk.FixOrigin();
	_d_atk.FixOrigin();
}

void Creature::SetClassType(uint8 class_type)
{
	_class_type = class_type;
	switch (class_type)
	{
	case ClassType::WARRIOR:
		_a_atk.SetAtkType(AttackType::STANDARD_ATK);
		_s_atk.SetAtkType(AttackType::WARRIOR_S);
		break;
	case ClassType:: ROGUE:
		_a_atk.SetAtkType(AttackType::STANDARD_ATK);
		_s_atk.SetAtkType(AttackType::ROGUE_S);
		break;
	case ClassType::SORCERER:
		_a_atk.SetAtkType(AttackType::STANDARD_ATK);
		_s_atk.SetAtkType(AttackType::SORCERER_S);
		break;
	case ClassType::NEPENTHES_MONSTER:
		_a_atk.SetAtkType(AttackType::FIXED_A);
		break;
	case ClassType::DOG_MONSTER:
		_a_atk.SetAtkType(AttackType::AGRO_A);
		break;
	case ClassType::BEAR_MONSTER:
		_a_atk.SetAtkType(AttackType::NEUT_A);
		break;
	case ClassType::KNIGHT_NPC:
		_a_atk.SetAtkType(AttackType::KNIGHT_A);
		break;
	default:
		break;
	}
}

uint8 Creature::GetClassType() const
{
	return _class_type;
}

void Creature::AddChat(std::string_view chat)
{
	_chattings.emplace_back(chat, 2000000);
}

void Creature::AddDamage(uint16 damage, bool heal)
{
	_damages.emplace_back(std::to_string(damage), 500000);
	_damages.back().SetSize(20, 0.3);
	if (heal) {
		_damages.back().SetColor(sf::Color::Green);
	}
	else {
		_damages.back().SetColor(_damage_color);
	}
}

void Creature::ShowAttack(uint8 atk_key, uint8 atk_dir)
{
	// TODO: class 나머지 공격 만들어야 함
	switch (atk_key)
	{
	case KeyType::KEY_A:
		_a_atk.SetActive(position, atk_dir);
		break;
	case KeyType::KEY_S:
		_s_atk.SetActive(position, atk_dir);
		break;
	case KeyType::KEY_D:
		_d_atk.SetActive(position, atk_dir);
		break;
	}
}

void Creature::ChangeHP(uint16 hp_)
{
	hp = hp_;
	float hp_size = (static_cast<float>(hp) / max_hp);

	_hp_fg.setSize(sf::Vector2f(HP_WIDTH * hp_size, HP_HEIGHT));
}

void Creature::SetDamageColor(sf::Color color)
{
	_damage_color = color;
}

void Creature::SetMaxHP(uint16 max_hp_)
{
	max_hp = max_hp_;
}

void Creature::SetLevel(uint8 level)
{
	_level = level;
	LevelUpdate();
}

void Creature::LevelUpdate()
{
	_level_text.setString(std::to_string(_level));
	sf::FloatRect bounds = _level_text.getLocalBounds();
	_level_text.setOrigin(bounds.width, bounds.height / 2.f);
}

uint8 Creature::GetLevel() const
{
	return _level;
}