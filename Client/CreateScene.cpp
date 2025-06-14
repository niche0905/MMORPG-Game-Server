#include "pch.h"
#include "CreateScene.h"
#include "TextureManager.h"


CreateScene::CreateScene()
	: _button_index{ 0 }
	, _max_index{ 3 }
{
	Init();
}

void CreateScene::Init()
{
	float size = 300.f;
	sf::Vector2u texture_size;
	sf::FloatRect bounds;

	const float third_window_width = WINDOW_WIDTH / 3.f;
	const float half_window_height = WINDOW_HEIGHT / 2.f;

	_warrior_sprite.setTexture(TextureManager::Instance().GetTexture("warrior_big"));
	_rogue_sprite.setTexture(TextureManager::Instance().GetTexture("rogue_big"));
	_sorcerer_sprite.setTexture(TextureManager::Instance().GetTexture("sorcerer_big"));
	
	texture_size = _warrior_sprite.getTexture()->getSize();
	_warrior_sprite.setScale(size / texture_size.x, size / texture_size.y);
	bounds = _warrior_sprite.getLocalBounds();
	_warrior_sprite.setOrigin(bounds.width / 2.f, bounds.height / 2.f);
	_warrior_sprite.setPosition({0 + third_window_width / 2.f, half_window_height });


	texture_size = _rogue_sprite.getTexture()->getSize();
	_rogue_sprite.setScale(size / texture_size.x, size / texture_size.y);
	bounds = _rogue_sprite.getLocalBounds();
	_rogue_sprite.setOrigin(bounds.width / 2.f, bounds.height / 2.f);
	_rogue_sprite.setPosition({ third_window_width + third_window_width / 2.f, half_window_height });

	texture_size = _sorcerer_sprite.getTexture()->getSize();
	_sorcerer_sprite.setScale(size / texture_size.x, size / texture_size.y);
	bounds = _sorcerer_sprite.getLocalBounds();
	_sorcerer_sprite.setOrigin(bounds.width / 2.f, bounds.height / 2.f);
	_sorcerer_sprite.setPosition({ 2 * third_window_width + third_window_width / 2.f, half_window_height });

	float selector_width = third_window_width / 2.f;
	_select_background.setFillColor(sf::Color::White);
	_select_background.setSize({ selector_width , WINDOW_HEIGHT });
	_select_background.setOrigin({ selector_width / 2.f , WINDOW_HEIGHT / 2.f });
	UpdateSelector();
}

void CreateScene::Update(const int64 delta_time)
{

}

void CreateScene::Draw(sf::RenderWindow& window)
{
	window.draw(_select_background);
	window.draw(_warrior_sprite);
	window.draw(_rogue_sprite);
	window.draw(_sorcerer_sprite);
}

void CreateScene::HUD(sf::RenderWindow& window)
{

}

void CreateScene::HandleInput(const sf::Event& input_event)
{
	if (input_event.type == sf::Event::KeyPressed) {
		if (input_event.key.code == sf::Keyboard::Left) {
			_button_index -= 1;
			_button_index = ((_button_index + _max_index) % _max_index);
			UpdateSelector();
		}
		if (input_event.key.code == sf::Keyboard::Right) {
			_button_index += 1;
			_button_index = (_button_index % _max_index);
			UpdateSelector();
		}
		if (input_event.key.code == sf::Keyboard::Enter) {
			uint8 class_type = ClassType::CLASS_NONE;

			switch (_button_index)
			{
			case 0:
				class_type = ClassType::WARRIOR;
				break;
			case 1:
				class_type = ClassType::ROGUE;
				break;
			case 2:
				class_type = ClassType::SORCERER;
				break;
			}

			game.SendRegister(class_type);
		}
	}
}

void CreateScene::ProcessPacket(std::vector<BYTE> packets)
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
			case LoginFailReason::USED_ID:
			{
				game.SceneLoad(SceneType::LoginScene);
			}
			break;
			default:
				std::cout << "Create Register Fail, But no idea\n";
				break;
			}
		}
		break;

		default:
			// 이 짝으로 패킷이 오면 안댜
			std::cout << "Create Scene Recv Invalid Packet ERROR\n";
			break;
		}

		processed_data_size += base_packet->_size;
	}
}

void CreateScene::UpdateSelector()
{
	const float third_window_width = WINDOW_WIDTH / 3.f;
	const float half_window_height = WINDOW_HEIGHT / 2.f;

	_select_background.setPosition({ _button_index * third_window_width + third_window_width / 2.f , half_window_height });
}
