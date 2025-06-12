#pragma once


class Attack
{
private:
	Object			_attack_obj;
	int64			_remove_time;

public:
	Attack();
	Attack(std::string text, sf::Vector2i pos, int64 t_point);

	bool operator<(const Attack& other) const;

	void Draw(sf::RenderWindow& window);

	void Update(const int64 delta_time);
	bool Validate() const;

};

