#pragma once


class Button
{
private:
	sf::Sprite		button_sprite;
	sf::Text		button_text;
	uint32			button_id;

public:
	Button();
	Button(uint32 b_id);

	void Draw(sf::RenderWindow& window);

	void SetSize(float size);
	void SetText(std::string text);
	void SetTexture(const sf::Texture& texture);
	void SetOrigin();
	void SetPosition(sf::Vector2f pos);

	uint32 GetBID() const;

};

