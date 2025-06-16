#include "pch.h"
#include "ChatUI.h"
#include "FontManager.h"


ChatBox::ChatBox()
{
	_chat_box.setSize({ 800.f, 150.f });
	_chat_box.setOrigin({ 0.f, 150.f });
	_chat_box.setPosition(0.f, WINDOW_HEIGHT - 30.f);
	_chat_box.setFillColor(sf::Color(0, 0, 0, 128));

	_input_box.setSize({ 800.f, 30.f });
	_input_box.setOrigin({ 0.f, 30.f });
	_input_box.setPosition(0.f, WINDOW_HEIGHT);
	_input_box.setFillColor(sf::Color(255, 255, 255, 200));

	_input_text.setFont(FontManager::Instance().GetFont("neodot"));
	_input_text.setCharacterSize(18);
	sf::FloatRect bounds = _input_text.getLocalBounds();
	_input_text.setFillColor(sf::Color::Black);
	_input_text.setPosition(10.f, WINDOW_HEIGHT - 30.f); // 입력 박스 안쪽 여백
}

void ChatBox::HandleInput(const sf::Event& ev)
{
	if (_is_active) {

		if (ev.type == sf::Event::TextEntered) {
			char entered = static_cast<char>(ev.text.unicode);
			if (entered >= 32 && entered <= 126 && _input_buffer.size() < 80) {
				_input_buffer += entered;
			}
			else if (entered == 8 && !_input_buffer.empty()) { // Backspace
				_input_buffer.pop_back();
			}
			UpdateInputText();
		}
	}

	if (ev.type == sf::Event::KeyPressed) {

		if (ev.key.code == sf::Keyboard::T) {
			if (_is_active == false)
				_is_active = true;
		}
		if (ev.key.code == sf::Keyboard::Enter) {
			FetchSendMessage();
		}
		if (ev.key.code == sf::Keyboard::Escape) {
			_is_active = false;
		}
	}
}

void ChatBox::UpdateInputText()
{
	_input_text.setString(_input_buffer);
}

void ChatBox::Draw(sf::RenderWindow& window)
{
	if (_is_active) {
		window.draw(_chat_box);
		window.draw(_input_box);
		window.draw(_input_text);

		for (const auto& msg : _messages) {
			window.draw(msg);
		}
	}

}

void ChatBox::AddMessage(const std::string& msg)
{
	sf::Text new_text;
	new_text.setFont(FontManager::Instance().GetFont("neodot"));
	new_text.setString(msg);
	new_text.setCharacterSize(18);		// TODO: chat_size를 생성자에서 받아서 설정
	new_text.setFillColor(sf::Color::White);
	sf::FloatRect bounds = new_text.getLocalBounds();

	float chat_height = 30.f;
	_messages.push_front(std::move(new_text));
	sf::Vector2f pos = { 10.f, WINDOW_HEIGHT - chat_height };
	for (auto& msg : _messages) {
		pos.y -= chat_height;
		msg.setPosition(pos);
	}

	if (_messages.size() > _max_messages)
		_messages.pop_back();
}

bool ChatBox::IsActive() const
{
	return _is_active;
}

void ChatBox::FetchSendMessage()
{
	_is_active = false;
	if (_input_buffer.empty()) return;

	if (game.HandleChatCommand(_input_buffer)) return;

	game.SendChatting(_input_buffer);
	_input_buffer.clear();
	UpdateInputText();
}
