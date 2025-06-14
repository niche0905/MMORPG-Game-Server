#pragma once


class LoginScene : public BaseScene
{
private:
	sf::RectangleShape _input_box;
	sf::Text _nickname_text;
	sf::Text _system_text;

	std::string _input_buffer;

public:
	LoginScene();

	void Init();

	void Update(const int64 delta_time) override;
	void Draw(sf::RenderWindow& window) override;
	void HUD(sf::RenderWindow& window) override;
	void HandleInput(const sf::Event& input_event) override;
	void ProcessPacket(std::vector<BYTE> packet) override;

private:
	void UpdateNicknameText();

};

