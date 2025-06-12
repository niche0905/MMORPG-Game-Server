#include "pch.h"
#include "Attack.h"
#include "TextureManager.h"

Attack::Attack()
	: Attack{ "", {0,0}, 2000000 }
{

}

Attack::Attack(std::string text, sf::Vector2i pos, int64 t_point)
	: _remove_time{ t_point }
{
	TextureManager::Instance().LoadTexture("standard_atk", "./Resource/Texture/standard_atk.png");
	_attack_obj.SetSprite("standard_atk");
	_attack_obj.ForceMove(pos);
}

bool Attack::operator<(const Attack& other) const
{
	return _remove_time;
}

void Attack::Draw(sf::RenderWindow& window)
{
	_attack_obj.Draw(window);
}

void Attack::Update(const int64 delta_time)
{
	_remove_time -= delta_time;
}

bool Attack::Validate() const
{
	return _remove_time > 0;
}
