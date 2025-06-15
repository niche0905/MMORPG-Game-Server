#include "pch.h"
#include "Attack.h"
#include "TextureManager.h"

Attack::Attack()
{

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
		SetSprite("standard_atk");
		SetSize(OBJECT_SIZE * 3);
		SetOrigin(true);
		break;

	case AttackType::WARRIOR_S:
		SetSprite("warrior_s");
		SetSize(OBJECT_SIZE * 5);
		SetOrigin(true);
		break;

	case AttackType::ROGUE_S:
		SetSprite("rogue_s");
		SetSize(OBJECT_SIZE * 1);
		_diretional_atk = true;
		SetOrigin(not _diretional_atk);
		break;

	case AttackType::SORCERER_S:
		SetSprite("sorcerer_s");
		SetSize(OBJECT_SIZE * 5);
		_diretional_atk = true;
		SetOrigin(not _diretional_atk);
		break;

	}
	_duration = 500000;
}

void Attack::SetActive(sf::Vector2i pos, uint8 dir)
{
	if (_diretional_atk) {
		switch (dir)
		{
		case AttackDirection::DIR_LEFT:
			_attack_obj.setRotation(180.f);
			break;
		case AttackDirection::DIR_RIGHT:
			_attack_obj.setRotation(0.f);
			break;
		case AttackDirection::DIR_UP:
			_attack_obj.setRotation(270.f);
			break;
		case AttackDirection::DIR_DOWN:
			_attack_obj.setRotation(90.f);
			break;
		default:
			// None
			break;
		}
	}

	sf::Vector2f now_position(pos.x * TILE_SIZE, pos.y * TILE_SIZE);
	sf::Vector2f offset = { TILE_SIZE / 2.f, TILE_SIZE / 2.f };
	sf::Vector2f sprite_pos = now_position + offset;
	_attack_obj.setPosition(sprite_pos);

	_remove_time = _duration;
}

void Attack::Draw(sf::RenderWindow& window)
{
	if (Validate())
		window.draw(_attack_obj);
}

void Attack::Update(const int64 delta_time)
{
	_remove_time -= delta_time;
}

bool Attack::Validate()
{
	return _remove_time > 0;
}

void Attack::SetSprite(const std::string& key)
{
	_attack_obj.setTexture(TextureManager::Instance().GetTexture(key));
}

void Attack::SetSize(float size)
{
	const sf::Texture* tex = _attack_obj.getTexture();
	if (!tex) return;

	sf::Vector2u texture_size = tex->getSize();
	if (texture_size.x == 0 || texture_size.y == 0) return;

	_attack_obj.setScale(size / texture_size.x, size / texture_size.y);
}

void Attack::SetOrigin(bool mid)
{
	if (mid) {
		sf::FloatRect bounds = _attack_obj.getLocalBounds();
		_attack_obj.setOrigin(bounds.width / 2.f, bounds.height / 2.f);
	}
	else {
		sf::FloatRect bounds = _attack_obj.getLocalBounds();
		_attack_obj.setOrigin(0.f, bounds.height / 2.f);
	}
}
