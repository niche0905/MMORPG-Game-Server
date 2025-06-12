#pragma once


class Chat
{
private:
	sf::Text	_text;
	int64		_remove_time;

public:
	Chat();
	Chat(std::string text, int64 t_point);
	Chat(std::string_view text, int64 t_point);

	bool operator<(const Chat& other) const;

	void SetSize(uint32 size, float thick);
	void SetColor(const sf::Color& color);

	void SetPosition(sf::Vector2f pos);
	void Draw(sf::RenderWindow& window);

	void Update(const int64 delta_time);
	bool Validate() const;

};

