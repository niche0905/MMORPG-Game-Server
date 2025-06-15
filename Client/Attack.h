#pragma once


class Attack
{
private:
	sf::Sprite		_attack_obj;
	int64			_remove_time = 0;
	
	bool			_diretional_atk = false;
	int64			_duration = 500000;		// 0.5s

public:
	Attack();

	bool operator<(const Attack& other) const;

	void SetAtkType(uint8 atk_type);

	void SetActive(sf::Vector2i pos, uint8 direction = NO_DIRECTION);

	void Draw(sf::RenderWindow& window);

	void FixOrigin();

	void Update(const int64 delta_time);
	bool Validate();

private:
	void SetSprite(const std::string& key);
	void SetSize(float size);
	void SetOrigin(bool mid);		// true : middle pivot, false, left-mid

};

