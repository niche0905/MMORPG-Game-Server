#pragma once


class Attack
{
private:
	Object			_attack_obj;
	int64			_remove_time = 0;

public:
	Attack();
	Attack(std::string text);

	bool operator<(const Attack& other) const;

	void SetActive(sf::Vector2i pos, int64 t_point);

	void Draw(sf::RenderWindow& window);

	void Update(const int64 delta_time);
	bool Validate();

};

