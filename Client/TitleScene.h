#pragma once
#include "Button.h"

class Button;


class TitleScene : public BaseScene
{
private:
	sf::Text				_title;

	Button					_start_button;
	Button					_exit_button;
	uint32					_button_index;
	uint32					_max_index;
	sf::RectangleShape		_select_background;

public:
	TitleScene();

	void Init();

	void Update(const int64 delta_time) override;
	void Draw(sf::RenderWindow& window) override;
	void HUD(sf::RenderWindow& window) override;
	void HandleInput(const sf::Event& input_event) override;
	void ProcessPacket(std::vector<BYTE> packet) override;
	sf::Vector2f GetCameraCenter() const override;

};

