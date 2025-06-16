#include "pch.h"
#include "Dialog.h"
#include "FontManager.h"


Dialog::Dialog()
	: _button_index{ 0 }
	, _max_index{ 2 }
{
	_dialog_box.setSize({ WINDOW_WIDTH, 300.f });
	_dialog_box.setOrigin({ 0.f, 300.f });
	_dialog_box.setPosition(0.f, WINDOW_HEIGHT);
	_dialog_box.setFillColor(sf::Color(0, 0, 0, 128));

	_text.setFont(FontManager::Instance().GetFont("neodot"));
	_text.setString("Return Place?");
	_text.setCharacterSize(40);
	_text.setFillColor(sf::Color::Black);
	_text.setStyle(sf::Text::Regular);
	_text.setOutlineColor(sf::Color::White);
	_text.setOutlineThickness(1.0f);
	sf::FloatRect bounds = _text.getLocalBounds();
	_text.setOrigin({ bounds.width / 2.f, bounds.height });
	_text.setPosition({ WINDOW_WIDTH / 2.f, WINDOW_HEIGHT - 220.f });

	_yes_text.setFont(FontManager::Instance().GetFont("neodot"));
	_yes_text.setString("Yes");
	_yes_text.setCharacterSize(30);
	_yes_text.setFillColor(sf::Color::Black);
	_yes_text.setStyle(sf::Text::Regular);
	_yes_text.setOutlineColor(sf::Color::White);
	_yes_text.setOutlineThickness(1.0f);
	sf::FloatRect bounds2 = _yes_text.getLocalBounds();
	_yes_text.setOrigin({ bounds2.width / 2.f, bounds2.height });
	_yes_text.setPosition({ 400.f , WINDOW_HEIGHT - 80.f });

	_no_text.setFont(FontManager::Instance().GetFont("neodot"));
	_no_text.setString("No");
	_no_text.setCharacterSize(30);
	_no_text.setFillColor(sf::Color::Black);
	_no_text.setStyle(sf::Text::Regular);
	_no_text.setOutlineColor(sf::Color::White);
	_no_text.setOutlineThickness(1.0f);
	sf::FloatRect bounds3 = _no_text.getLocalBounds();
	_no_text.setOrigin({ bounds3.width / 2.f, bounds3.height });
	_no_text.setPosition({ 1200.f , WINDOW_HEIGHT - 80.f });

	_button_box.setFillColor(sf::Color::White);
	_button_box.setSize({ 300.0f , bounds3.height*1.5f });
	sf::FloatRect bounds4 = _button_box.getLocalBounds();
	_button_box.setOrigin({ bounds4.width / 2.f , bounds4.height / 2.f });
	UpdateButtonSelect();
}

void Dialog::HandleInput(const sf::Event& event)
{
	if (not _is_active) return;
	
	if (event.type == sf::Event::KeyPressed) {

		if (event.key.code == sf::Keyboard::Left) {
			_button_index -= 1;
			_button_index = ((_button_index + _max_index) % _max_index);
			UpdateButtonSelect();
		}
		if (event.key.code == sf::Keyboard::Right) {
			_button_index += 1;
			_button_index = (_button_index % _max_index);
			UpdateButtonSelect();
		}
		if (event.key.code == sf::Keyboard::Enter) {
			// TODO: confirm
			_is_active = false;
		}
		if (event.key.code == sf::Keyboard::Escape) {
			_is_active = false;
		}
	}
}

void Dialog::UpdateButtonSelect()
{
	float pos_x = 400.f + _button_index * 800.f;

	_button_box.setPosition({ pos_x,WINDOW_HEIGHT - 75.f });
}

void Dialog::Draw(sf::RenderWindow& window)
{
	if (_is_active) {

		window.draw(_dialog_box);
		window.draw(_button_box);

		window.draw(_text);
		window.draw(_yes_text);
		window.draw(_no_text);
	}
}

void Dialog::SetActive(bool active)
{
	_is_active = active;
}

bool Dialog::IsActive() const
{
	return _is_active;
}
