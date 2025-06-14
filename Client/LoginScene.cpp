#include "pch.h"
#include "LoginScene.h"
#include "FontManager.h"


LoginScene::LoginScene()
{
	Init();
}

void LoginScene::Init()
{
	float mid_window = WINDOW_WIDTH / 2.f;
	float mid_name = NAME_WIDTH / 2.f;
	float start_input_box = mid_window - mid_name;
	float offset_x = 10.f;
	_input_box.setFillColor(sf::Color::White);
	_input_box.setSize({ NAME_WIDTH , NAME_HEIGHT });
	_input_box.setPosition({ start_input_box , 400 });

	_nickname_text.setFont(FontManager::Instance().GetFont("neodot"));
	_nickname_text.setCharacterSize(24);
	_nickname_text.setFillColor(sf::Color::Black);
	_nickname_text.setOutlineColor(sf::Color::White);
	_nickname_text.setStyle(sf::Text::Regular);
	_nickname_text.setOutlineThickness(2.f);
	_nickname_text.setPosition({ start_input_box + offset_x, 400 });

	_system_text.setFont(FontManager::Instance().GetFont("neodot"));
	_system_text.setCharacterSize(40);
	_system_text.setFillColor(sf::Color::Red);
	_system_text.setOutlineColor(sf::Color::Black);
	_system_text.setStyle(sf::Text::Regular);
	_system_text.setOutlineThickness(2.f);
	sf::FloatRect bounds = _system_text.getLocalBounds();
	_system_text.setOrigin(bounds.width / 2.f, bounds.height / 2.f);
	_system_text.setPosition({ mid_window, 700 });
}

void LoginScene::Update(const int64 delta_time)
{

}

void LoginScene::Draw(sf::RenderWindow& window)
{
	window.draw(_input_box);
	window.draw(_nickname_text);
	window.draw(_system_text);
}

void LoginScene::HUD(sf::RenderWindow& window)
{

}

void LoginScene::HandleInput(const sf::Event& input_event)
{
	if (input_event.type == sf::Event::TextEntered) {
		char entered = static_cast<char>(input_event.text.unicode);
		if (entered >= 32 && entered <= 126 && _input_buffer.size() < 20) {
			_input_buffer += entered;
		}
		else if (entered == 8 && !_input_buffer.empty()) { // Backspace
			_input_buffer.pop_back();
		}
		UpdateNicknameText();
	}

	if (input_event.type == sf::Event::KeyPressed && input_event.key.code == sf::Keyboard::Enter) {
		if (!_input_buffer.empty()) {
			game.SendID(_input_buffer);
		}
	}
}

void LoginScene::ProcessPacket(std::vector<BYTE> packets)
{
	size_t processed_data_size = 0;

	while (processed_data_size < packets.size())
	{
		BYTE* packet = packets.data() + processed_data_size;

		BASE_PACKET* base_packet = reinterpret_cast<BASE_PACKET*>(packet);

		switch (base_packet->_packet_id)
		{

		case PacketID::S2C_LOGIN_ALLOW:
		{
			game.SceneLoad(SceneType::GameScene);
			game.ProcessPacket(packets);
		}
		break;

		case PacketID::S2C_LOGIN_FAIL:
		{
			SC_LOGIN_FAIL_PACKET* login_fail_packet = reinterpret_cast<SC_LOGIN_FAIL_PACKET*>(packet);
			switch (login_fail_packet->_reason)
			{
			case LoginFailReason::NO_IDEA:
			{
				_system_text.setString("No Idea login fail");
				UpdateSystemText();
			}
			break;
			case LoginFailReason::USED_ID:
			{
				_system_text.setString("that name is used");
				UpdateSystemText();
			}
			break;
			case LoginFailReason::INAPPOSITE_ID:
			{
				_system_text.setString("invalid text");
				UpdateSystemText();
			}
			break;
			case LoginFailReason::TO_MANY:
			{
				_system_text.setString("to many user in server");
				UpdateSystemText();
			}
			break;

			case LoginFailReason::GO_REGISTER:
			{
				game.SetName(_input_buffer);
				game.SceneLoad(SceneType::CreateScene);
			}
			break;
			}
		}
		break;

		default:
			// 이 짝으로 패킷이 오면 안댜
			std::cout << "Login Scene Recv Invalid Packet ERROR\n";
			break;
		}

		processed_data_size += base_packet->_size;
	}
}

void LoginScene::UpdateNicknameText()
{
	_nickname_text.setString(_input_buffer);
}

void LoginScene::UpdateSystemText()
{
	sf::FloatRect bounds = _system_text.getLocalBounds();
	_system_text.setOrigin(bounds.width / 2.f, bounds.height / 2.f);
}
