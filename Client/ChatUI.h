#pragma once
#include <deque>


class ChatBox
{
private:
	bool					_is_active = false;		// 활성화 상태
	std::string				_input_buffer;			// 입력 버퍼

	sf::RectangleShape		_input_box;				// 인풋 창
	sf::RectangleShape		_chat_box;				// 채팅 창
	sf::Text				_input_text;			// 인풋 텍스트
	std::deque<sf::Text>	_messages;				// 채팅들
	const size_t			_max_messages = 10;		// 최대 메시지 수

public:
	ChatBox();

	void HandleInput(const sf::Event& event);
	void UpdateInputText();
	void Draw(sf::RenderWindow& window);
	void AddMessage(const std::string& message);
	
	bool IsActive() const;

	void FetchSendMessage();
	
};

