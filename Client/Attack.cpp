#include "pch.h"
#include "Attack.h"
#include "TextureManager.h"

Attack::Attack()
	: Attack{ "standard_atk" }
{

}

Attack::Attack(std::string text)
{
	_attack_obj.Hide();
	_attack_obj.SetSprite(text);
	_attack_obj.SetSize(OBJECT_SIZE * 3);
}

Attack::Attack(uint8 atk_type)
{
	_attack_obj.Hide();
	SetAtkType(atk_type);
}

bool Attack::operator<(const Attack& other) const
{
	return _remove_time;
}

void Attack::SetAtkType(uint8 atk_type)
{
	switch (atk_type)
	{
	case AttackType::STANDARD_ATK:
		_attack_obj.SetSprite("standard_atk");
		_attack_obj.SetSize(OBJECT_SIZE * 3);
		break;

	case AttackType::WARRIOR_S:
		_attack_obj.SetSprite("warrior_s");
		_attack_obj.SetSize(OBJECT_SIZE * 5);
		break;

	case AttackType::ROGUE_S:
		_attack_obj.SetSprite("rogue_s");
		_attack_obj.SetSize(OBJECT_SIZE * 1);
		break;

	case AttackType::SORCERER_S:
		_attack_obj.SetSprite("sorcerer_s");
		_attack_obj.SetSize(OBJECT_SIZE * 5);
		break;

	}
	_duration = 500000;
}

void Attack::SetTexture(const std::string& key)
{
	_attack_obj.SetSprite(key);
}

void Attack::SetSize(float size)
{
	_attack_obj.SetSize(size);
}

void Attack::SetActive(sf::Vector2i pos, uint8 dir)
{
	// TODO: dir에 맞게 위치 수정 및 방향 flip 해주어야 함
	_attack_obj.ForceMove(pos);

	_remove_time = _duration;

	_attack_obj.Show();
}

void Attack::Draw(sf::RenderWindow& window)
{
	_attack_obj.Draw(window);
}

void Attack::Update(const int64 delta_time)
{
	_remove_time -= delta_time;
	Validate();
}

bool Attack::Validate()
{
	bool validate = _remove_time > 0;
	if (not validate)
		_attack_obj.Hide();

	return validate;
}
