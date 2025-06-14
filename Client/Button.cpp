#include "pch.h"
#include "Button.h"
#include "FontManager.h"


Button::Button()
	: Button{0}
{

}

Button::Button(uint32 b_id)
	: button_id{ b_id }
{
	button_text.setFont(FontManager::Instance().GetFont("neodot"));

	button_text.setFillColor(sf::Color::White);
	button_text.setOutlineColor(sf::Color::Black);
	button_text.setStyle(sf::Text::Regular);
	button_text.setOutlineThickness(2.0f);
}

void Button::Draw(sf::RenderWindow& window)
{
	window.draw(button_sprite);
	window.draw(button_text);
}

void Button::SetSize(float size)
{
	if (button_sprite.getTexture() == nullptr) return;

	sf::Vector2u texture_size = button_sprite.getTexture()->getSize();

	button_sprite.setScale(size / texture_size.x, size / texture_size.y);

	button_text.setCharacterSize(static_cast<int>(size / 10));
	SetOrigin();
}

void Button::SetText(std::string text)
{
	button_text.setString(text);
	SetOrigin();
}

void Button::SetTexture(const sf::Texture& texture)
{
	button_sprite.setTexture(texture);
	SetOrigin();
}

void Button::SetOrigin()
{
	sf::FloatRect bounds = button_sprite.getLocalBounds();
	button_sprite.setOrigin(bounds.width / 2.f, bounds.height / 2.f);
	bounds = button_text.getLocalBounds();
	button_text.setOrigin(bounds.width / 2.f, bounds.height / 2.f);
}

void Button::SetPosition(sf::Vector2f pos)
{
	button_sprite.setPosition(pos);
	button_text.setPosition(pos);
}
