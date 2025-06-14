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
	_attack_obj.SetSize(OBJECT_SIZE * 3);	// TODO: 이겄도 빼내어야 함
}

bool Attack::operator<(const Attack& other) const
{
	return _remove_time;
}

void Attack::SetActive(sf::Vector2i pos, int64 t_point)
{
	_attack_obj.ForceMove(pos);
	_remove_time = t_point;

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
