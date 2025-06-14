#include "pch.h"
#include "TitleScene.h"
#include "TextureManager.h"
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
	// TODO: 버튼들과 선택 Rect 그리기
	_start_button.Draw(window);
	_exit_button.Draw(window);
}

void TitleScene::HUD(sf::RenderWindow& window)
{
	// 할게 없으여
}

void TitleScene::HandleInput(const sf::Event& input_event)
{
	// TODO: 키 입력에 따라 뭐냐 선택 Rect 옮기기
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

sf::Vector2f TitleScene::GetCameraCenter() const
{
	return { WINDOW_WIDTH / 2.f, WINDOW_HEIGHT / 2.f };
}
