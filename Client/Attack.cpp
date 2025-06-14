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
		break;

	case AttackType::WARRIOR_S:
		SetSprite("warrior_s");
		SetSize(OBJECT_SIZE * 5);
		break;

	case AttackType::ROGUE_S:
		SetSprite("rogue_s");
		SetSize(OBJECT_SIZE * 1);
		break;

	case AttackType::SORCERER_S:
		SetSprite("sorcerer_s");
		SetSize(OBJECT_SIZE * 5);
		break;

	}
	_duration = 500000;
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

	sf::FloatRect bounds = _attack_obj.getLocalBounds();
	_attack_obj.setOrigin(bounds.width / 2.f, bounds.height / 2.f);
}

void Attack::SetActive(sf::Vector2i pos, uint8 dir)
{
	// TODO: dir에 맞게 위치 수정 및 방향 flip 해주어야 함

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
