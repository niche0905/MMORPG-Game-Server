#include "pch.h"
#include "TitleScene.h"
#include "TextureManager.h"
#include "FontManager.h"
#include "Button.h"


TitleScene::TitleScene()
	: _start_button{ 0 }
	, _exit_button{ 1 }
	, _max_index{ 2 }
	, _button_index{ 0 }
{
	Init();
}

void TitleScene::Init()
{
	float mid_window = WINDOW_WIDTH / 2.f;

	_title.setFont(FontManager::Instance().GetFont("neodot"));
	_title.setString("MMORPG");
	_title.setCharacterSize(50);
	_title.setFillColor(sf::Color::Red);
	_title.setOutlineColor(sf::Color::White);
	_title.setStyle(sf::Text::Regular);
	_title.setOutlineThickness(3.f);

	sf::FloatRect bounds = _title.getLocalBounds();
	_title.setOrigin(bounds.width / 2.f, bounds.height / 2.f);

	_title.setPosition({ mid_window, 200 });

	float selecter_height = BIG_BUTTON_SIZE / 2.f;
	_select_background.setFillColor(sf::Color::Yellow);
	_select_background.setSize({ WINDOW_WIDTH , selecter_height });
	_select_background.setOrigin({ mid_window , selecter_height / 2.f });
	_select_background.setPosition({ mid_window , 400 });

	_start_button.SetTexture(TextureManager::Instance().GetTexture("button_big"));
	_start_button.SetText("Start");
	_start_button.SetSize(BIG_BUTTON_SIZE);
	_start_button.SetPosition({ mid_window , 400 });
	
	_exit_button.SetTexture(TextureManager::Instance().GetTexture("button_big"));
	_exit_button.SetText("Exit");
	_exit_button.SetSize(BIG_BUTTON_SIZE);
	_exit_button.SetPosition({ mid_window , 600 });

}

void TitleScene::Update(const int64 delta_time)
{
	// 할게 없으여
}

void TitleScene::Draw(sf::RenderWindow& window)
{
	window.draw(_title);
	window.draw(_select_background);
	_start_button.Draw(window);
	_exit_button.Draw(window);
}

void TitleScene::HUD(sf::RenderWindow& window)
{
	// 할게 없으여
}

void TitleScene::HandleInput(const sf::Event& input_event)
{
	if (input_event.type == sf::Event::KeyPressed) {
		if (input_event.key.code == sf::Keyboard::Up) {
			_button_index -= 1;
			_button_index = ((_button_index + _max_index) % _max_index);
			MoveSelector();
		}
		if (input_event.key.code == sf::Keyboard::Down) {
			_button_index += 1;
			_button_index = (_button_index % _max_index);
			MoveSelector();
		}
		if (input_event.key.code == sf::Keyboard::Enter) {
			if (_start_button.GetBID() == _button_index) {
				// TODO: 다음 씬으로 넘어가기
			}
			else if (_exit_button.GetBID() == _button_index) {
				game.Exit();
			}
			else {
				// 있을 수 없는 일
				std::cout << "Title Scene Selector Button ERROR\n";
			}
		}
	}
}

void TitleScene::ProcessPacket(std::vector<BYTE> packets)
{
	// 여기서는 패킷 받을게 없을긴디 (오면 이상헌겨)
	size_t processed_data_size = 0;

	while (processed_data_size < packets.size())
	{
		BYTE* packet = packets.data() + processed_data_size;

		BASE_PACKET* base_packet = reinterpret_cast<BASE_PACKET*>(packet);

		switch (base_packet->_packet_id)
		{
		default:
			// 이 짝으로 패킷이 오면 안댜
			std::cout << "Title Scene Recv Packet ERROR\n";
			break;
		}

		processed_data_size += base_packet->_size;
	}
}

void TitleScene::MoveSelector()
{
	const float mid_window = WINDOW_WIDTH / 2.f;

	_select_background.setPosition({ mid_window , static_cast<float>(400 + (200 * _button_index)) });
}
