#pragma once


class CreateScene : public BaseScene
{
private:
	sf::Sprite				_warrior_sprite;
	sf::Sprite				_rogue_sprite;
	sf::Sprite				_sorcerer_sprite;

	uint32					_button_index;
	uint32					_max_index;
	sf::RectangleShape		_select_background;

public:
	CreateScene();

	void Init();

	void Update(const int64 delta_time) override;
	void Draw(sf::RenderWindow& window) override;
	void HUD(sf::RenderWindow& window) override;
	void HandleInput(const sf::Event& input_event) override;
	void ProcessPacket(std::vector<BYTE> packet) override;

private:

	void UpdateSelector();

};

