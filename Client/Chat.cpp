#include "pch.h"
#include "Chat.h"
#include "FontManager.h"


Chat::Chat()
	: Chat{ std::string(""), 2000000 }
{

}

Chat::Chat(std::string text, int64 t_point)
	: _remove_time{ t_point }
{
	_text.setString(text);
	_text.setFont(FontManager::Instance().GetFont("neodot"));
	_text.setCharacterSize(12);
	_text.setFillColor(sf::Color::White);
	_text.setStyle(sf::Text::Regular);
	_text.setOutlineColor(sf::Color::Black);
	_text.setOutlineThickness(0.2f);
	sf::FloatRect bounds = _text.getLocalBounds();
	_text.setOrigin(bounds.width / 2.f, bounds.height / 2.f);
}

Chat::Chat(std::string_view text, int64 t_point)
	: Chat{ std::string(text), t_point }
{

}

bool Chat::operator<(const Chat& other) const
{
	return _remove_time > other._remove_time;
}

void Chat::SetSize(uint32 size, float thick)
{
	_text.setCharacterSize(size);
	_text.setOutlineThickness(thick);
	sf::FloatRect bounds = _text.getLocalBounds();
	_text.setOrigin(bounds.width / 2.f, bounds.height / 2.f);
}

void Chat::SetColor(const sf::Color& color)
{
	_text.setFillColor(color);
}

void Chat::SetPosition(sf::Vector2f pos)
{
	_text.setPosition(pos);
}

void Chat::Draw(sf::RenderWindow& window)
{
	window.draw(_text);
}

void Chat::Update(const int64 delta_time)
{
	_remove_time -= delta_time;
}

bool Chat::Validate() const
{
	return _remove_time > 0;
}
